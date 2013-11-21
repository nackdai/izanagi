#include "posteffect/functor/PostEffectFunctorMGF.h"

using namespace izanagi;

IZ_BOOL CPostEffectFunctorMGF::Apply(
    graph::CGraphicsDevice* pDevice,
    CPostEffectShader* pShader)
{
    IZ_BOOL ret = IZ_TRUE;

    // 縮小
    ret = ApplyMGFDownScale(
            pDevice,
            pShader);
    if (!ret) {
        //IZ_ASSERT(IZ_FALSE);
        goto __EXIT__;
    }

    // ブルーム作成
    ret = ApplyMGFRenderBloom(
            pDevice,
            pShader);
    if (!ret) {
        //IZ_ASSERT(IZ_FALSE);
        goto __EXIT__;
    }

#ifdef __IZ_DEBUG__
        graph::CGraphPerf::BeginEvent(0, "passMGFMerge");
#endif  // #ifdef __IZ_DEBUG__

    // マージ
    ret = ApplyMGFMerge(
            pDevice,
            pShader);
    if (!ret) {
        //IZ_ASSERT(IZ_FALSE);
        goto __EXIT__;
    }

#ifdef __IZ_DEBUG__
        graph::CGraphPerf::EndEvent();
#endif  // #ifdef __IZ_DEBUG__

__EXIT__:
    return ret;
}

