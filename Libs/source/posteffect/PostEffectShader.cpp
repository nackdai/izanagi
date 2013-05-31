#include "posteffect/PostEffectShader.h"
#include "posteffect/functor/PostEffectFunctor.h"
#include "posteffect/util/PostEffectUtilityRect.h"
#include "posteffect/vs/PostEffectVSManager.h"

using namespace izanagi;

////////////////////////////////////////////////////////////

CPostEffectShader::SRenderParam::SRenderParam()
{
    tech_idx = -1;
    pass_idx = -1;
    functor = IZ_NULL;
    render_target = IZ_NULL;
}

void CPostEffectShader::SRenderParam::Clear()
{
    tech_idx = -1;
    pass_idx = -1;
    SAFE_RELEASE(functor);
    SAFE_RELEASE(render_target);
}

IZ_BOOL CPostEffectShader::SRenderParam::IsValid()
{
    return ((tech_idx >= 0)
            && (pass_idx >= 0)
            && (functor != IZ_NULL));
}

////////////////////////////////////////////////////////////

// ********************************************************************
// NOTE
// テクスチャについてのパラメータハンドルは
// 一番上のビットを１にする

#define _IZ_POSTEFFECT_HANDLE_MASK_IS_TEX       (0x80000000)
#define _IZ_POSTEFFECT_HANDLE_MASK              (~_IZ_POSTEFFECT_HANDLE_MASK_IS_TEX)
#define _IZ_POSTEFFECT_TEX_HANDLE(n)            ((n) | _IZ_POSTEFFECT_HANDLE_MASK_IS_TEX);
#define _GET_IDX_FROM_IZ_POSTEFFECT_HANDLE(n)   ((n) & _IZ_POSTEFFECT_HANDLE_MASK)
#define _IS_TEX_HANDLE(n)   (((n) & _IZ_POSTEFFECT_HANDLE_MASK_IS_TEX) > 0)

// ********************************************************************

CPostEffectShader* CPostEffectShader::CreatePostEffectShader(
    IMemoryAllocator* pAllocator,
    graph::CGraphicsDevice* pDevice,
    CPostEffectVSManager* pVSMgr,
    IInputStream* in)
{
    // ファイルヘッダ読み込み
    S_PES_HEADER sHeader;
    IZ_INPUT_READ_ASSERT(in, (IZ_UINT8*)&sHeader, 0, sizeof(sHeader));

    // TODO
    // マジックナンバーチェック
    //VRETURN_NULL(IS_PES_FORMAT(sHeader.magic));

    // バッファサイズ
    size_t nInstanceSize = sizeof(CPostEffectShader);
    size_t nBufSize = sHeader.posProgram - sHeader.sizeHeadaer;
    size_t nReadSize = nBufSize;

    nBufSize += CPostEffectTextureTable::ComputeTexBufferSize(sHeader.numTex);
    nBufSize += CPostEffectTextureTable::ComputeTexBufferSize(sHeader.numTex);  // for temporaly
    nBufSize += sizeof(CPostEffectPass) * sHeader.numPass;
    nBufSize += sHeader.sizePassBuffForParamIdx;

    // バッファ確保
    IZ_UINT8* pBuffer = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(pAllocator, nInstanceSize + nBufSize));
    VRETURN_NULL(pBuffer != IZ_NULL);

    CPostEffectShader* pInstance = new(pBuffer) CPostEffectShader;
    {
        pBuffer += sizeof(CPostEffectShader);

        pInstance->AddRef();

        pInstance->m_Allocator = pAllocator;
        SAFE_REPLACE(pInstance->m_pDevice, pDevice);
    }

    // ヘッダ直後からプログラム直前までをまとめて読む
    IZ_BOOL result = IZ_INPUT_READ(in, pBuffer, 0, nReadSize);
    VGOTO(result, __EXIT__);

    result = pInstance->Init(
                nBufSize,
                pBuffer,
                &sHeader,
                pVSMgr,
                in);
    VGOTO(result, __EXIT__);

__EXIT__:
    if (!result) {
        SAFE_RELEASE(pInstance);
    }
    return pInstance;
}

// コンストラクタ
CPostEffectShader::CPostEffectShader()
{
    m_Allocator = IZ_NULL;
    m_pDevice = IZ_NULL;

    m_pPass = IZ_NULL;

    m_bIsBeginScene = IZ_FALSE;
}

// デストラクタ
CPostEffectShader::~CPostEffectShader()
{
    for (IZ_UINT i = 0; i < m_sHeader.numPass; ++i) {
        m_pPass[i].Clear();
    }

    SAFE_RELEASE(m_pDevice);
}

/**
* 描画
*/
IZ_BOOL CPostEffectShader::Render(
    const SIntRect* pSrcRect,
    const SIntRect* pDstRect)
{
    VRETURN(m_RenderParam.IsValid());

    // パス取得
    CPostEffectPass* pPass = GetPass(
                                m_RenderParam.tech_idx,
                                m_RenderParam.pass_idx);

    const S_PES_PASS* pDesc = pPass->GetDesc();

    // 入力テクスチャを探す
    graph::CTexture* pSrcTex = IZ_NULL;
    if (pPass->GetSamplerNum() > 0) {
        const SSmplInfo* pSmplInfo = pPass->GetSamplerInfo(0);
        const S_PES_SAMPLER* pSmplDesc = GetSamplerDesc(pSmplInfo->idx);
        
        pSrcTex = GetTexture(pSmplDesc->state.BindTexIdx);
    }

    // 入力矩形
    SIntRect rcSrc;

    if (pSrcRect == IZ_NULL) {
        if (pSrcTex != IZ_NULL) {
            // 入力テクスチャが存在する
            CPostEffectRectUtil::GetTextureRect(pSrcTex, &rcSrc);
            CPostEffectRectUtil::InflateRect(
                &rcSrc,
                pDesc->ann.SrcTexRectInflate[0],
                pDesc->ann.SrcTexRectInflate[1]);
        }
    }
    else {
        // 外部設定を使う
        rcSrc = *pSrcRect;
    }

    // シザリングしたかどうか
    IZ_BOOL bIsScissor = IZ_FALSE;

    // 出力矩形
    SIntRect rcDst;

    if (m_RenderParam.render_target != IZ_NULL) {
        // レンダーターゲットが存在する

        // レンダーターゲットの矩形を取得する
        SIntRect rcRenderTarget;
        CPostEffectRectUtil::GetTextureRect(
            m_RenderParam.render_target,
            &rcRenderTarget);

        // 出力矩形を設定
        if (pDstRect == IZ_NULL) {
            // シザリング設定にinflate
            CPostEffectRectUtil::InflateRect(
                &rcDst,
                rcRenderTarget,
                pDesc->ann.ScissorRectInflate[0],
                pDesc->ann.ScissorRectInflate[1]);
        }
        else {
            // 外部設定を使う
            rcDst = *pDstRect;
        }

        if (!CPostEffectRectUtil::CmpRect(rcDst, rcRenderTarget)) {
            // レンダーターゲットの矩形と出力矩形が一致しない
            // シザリング設定
            m_pDevice->SetRenderState(
                graph::E_GRAPH_RS_SCISSORTESTENABLE,
                IZ_TRUE);
            m_pDevice->SetScissorTestRect(rcDst);

            bIsScissor = IZ_TRUE;
        }
    }

    // テクスチャ座標矩形を取得
    SFloatRect rcTexCoord;
    CPostEffectRectUtil::GetTextureCoord(
        &rcTexCoord,
        pSrcTex,
        &rcSrc,
        m_RenderParam.render_target,
        &rcDst);

    // 描画
    pPass->Render(m_pDevice, rcTexCoord);

    if (bIsScissor) {
        // シザリングを解除
        m_pDevice->SetRenderState(
            graph::E_GRAPH_RS_SCISSORTESTENABLE,
            IZ_FALSE);
    }

    return IZ_TRUE;
}

/**
* 描画
*/
IZ_BOOL CPostEffectShader::Render(const SFloatRect& rcTexCoord)
{
    if (!m_RenderParam.IsValid()) {
        IZ_ASSERT(IZ_FALSE);
        return IZ_FALSE;
    }

    // パス取得
    CPostEffectPass* pPass = GetPass(
                                m_RenderParam.tech_idx,
                                m_RenderParam.pass_idx);

    // 描画
    pPass->Render(m_pDevice, rcTexCoord);

    return IZ_TRUE;
}