// ファンクタ独自のテクスチャ作成
IZ_BOOL CPostEffectFunctorMGF::CreateTexture(
    CPostEffect* pPostEffect,
    CPostEffectShader* pShader,
    CPostEffectTextureCreator* pTexCreator,
    graph::CGraphicsDevice* pDevice)
{
    IZ_UINT nValidCnt = MGF_LOOP_NUM;

    IZ_UINT nTechIdx = GetTechIdx();

    // ダウンスケール
    {
        // 最初だけは特別扱い
        {
            IZ_UINT nPassIdx = GetPassIdx();

            // 入力
            IZ_UINT nSrcTexIdx = pShader->GetSrcTexIdx(0, nTechIdx, nPassIdx);
            graph::CTexture* pSrc = pShader->GetTexture(nSrcTexIdx);

            // 出力
            IZ_UINT nDstTexIdx = pShader->GetDstTexIdx(nTechIdx, nPassIdx);
            graph::CTexture* pDst = pShader->GetTexture(nDstTexIdx);

            SAFE_REPLACE(m_DownScaleState[0].pSrc, pSrc);
            SAFE_REPLACE(m_DownScaleState[0].pDst, pDst);

            m_DownScaleState[0].nSrcTexIdx = nSrcTexIdx;
            m_DownScaleState[0].nDstTexIdx = nDstTexIdx;
        }

        for (IZ_UINT i = 1; i < MGF_LOOP_NUM; ++i) {
            // 入力は前の出力
            SAFE_REPLACE(
                m_DownScaleState[i].pSrc,
                m_DownScaleState[i - 1].pDst);

            // テクスチャサイズ
            // 必ず+2されているものとする・・・
            IZ_INT nW = m_DownScaleState[i].pSrc->GetWidth() - 2;
            IZ_INT nH = m_DownScaleState[i].pSrc->GetHeight() - 2;

            // 無いとは思うが念のため・・・
            IZ_ASSERT((nW >= 0) && (nH >= 0));

            // 1/2
            nW >>= 1;
            nH >>= 1;

            if ((nW == 0) || (nH == 0)) {
                // 小さすぎる・・・
                break;
            }

            graph::E_GRAPH_PIXEL_FMT fmt = m_DownScaleState[i].pSrc->GetPixelFormat();

            // 出力
            graph::CTexture* pDst = pTexCreator->Create(
                                pDevice,
                                nW + 2,
                                nH + 2,
                                fmt,
                                IZ_TRUE, 
                                graph::E_GRAPH_RSC_USAGE_STATIC, // RenderTargetなので何でもいい
                                E_POSTEFFECT_TEXTURE_TYPE_NONE);

            SAFE_REPLACE(m_DownScaleState[i].pDst, pDst);

            // テクスチャインデックスは全て共通
            m_DownScaleState[i].nSrcTexIdx = m_DownScaleState[0].nSrcTexIdx;
            m_DownScaleState[i].nDstTexIdx = m_DownScaleState[0].nDstTexIdx;
        }
    }

    // ブルーム
    {
        {
            // 最初
            {
                IZ_UINT nPassIdx = GetConnectPassIdx(MGF_BLOOM_STATE_INIT);

                // 入力
                IZ_UINT nSrcTexIdx = pShader->GetSrcTexIdx(0, nTechIdx, nPassIdx);
                graph::CTexture* pSrc = pShader->GetTexture(nSrcTexIdx);

                // 出力
                IZ_UINT nDstTexIdx = pShader->GetDstTexIdx(nTechIdx, nPassIdx);
                graph::CTexture* pDst = pShader->GetTexture(nDstTexIdx);

                SAFE_REPLACE(m_BloomState[0][MGF_BLOOM_STATE_INIT].pSrc, pSrc);
                SAFE_REPLACE(m_BloomState[0][MGF_BLOOM_STATE_INIT].pDst, pDst);

                m_BloomState[0][MGF_BLOOM_STATE_INIT].nSrcTexIdx = nSrcTexIdx;
                m_BloomState[0][MGF_BLOOM_STATE_INIT].nDstTexIdx = nDstTexIdx;
            }

            // 途中
            {
                IZ_UINT nPassIdx = GetConnectPassIdx(MGF_BLOOM_STATE_ITER);

                // 入力
                IZ_UINT nSrcTexIdx = pShader->GetSrcTexIdx(0, nTechIdx, nPassIdx);
                graph::CTexture* pSrc = pShader->GetTexture(nSrcTexIdx);

                // 出力
                IZ_UINT nDstTexIdx = pShader->GetDstTexIdx(nTechIdx, nPassIdx);
                graph::CTexture* pDst = pShader->GetTexture(nDstTexIdx);

                SAFE_REPLACE(m_BloomState[0][MGF_BLOOM_STATE_ITER].pSrc, pSrc);
                SAFE_REPLACE(m_BloomState[0][MGF_BLOOM_STATE_ITER].pDst, pDst);

                m_BloomState[0][MGF_BLOOM_STATE_ITER].nSrcTexIdx = nSrcTexIdx;
                m_BloomState[0][MGF_BLOOM_STATE_ITER].nDstTexIdx = nDstTexIdx;
            }

            // 最後
            {
                IZ_UINT nPassIdx = GetConnectPassIdx(MGF_BLOOM_STATE_FIN);

                // 入力
                IZ_UINT nSrcTexIdx = pShader->GetSrcTexIdx(0, nTechIdx, nPassIdx);
                graph::CTexture* pSrc = pShader->GetTexture(nSrcTexIdx);

                // 出力
                IZ_UINT nDstTexIdx = pShader->GetDstTexIdx(nTechIdx, nPassIdx);
                graph::CTexture* pDst = pShader->GetTexture(nDstTexIdx);

                // 出力は新規作成する
                pDst = pTexCreator->Create(
                        pDevice,
                        pDst->GetWidth(),
                        pDst->GetHeight(),
                        pDst->GetPixelFormat(),
                        IZ_TRUE,
                        graph::E_GRAPH_RSC_USAGE_STATIC,
                        E_POSTEFFECT_TEXTURE_TYPE_NONE);
                VRETURN(pDst != IZ_NULL);

                SAFE_REPLACE(m_BloomState[0][MGF_BLOOM_STATE_FIN].pSrc, pSrc);
                SAFE_REPLACE(m_BloomState[0][MGF_BLOOM_STATE_FIN].pDst, pDst);

                m_BloomState[0][MGF_BLOOM_STATE_FIN].nSrcTexIdx = nSrcTexIdx;
                m_BloomState[0][MGF_BLOOM_STATE_FIN].nDstTexIdx = nDstTexIdx;
            }
        }

        for (IZ_UINT i = 1; i < nValidCnt; ++i) {
            // 最初
            {
                // 入力は前(DownScale)の出力
                SAFE_REPLACE(
                    m_BloomState[i][MGF_BLOOM_STATE_INIT].pSrc,
                    m_DownScaleState[i].pDst);

                IZ_UINT nW = m_BloomState[i][MGF_BLOOM_STATE_INIT].pSrc->GetWidth();
                IZ_UINT nH = m_BloomState[i][MGF_BLOOM_STATE_INIT].pSrc->GetHeight();
                graph::E_GRAPH_PIXEL_FMT fmt = m_BloomState[i][MGF_BLOOM_STATE_INIT].pSrc->GetPixelFormat();

                // 出力
                // 同じサイズで作成する
                graph::CTexture* pDst = pTexCreator->Create(
                                    pDevice,
                                    nW,
                                    nH,
                                    fmt,
                                    IZ_TRUE,
                                    graph::E_GRAPH_RSC_USAGE_STATIC,
                                    E_POSTEFFECT_TEXTURE_TYPE_NONE);
                VRETURN(pDst != IZ_NULL);

                SAFE_REPLACE(m_BloomState[i][MGF_BLOOM_STATE_INIT].pDst, pDst);

                // テクスチャインデックスは全て共通
                m_BloomState[i][MGF_BLOOM_STATE_INIT].nSrcTexIdx = m_BloomState[0][MGF_BLOOM_STATE_INIT].nSrcTexIdx;
                m_BloomState[i][MGF_BLOOM_STATE_INIT].nDstTexIdx = m_BloomState[0][MGF_BLOOM_STATE_INIT].nDstTexIdx;
            }

            // 途中
            {
                // 入力は前(MGF_BLOOM_STATE_INIT)の出力
                SAFE_REPLACE(
                    m_BloomState[i][MGF_BLOOM_STATE_ITER].pSrc,
                    m_BloomState[i][MGF_BLOOM_STATE_INIT].pDst);

                // 使いまわす
                SAFE_REPLACE(
                    m_BloomState[i][MGF_BLOOM_STATE_ITER].pDst,
                    m_BloomState[i][MGF_BLOOM_STATE_INIT].pSrc);

                // テクスチャインデックスは全て共通
                m_BloomState[i][MGF_BLOOM_STATE_ITER].nSrcTexIdx = m_BloomState[0][MGF_BLOOM_STATE_ITER].nSrcTexIdx;
                m_BloomState[i][MGF_BLOOM_STATE_ITER].nDstTexIdx = m_BloomState[0][MGF_BLOOM_STATE_ITER].nDstTexIdx;
            }

            // 最後
            {
                // 入力は前(MGF_BLOOM_STATE_ITER)の出力
                SAFE_REPLACE(
                    m_BloomState[i][MGF_BLOOM_STATE_FIN].pSrc,
                    m_BloomState[i][MGF_BLOOM_STATE_ITER].pDst);

                // 必ず+2されているものとする・・・
                IZ_UINT nW = m_DownScaleState[i].pDst->GetWidth() - 2;
                IZ_UINT nH = m_DownScaleState[i].pDst->GetHeight() - 2;
                graph::E_GRAPH_PIXEL_FMT fmt = m_DownScaleState[i].pDst->GetPixelFormat();

                // 出力は新規作成
                graph::CTexture* pDst = pTexCreator->Create(
                                    pDevice,
                                    nW,
                                    nH,
                                    fmt,
                                    IZ_TRUE,
                                    graph::E_GRAPH_RSC_USAGE_STATIC,
                                    E_POSTEFFECT_TEXTURE_TYPE_NONE);
                VRETURN(pDst != IZ_NULL);

                SAFE_REPLACE(
                    m_BloomState[i][MGF_BLOOM_STATE_FIN].pDst,
                    pDst);

                // テクスチャインデックスは全て共通
                m_BloomState[i][MGF_BLOOM_STATE_FIN].nSrcTexIdx = m_BloomState[0][MGF_BLOOM_STATE_FIN].nSrcTexIdx;
                m_BloomState[i][MGF_BLOOM_STATE_FIN].nDstTexIdx = m_BloomState[0][MGF_BLOOM_STATE_FIN].nDstTexIdx;
            }
        }
    }

    return IZ_TRUE;
}