/**
* 描画開始
*/
IZ_BOOL CPostEffectShader::BeginRender(
    CPostEffectFunctor* pFunctor,
    IZ_UINT nTechIdx,
    IZ_UINT nPassIdx,
    graph::CTexture* pTex,
    IZ_BOOL bIsClear)
{
    IZ_ASSERT(pFunctor != IZ_NULL);

    IZ_BOOL ret = IZ_TRUE;

    // パス取得
    CPostEffectPass* pPass = GetPass(nTechIdx, nPassIdx);
    const S_PES_PASS* pDesc = pPass->GetDesc();

    // レンダーステート変更
    {
        // アルファブレンド
        {
            IZ_BOOL bEnable = pDesc->state.AlphaBlendEnable;
            IZ_UINT nMethod = pDesc->state.AlphaBlendMethod;

            if (m_StateAlphaBlend.IsDirty()) {
                // 外部設定を利用する
                bEnable = m_StateAlphaBlend.enable;
                nMethod = m_StateAlphaBlend.method;
            }

            m_pDevice->SetRenderState(graph::E_GRAPH_RS_ALPHABLENDENABLE, bEnable);
            m_pDevice->SetRenderState(graph::E_GRAPH_RS_BLENDMETHOD, nMethod);
        }

        // アルファテスト
        {
            IZ_BOOL bEnable = pDesc->state.AlphaTestEnable;
            graph::E_GRAPH_CMP_FUNC nFunc = pDesc->state.AlphaTestFunc;
            IZ_UINT8 nRef = pDesc->state.AlphaTestRef;
            if (m_StateAlphaTest.IsDirty()) {
                // 外部設定を利用する
                bEnable = m_StateAlphaTest.enable;
                nFunc = m_StateAlphaTest.func;
                nRef = m_StateAlphaTest.ref;
            }

            m_pDevice->SetRenderState(graph::E_GRAPH_RS_ALPHABLENDENABLE, bEnable);
            m_pDevice->SetRenderState(graph::E_GRAPH_RS_ALPHAFUNC, nFunc);
            m_pDevice->SetRenderState(graph::E_GRAPH_RS_ALPHAREF, nRef);
        }
    }

    // レンダーターゲット切り替え
    if (pTex != IZ_NULL) {
        graph::CRenderTarget* rt = pTex->AsRenderTarget();
        IZ_ASSERT(rt != IZ_NULL);

        IZ_UINT nClearFlag = 0;
        IZ_COLOR nClearColor = 0;

#if 0
        if (pPassPesAnn->isColorClear) {
#else
        if (pDesc->ann.isClearColor && bIsClear) {
#endif
            nClearFlag = graph::E_GRAPH_CLEAR_FLAG_COLOR;
            nClearColor = pDesc->ann.ClearColor;
        }

        // レンダーターゲットを切り替える
        ret = m_pDevice->BeginScene(
                &rt,
                1,
                nClearFlag,
                nClearColor);
        IZ_ASSERT(ret);

        if (ret) {
            // レンダーターゲット保持
            SAFE_REPLACE(m_RenderParam.render_target, pTex);

            m_bIsBeginScene = IZ_TRUE;
        }
    }

    if (ret) {
        // 描画パラメータ保持
        m_RenderParam.tech_idx = nTechIdx;
        m_RenderParam.pass_idx = nPassIdx;
        SAFE_REPLACE(m_RenderParam.functor, pFunctor);
    }

    return ret;
}

/**
* 描画開始
*/
IZ_BOOL CPostEffectShader::BeginRender(
    CPostEffectFunctor* pFunctor,
    IZ_UINT nTechIdx,
    IZ_UINT nPassIdx)
{
    IZ_BOOL ret = IZ_TRUE;
    graph::CTexture* pTex = IZ_NULL;

    // パス取得
    CPostEffectPass* pPass = GetPass(nTechIdx, nPassIdx);
    const S_PES_PASS* pDesc = pPass->GetDesc();

    // 次のパスに向けてレンダーターゲットを切り替えるか
    if (pDesc->ann.RenderTargetIdx >= 0) {
        pTex = GetTexture(pDesc->ann.RenderTargetIdx);
        IZ_ASSERT(pTex != IZ_NULL);
    }

    ret = BeginRender(
            pFunctor,
            nTechIdx,
            nPassIdx,
            pTex,
            IZ_TRUE);

    return ret;
}

/**
* 描画終了
*/
IZ_BOOL CPostEffectShader::EndRender()
{
    if (m_bIsBeginScene) {
        // BeginRender()が呼ばれている
        m_pDevice->EndScene(graph::E_GRAPH_END_SCENE_FLAG_RT_0);
        m_bIsBeginScene = IZ_FALSE;
    }

    if (!m_RenderParam.IsValid()) {
        IZ_ASSERT(IZ_FALSE);
        return IZ_FALSE;
    }

    // パス取得
    CPostEffectPass* pPass = GetPass(
                                m_RenderParam.tech_idx,
                                m_RenderParam.pass_idx);

    IZ_UINT nSmplNum = pPass->GetSamplerNum();

    // セットした分だけクリアする
    for (IZ_UINT i = 0; i < nSmplNum; ++i) {
        const SHADER_PARAM_HANDLE handle = pPass->GetSamplerInfo(i)->handle;
        IZ_UINT nRscId = pPass->GetSamplerInfo(i)->resource_id;

        if (handle != IZ_NULL) {
            m_pDevice->SetTexture(
                nRscId,
                IZ_NULL);
        }
    }

    // 描画パラメータクリア
    m_RenderParam.Clear();
    SAFE_RELEASE(m_RenderParam.functor);

    // レンダーステートクリア
    m_StateAlphaBlend.SetDisable();
    m_StateAlphaTest.SetDisable();

    return IZ_TRUE;
}

IZ_POSTEFFECT_HANDLE CPostEffectShader::GetParamHandleByName(IZ_PCSTR pszName) const
{
    IZ_INT ret = m_cConstTable.GetIdxByName(pszName);

    if (ret < 0) {
        ret = m_cTexTable.GetIdxByName(pszName);

        VRETURN_VAL(ret >= 0, IZ_POSTEFFECT_HANDLE_INVALID);
        ret = _IZ_POSTEFFECT_TEX_HANDLE(ret);
    }

    return ret;
}

IZ_POSTEFFECT_HANDLE CPostEffectShader::GetParamHandleBySemantic(IZ_PCSTR pszSemantic) const
{
    IZ_INT ret = m_cConstTable.GetIdxBySemantic(pszSemantic);
    
    if (ret < 0) {
        ret = m_cTexTable.GetIdxBySemantic(pszSemantic);

        VRETURN_VAL(ret >= 0, IZ_POSTEFFECT_HANDLE_INVALID);
        ret = _IZ_POSTEFFECT_TEX_HANDLE(ret);
    }

    return ret;
}

IZ_POSTEFFECT_HANDLE CPostEffectShader::GetParamHandleByIdx(IZ_UINT nIdx)
{
    IZ_ASSERT(nIdx < m_sHeader.numParam);
    return nIdx;
}

IZ_POSTEFFECT_HANDLE CPostEffectShader::GetTexHandleByIdx(IZ_UINT nIdx)
{
    IZ_ASSERT(nIdx < m_sHeader.numTex);

    IZ_INT ret = _IZ_POSTEFFECT_TEX_HANDLE(nIdx);
    return ret;
}

/**
* パラメータ値をセットする
*/
IZ_BOOL CPostEffectShader::SetParameter(
    IZ_POSTEFFECT_HANDLE nHandle,
    const void* pValue,
    IZ_UINT nBytes)
{
    IZ_ASSERT(nHandle != IZ_POSTEFFECT_HANDLE_INVALID);
    IZ_ASSERT(pValue != IZ_NULL);

    VRETURN(!_IS_TEX_HANDLE(nHandle));

    const S_PES_PARAMETER* pDesc = m_cConstTable.GetDesc(nHandle);
    VRETURN(pDesc != IZ_NULL);
    VRETURN(pDesc->Bytes == nBytes);

    m_cConstTable.SetParam(
        nHandle,
        pValue,
        nBytes);

    return IZ_TRUE;
}

/**
* テクスチャをセットする
*/
IZ_BOOL CPostEffectShader::SetTexture(
    IZ_POSTEFFECT_HANDLE nHandle,
    graph::CTexture* pTex)
{
    IZ_ASSERT(nHandle != IZ_POSTEFFECT_HANDLE_INVALID);
    IZ_ASSERT(pTex != IZ_NULL);

    VRETURN(_IS_TEX_HANDLE(nHandle));

    IZ_UINT idx = _GET_IDX_FROM_IZ_POSTEFFECT_HANDLE(nHandle);

#if 0
    IZ_BOOL ret = m_cTexTable.SetTexture(idx, pTex);
#else
    IZ_BOOL ret = m_cTexTableTmp.SetTexture(idx, pTex);
#endif
    return ret;
}

/**
* 直接頂点シェーダにテクスチャオフセットパラメータをセットする
*/
IZ_BOOL CPostEffectShader::SetTextureOffsetParameter(
    const math::SVector* pVector,
    IZ_UINT num)
{
    VRETURN(m_RenderParam.IsValid());

    // パス取得
    CPostEffectPass* pPass = GetPass(
                                m_RenderParam.tech_idx,
                                m_RenderParam.pass_idx);

    // パラメータセット
    pPass->GetPostEffectVS()->RegisterParameter(pVector, num);

    return IZ_TRUE;
}

/**
* テクスチャオフセットパラメータをセットする
*/
IZ_BOOL CPostEffectShader::SetTextureOffsetParameter(const graph::CTexture* pTex)
{
    VRETURN(m_RenderParam.IsValid());

    if (pTex == IZ_NULL) {
        // こういうこともある・・・
        return IZ_TRUE;
    }

    // テクスチャオフセットをセットする
    IZ_BOOL ret = IZ_TRUE;

    // パス記述取得
    CPostEffectPass* pPass = GetPass(
                                m_RenderParam.tech_idx,
                                m_RenderParam.pass_idx);
    const S_PES_PASS* pPassDesc = pPass->GetDesc();

    IZ_UINT nOffsetVectorNum = 0;   // オフセット数
    math::SVector vecOffset[16];            // オフセット

    if (pPassDesc->ann.TexOffsetParamIdx >= 0) {
        // パラメータ記述取得
        const S_PES_PARAMETER* pParamDesc = m_cConstTable.GetDesc(pPassDesc->ann.TexOffsetParamIdx);
        VRETURN(pParamDesc != IZ_NULL);

        // パラメータ取得
        IZ_UINT nBytes = 0;
        const void* pValue = m_cConstTable.GetParam(
                                pPassDesc->ann.TexOffsetParamIdx,
                                &nBytes);

        if (pValue != IZ_NULL) {
            // パラメータが存在するのでコピー
            IZ_ASSERT(nBytes <= sizeof(vecOffset));
            memcpy(
                vecOffset,
                pValue,
                nBytes);

            nOffsetVectorNum = pParamDesc->Elements;
        }
    }

    if (nOffsetVectorNum > 0) {
        IZ_FLOAT fTexelW = 1.0f / pTex->GetWidth();
        IZ_FLOAT fTexelH = 1.0f / pTex->GetHeight();

        for (IZ_UINT i = 0; i < nOffsetVectorNum; ++i) {
            vecOffset[i].x *= fTexelW;
            vecOffset[i].y *= fTexelH;
            vecOffset[i].z *= fTexelW;
            vecOffset[i].w *= fTexelH;
        }

        // 頂点シェーダにセット
        ret = SetTextureOffsetParameter(
                vecOffset,
                nOffsetVectorNum);
    }

    return ret;
}

/**
* コミットチェンジ
*/
IZ_BOOL CPostEffectShader::CommitChanges()
{
    VRETURN(m_RenderParam.IsValid());

    IZ_BOOL ret = IZ_TRUE;

    // パス取得
    CPostEffectPass* pPass = GetPass(
                                m_RenderParam.tech_idx,
                                m_RenderParam.pass_idx);

    IZ_UINT nParamNum = pPass->GetParamNum();
    IZ_UINT nSmplNum = pPass->GetSamplerNum();

    // パラメータセット
    for (IZ_UINT i = 0; i < nParamNum; ++i) {
        IZ_UINT nParamIdx = pPass->GetParamInfo(i)->idx;
        SHADER_PARAM_HANDLE handle = pPass->GetParamInfo(i)->handle;

        const S_PES_PARAMETER* pParamDesc = m_cConstTable.GetDesc(nParamIdx);
        VRETURN(pParamDesc != IZ_NULL);

        // パラメータ取得
        const void* pValue = m_cConstTable.GetParam(nParamIdx, IZ_NULL);

        if ((pValue != IZ_NULL)
            && (handle != IZ_NULL))
        {
            IZ_UINT nElements = (pParamDesc->Elements > 0
                                    ? pParamDesc->Elements
                                    : 1);

            m_pDevice->SetShaderProgram(pPass->GetShaderProgram());

            ret = CPostEffectShaderUtil::SetValue(
                    m_pDevice,
                    pPass->GetShaderProgram(),
                    handle,
                    pValue,
                    pParamDesc->Type,
                    nElements);

            VRETURN(ret);
        }
    }

    // テクスチャセット
    for (IZ_UINT i = 0; i < nSmplNum; ++i) {
        IZ_UINT nSmplIdx = pPass->GetSamplerInfo(i)->idx;
        SHADER_PARAM_HANDLE handle = pPass->GetSamplerInfo(i)->handle;
        IZ_UINT nRscId = pPass->GetSamplerInfo(i)->resource_id;

        if (handle != IZ_NULL) {
            // サンプラ記述取得
            const S_PES_SAMPLER* pSmplDesc = m_cSmplTable.GetDesc(nSmplIdx);
            VRETURN(pSmplDesc);

            // テクスチャ取得
            graph::CTexture* pTex = m_cTexTableTmp.GetTextureIfDirty(
                                pSmplDesc->state.BindTexIdx);
            if (pTex == IZ_NULL) {
                pTex = m_cTexTable.GetTexture(
                        pSmplDesc->state.BindTexIdx);
            }

            if (pTex != IZ_NULL) {
                // ステートを反映
                pTex->SetAddress(
                    pSmplDesc->state.addrU,
                    pSmplDesc->state.addrV);
                pTex->SetFilter(
                    pSmplDesc->state.magFilter,
                    pSmplDesc->state.minFilter,
                    graph::E_GRAPH_TEX_FILTER_NONE);

                // テクスチャセット
                m_pDevice->SetTexture(
                    nRscId,
                    pTex);
            }
        }
    }

    m_cTexTableTmp.Clean();

    return ret;
}

// 初期化
IZ_BOOL CPostEffectShader::Init(
    size_t nBufSize,
    IZ_UINT8* pBuffer,
    const S_PES_HEADER* pHeader,
    CPostEffectVSManager* pVSMgr,
    IInputStream* in)
{
    memcpy(&m_sHeader, pHeader, sizeof(S_PES_HEADER));

    // バッファポインタ位置
    IZ_UINT8* pBuf = pBuffer;

    // それぞれのテーブルを初期化していく
    {
        // 定数テーブル
        pBuf = m_cConstTable.Init(m_sHeader, pBuf);

        // テクスチャテーブル
        pBuf = m_cTexTable.Init(m_sHeader, pBuf);

        // サンプラテーブル
        pBuf = m_cSmplTable.Init(m_sHeader, pBuf);

        // テクニックテーブル
        pBuf = m_cTechTable.Init(m_sHeader, pBuf);

        // パス
        pBuf = m_cPassTable.Init(m_sHeader, pBuf);

        // 文字列テーブル
        pBuf = m_cStrTable.Init(m_sHeader.sizeStringBuffer, pBuf);

        pBuf = m_cTexTable.SetTexBuffer(pBuf);
    }

    IZ_UINT64 tmp = CStdUtil::GetPtrDistance(pBuf, pBuffer);

    // パスデータ作成
    pBuf = CreatePass(pBuf, in);

    tmp = CStdUtil::GetPtrDistance(pBuf, pBuffer);

    // テクスチャテーブル（テンポラリ）
    pBuf = m_cTexTableTmp.SetTexBufferDirectly(
            m_sHeader.numTex,
            pBuf);

    tmp = CStdUtil::GetPtrDistance(pBuf, pBuffer);

    // チェック
    IZ_BOOL ret = (CStdUtil::GetPtrDistance(pBuf, pBuffer) == nBufSize);
    VRETURN(ret);

    // 頂点プログラム作成 & 割り当て
    {
        void* pProgramBuf = ALLOC_ZERO(m_Allocator, m_sHeader.maxProgamSize);
        VRETURN(pProgramBuf);

        ret = pVSMgr->CreateVS(
                m_sHeader.numVtxProgram,
                m_Allocator,
                m_pDevice,
                reinterpret_cast<IZ_UINT8*>(pProgramBuf),
                in);
        VGOTO(ret, __EXIT__);

        for (IZ_UINT i = 0; i < m_sHeader.numPass; ++i) {
            CPostEffectPass& cPass = m_pPass[i];
            const S_PES_PASS* pDesc = cPass.GetDesc();
            IZ_ASSERT(pDesc != IZ_NULL);

            CPostEffectVS* pVS = pVSMgr->GetVS(pDesc->VSType);
            IZ_ASSERT(pVS != IZ_NULL);

            cPass.SetPostEffectVS(pVS);

            if (!pVS->IsInitilizedShaderParameter()) {
                pVS->InitShaderParameter(
                    m_pDevice,
                    cPass.GetShaderProgram());
            }
        }

__EXIT__:
        FREE(m_Allocator, pProgramBuf);
    }

    // パスパラメータ初期化
    InitPassParameter(m_pDevice);

    return IZ_TRUE;
}

// パスデータ作成
IZ_UINT8* CPostEffectShader::CreatePass(
    IZ_UINT8* pBuffer,
    IInputStream* in)
{
    IZ_ASSERT(m_Allocator != IZ_NULL);
    IZ_ASSERT(m_pDevice != IZ_NULL);

    // プログラム読み込み用バッファ
    void* pProgramBuf = ALLOC(m_Allocator, m_sHeader.maxProgamSize);
    IZ_ASSERT(pProgramBuf != IZ_NULL);

    m_pPass = reinterpret_cast<CPostEffectPass*>(pBuffer);

    pBuffer += sizeof(CPostEffectPass) * m_sHeader.numPass;

    for (IZ_UINT i = 0; i < m_sHeader.numPass; ++i) {
        CPostEffectPass& cPass = m_pPass[i];
        const S_PES_PASS* pDesc = m_cPassTable.GetDesc(i);

        cPass.Init(i, pDesc);
        pBuffer = cPass.SetParamBuffer(pDesc->numConst, pBuffer);
        pBuffer = cPass.SetSamplerBuffer(pDesc->numSampler, pBuffer);

        // プログラム読み込み
        FILL_ZERO(pProgramBuf, m_sHeader.maxProgamSize);
        IZ_INPUT_READ_ASSERT(in, pProgramBuf, 0, pDesc->sizeProgram);

        // ピクセルシェーダ作成
        graph::CPixelShader* pPS = m_pDevice->CreatePixelShader(pProgramBuf);
        IZ_ASSERT(pPS != IZ_NULL);

        // シェーダプログラム作成
        graph::CShaderProgram* shader = m_pDevice->CreateShaderProgram();
        IZ_ASSERT(shader != IZ_NULL);

        // オーナーをシェーダプログラムに委譲する
        shader->AttachPixelShader(pPS);
        SAFE_RELEASE(pPS);

        // オーナーをパスに委譲する
        cPass.SetShaderProgram(shader);
        SAFE_RELEASE(shader);
    }

    FREE(m_Allocator, pProgramBuf);

    return pBuffer;
}

IZ_BOOL CPostEffectShader::InitPassParameter(graph::CGraphicsDevice* device)
{
    for (IZ_UINT i = 0; i < m_sHeader.numPass; ++i) {
        CPostEffectPass& cPass = m_pPass[i];
        const S_PES_PASS* pDesc = m_cPassTable.GetDesc(i);

        // パラメータ情報初期化
        for (IZ_UINT n = 0; n < pDesc->numConst; ++n) {
            IZ_UINT nParamIdx = pDesc->GetConstIdx(n);

            const S_PES_PARAMETER* pParamDesc = m_cConstTable.GetDesc(nParamIdx);
            IZ_ASSERT(pParamDesc != IZ_NULL);

            IZ_PCSTR pszName = m_cStrTable.GetString(pParamDesc->posName);
            IZ_ASSERT(pszName != IZ_NULL);

            cPass.InitParam(n, nParamIdx, pszName);
        }

        // サンプラ情報初期化
        for (IZ_UINT n = 0; n < pDesc->numSampler; ++n) {
            IZ_UINT nSmplIdx = pDesc->GetSmplIdx(n);

            const S_PES_SAMPLER* pSmplDesc = m_cSmplTable.GetDesc(nSmplIdx);
            IZ_ASSERT(pSmplDesc != IZ_NULL);

            IZ_PCSTR pszName = m_cStrTable.GetString(pSmplDesc->posName);
            IZ_ASSERT(pszName != IZ_NULL);

            VRETURN(
                cPass.InitSampler(
                    m_cSmplTable,
                    n, nSmplIdx, pszName));
        }
    }

    return IZ_TRUE;
}