static char tmp[256];

// 縮小
IZ_BOOL CPostEffectFunctorMGF::ApplyMGFDownScale(
    graph::CGraphicsDevice* pDevice,
    CPostEffectShader* pShader)
{
    // 1/2 x 1/2 縮小を繰り返し行う

    IZ_BOOL ret = IZ_TRUE;

    for (IZ_UINT nLoopCnt = 0; nLoopCnt < MGF_LOOP_NUM; ++nLoopCnt) {
        if (m_DownScaleState[nLoopCnt].nSrcTexIdx < 0) {
            continue;
        }

        IZ_POSTEFFECT_HANDLE nSrcTexHandle = pShader->GetTexHandleByIdx(
                                                m_DownScaleState[nLoopCnt].nSrcTexIdx);
        IZ_POSTEFFECT_HANDLE nDstTexHandle = pShader->GetTexHandleByIdx(
                                                m_DownScaleState[nLoopCnt].nDstTexIdx);

        pShader->SetTexture(
            nSrcTexHandle,
            m_DownScaleState[nLoopCnt].pSrc);

        pShader->SetTexture(
            nDstTexHandle,
            m_DownScaleState[nLoopCnt].pDst);

#ifdef __IZ_DEBUG__
        graph::CGraphPerf::BeginEvent(0, "passMGFDownScale_%d", nLoopCnt);
#endif  // #ifdef __IZ_DEBUG__

        ret = ApplyCore(
                pDevice,
                pShader);

        VRETURN(ret);

#ifdef __IZ_DEBUG__
        graph::CGraphPerf::EndEvent();
#endif  // #ifdef __IZ_DEBUG__
    }

    return ret;
}   

// ブルーム作成
IZ_BOOL CPostEffectFunctorMGF::ApplyMGFRenderBloom(
    graph::CGraphicsDevice* pDevice,
    CPostEffectShader* pShader)
{
    IZ_BOOL ret = IZ_TRUE;

    // パスインデックスを覚えておく
    IZ_UINT nPassIdxTmp = m_nPassIdx;

    for (IZ_UINT nLoopCnt = 0; nLoopCnt < MGF_LOOP_NUM; ++nLoopCnt) {
        for (IZ_UINT nState = 0; nState < MGF_BLOOM_STATE_NUM; ++nState) {
            if (m_BloomState[nLoopCnt][nState].nSrcTexIdx < 0) {
                continue;
            }

            IZ_POSTEFFECT_HANDLE nSrcTexHandle = pShader->GetTexHandleByIdx(
                                                    m_BloomState[nLoopCnt][nState].nSrcTexIdx);
            IZ_POSTEFFECT_HANDLE nDstTexHandle = pShader->GetTexHandleByIdx(
                                                    m_BloomState[nLoopCnt][nState].nDstTexIdx);

            pShader->SetTexture(
                nSrcTexHandle,
                m_BloomState[nLoopCnt][nState].pSrc);

            pShader->SetTexture(
                nDstTexHandle,
                m_BloomState[nLoopCnt][nState].pDst);

            // パスインデックスを切り替える
            m_nPassIdx = GetConnectPassIdx(nState);

#ifdef __IZ_DEBUG__
        graph::CGraphPerf::BeginEvent(0, "passMGFBloom_%d_%d", nLoopCnt, nState);
#endif  // #ifdef __IZ_DEBUG__

            ret = ApplyCore(
                    pDevice,
                    pShader);
            
            VRETURN(ret);

#ifdef __IZ_DEBUG__
        graph::CGraphPerf::EndEvent();
#endif  // #ifdef __IZ_DEBUG__
        }
    }

    // パスインデックスを元に戻す
    m_nPassIdx = nPassIdxTmp;

    return ret;
}

// マージ
IZ_BOOL CPostEffectFunctorMGF::ApplyMGFMerge(
    graph::CGraphicsDevice* pDevice,
    CPostEffectShader* pShader)
{
    IZ_BOOL ret = IZ_TRUE;

    // NOTE
    // マージはアルファブレンドで行う。
    // 各テクスチャを２Ｄ描画することで行う

    IZ_UINT nPassIdx = GetConnectPassIdx(MGF_BLOOM_STATE_FIN);

    IZ_UINT nDstTexIdx = pShader->GetDstTexIdx(
                            m_nTechIdx,
                            nPassIdx);

    // 出力先
    graph::CTexture* pFinalDst = pShader->GetTexture(nDstTexIdx);

    // レンダーターゲット切り替え
    // 手動で行う・・・

    IZ_ASSERT(pFinalDst->IsRenderTarget());
    graph::CRenderTarget* rt = pFinalDst->AsRenderTarget();
    
    ret = (rt != IZ_NULL);
    IZ_ASSERT(ret);

    if (ret)
    {
        ret = pDevice->BeginScene(
                &rt, 1,
                graph::E_GRAPH_CLEAR_FLAG_COLOR,
                0);
        IZ_ASSERT(ret);
    }

    // テクスチャ座標
    SFloatRect rcTex;
    {
        rcTex.left = 0.0f;
        rcTex.top = 0.0f;
        rcTex.right = 1.0f;
        rcTex.bottom = 1.0f;
    }

    // 出力位置
    SIntRect rcDst;
    {
        rcDst.left = 0;
        rcDst.top = 0;
        rcDst.right = pFinalDst->GetWidth();
        rcDst.bottom = pFinalDst->GetHeight();
    }

    // カラー
    IZ_COLOR nColor = IZ_COLOR_RGBA(255, 255, 255, 255);

    if (ret) {
        // 2D描画
        ret = pDevice->Begin2D();
        if (ret) {
            // レンダーステート設定
            {
                // 加算アルファブレンドする
                pDevice->SetRenderState(
                    graph::E_GRAPH_RS_ALPHABLENDENABLE,
                    IZ_TRUE);
                pDevice->SetRenderState(
                    graph::E_GRAPH_RS_BLENDMETHOD,
                    graph::E_GRAPH_ALPHA_BLEND_Cs_Cd);

                // アルファテスト無し
                pDevice->SetRenderState(
                    graph::E_GRAPH_RS_ALPHATESTENABLE,
                    IZ_FALSE);
            }

            // TODO
            // TEXOP_RGBA_MODULATE);

            // 描画
            for (IZ_UINT i = 0; i < MGF_LOOP_NUM; ++i) {
                graph::CTexture* tex = m_BloomState[i][MGF_BLOOM_STATE_FIN].pDst;

                if (tex != IZ_NULL) {
                    pDevice->SetTexture(0, tex);

                    pDevice->Draw2DSprite(
                        rcTex, rcDst,
                        nColor);
                }
            }

            pDevice->End2D();
        }

        // 戻す
        pDevice->EndScene(graph::E_GRAPH_END_SCENE_FLAG_RT_0);
    }

    return ret;
}
