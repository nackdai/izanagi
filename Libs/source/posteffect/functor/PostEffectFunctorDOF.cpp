#if 0
#include "posteffect/internal/PostEffectFunctorDOF.h"
#include "posteffect/PostEffect.h"
#include "posteffect/PostEffectFunctor.h"
#include "posteffect/PostEffectParamContainer.h"

using namespace izanagi;

// ファンクタ独自のテクスチャ作成
IZ_BOOL CPostEffectFunctorDOFMerge::CreateTexture(
    CPostEffect* pPostEffect,
    CPostEffectShader* pShader,
    IPostEffectTextureCreator* pTexCreator,
    CGraphicsDevice* pDevice)
{
    UNUSED_ALWAYS(pShader);
    UNUSED_ALWAYS(pTexCreator);

    IZ_BOOL ret = IZ_FALSE;
    E_GRAPH_PIXEL_FMT fmt;

    // 作成するテクスチャのデータ
    S_POSTEFFECT_TEX_ANN sTexAnn;
    ret = pPostEffect->GetTexAnnByName(
            "texWeight",
            &sTexAnn);

    // ウエイトテーブルテクスチャが存在しない場合もある・・・
    if (!ret) {
        return IZ_TRUE;
    }

    fmt = sTexAnn.Fmt;

    // NOTE
    // ロックして直接データを変更するので、リニアフォーマットでないといけない

    IZ_ASSERT(
        (fmt == E_GRAPH_PIXEL_FMT_RGBA8)
        || (fmt == E_GRAPH_PIXEL_FMT_BGRA8)
        || (fmt == E_GRAPH_PIXEL_FMT_A8));

    // グラフィックスデバイスから直接作成する
    for (IZ_UINT i = 0; i < COUNTOF(m_pWeightTex); ++i) {
        m_pWeightTex[i] = pDevice->CreateTexture(
                            sTexAnn.Dimensions[0],
                            sTexAnn.Dimensions[1],
                            1,
                            fmt,
                            E_GRAPH_RSC_TYPE_DYNAMIC);

        ret = (m_pWeightTex[i] != IZ_NULL);
        if (!ret) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }
    }

__EXIT__:
    return ret;
}

// 処理
IZ_BOOL CPostEffectFunctorDOFMerge::Apply(
    CPostEffectParamContainer* pParamContainer,
    CGraphicsDevice* pDevice,
    CPostEffectShader* pShader)
{
    IZ_BOOL ret = IZ_TRUE;

    // 前処理
    ret = BeginApplyMerge(
            pParamContainer,
            pDevice,
            pShader);
    if (!ret) {
        goto __EXIT__;
    }

    // メイン処理
    ret = ApplyMerge(
            pParamContainer,
            pDevice,
            pShader);
    if (!ret) {
        goto __EXIT__;
    }

    // 後処理
    EndApplyMerge(
        pParamContainer,
        pDevice,
        pShader);

__EXIT__:
    return ret;
}

// マージ処理
IZ_BOOL CPostEffectFunctorDOFMerge::ApplyMerge(
    CPostEffectParamContainer* pParamContainer,
    CGraphicsDevice* pDevice,
    CPostEffectShader* pShader)
{
    IZ_BOOL ret = pShader->BeginRender(
                    this,
                    GetTechIdx(),
                    GetPassIdx(),
                    GetTextureList()->GetTexBySemantic("INPUT_SCENE"),
                    IZ_FALSE);

    if (ret) {
        ret = SetTextureOffset(
                pParamContainer,
                pDevice,
                pShader);
        if (!ret) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // パラメータセット
        pShader->SetParameter(pParamContainer);

        // テクスチャセット
        ret = pShader->SetTexture(m_pTexList);
        if (!ret) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }
        
        // コミットチェンジ
        ret = pShader->CommitChanges();
        if (!ret) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // 描画
        ret = pShader->Render(IZ_NULL, IZ_NULL);
        if (!ret) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }

        // 描画終了
        ret = pShader->EndRender();
        if (!ret) {
            IZ_ASSERT(IZ_FALSE);
            goto __EXIT__;
        }
    }

__EXIT__:
    return ret;
}

namespace {
    // パラメータ取得
    inline IZ_BOOL _GetParamFromParamContainerBySemantic(
        CPostEffectParamContainer* pParamContainer,
        IZ_PCSTR pszSemantic,
        IZ_FLOAT* pRet)
    {
        E_POSTEFFECT_PARAMETER_TYPE type;
        const void* data = pParamContainer->GetParameterBySemantic(
                            pszSemantic,
                            IZ_NULL, &type, IZ_NULL);

        VRETURN((type == E_POSTEFFECT_PARAMETER_TYPE_FLOAT) && (data != IZ_NULL));

        *pRet = *(IZ_FLOAT*)data;

        return IZ_TRUE;
    }

    const IZ_FLOAT DELTA = 0.00001f;

    // 深度値[0- 1]を逆変換する
    inline IZ_FLOAT _ComputeInvZ(
        IZ_FLOAT fZ,
        IZ_FLOAT fNear,
        IZ_FLOAT fFar)
    {
        // NOTE
        //           FN                   Z - N
        // Z = --------------- <=> Z' = (------- × F) / Z
        //      F - Z'(F - N)             F - N

        IZ_FLOAT fQ = fFar - fZ * (fFar - fNear);

        IZ_FLOAT ret = fFar * fNear / (fQ + DELTA);
        return ret;
    }
}   // namespace

// マージ処理開始前処理
IZ_BOOL CPostEffectFunctorDOFMerge::BeginApplyMerge(
    CPostEffectParamContainer* pParamContainer,
    CGraphicsDevice* pDevice,
    CPostEffectShader* pShader)
{
    UNUSED_ALWAYS(pShader);

    if (m_pWeightTex[m_nWeightTexPos] != IZ_NULL) {
        // ウエイトテーブルテクスチャが存在するときのみ

        IZ_FLOAT fL = 0.0f;     // ピントの合う距離
        IZ_FLOAT fFov = 0.0f;       // カメラ画角
        IZ_FLOAT fAperture = 0.0f;  // カメラ絞り値
        IZ_FLOAT fNear = 0.0f;      // カメラNear
        IZ_FLOAT fFar = 0.0f;       // カメラFar

        struct {
            IZ_FLOAT* pParam;
            IZ_PCSTR pszSemantic;
        } ParamList[] = {
            {&fL,        "CAMERA_PARAM_EYE_TO_AT_LENGTH"},
            {&fFov,      "CAMERA_PARAM_FOV"},
            {&fAperture, "CAMERA_PARAM_APERTURE"},
            {&fNear,     "CAMERA_PARAM_NEAR"},
            {&fFar,      "CAMERA_PARAM_FAR"},
        };
        
        // 各種パラメータ取得
        for (IZ_UINT i = 0; i < COUNTOF(ParamList); ++i) {
            VRETURN(
                _GetParamFromParamContainerBySemantic(
                    pParamContainer,
                    ParamList[i].pszSemantic,
                    ParamList[i].pParam));
        }

        IZ_FLOAT fTan = tanf(fFov * 0.5f);

        // スクリーン高さ
        IZ_INT fH = pDevice->GetPresentParam().BackBufferHeight;

        // サイズは同じなので、０or１のどっちでもいい
        IZ_INT nTableStep = m_pWeightTex[0]->GetWidth();

        IZ_FLOAT fStep = 1.0f / (nTableStep - 1);

        void* pvoid = IZ_NULL;
        m_pWeightTex[m_nWeightTexPos]->Lock(
            0,
            &pvoid,
            IZ_FALSE);

        IZ_UINT8* data = static_cast<IZ_UINT8*>(pvoid);
        IZ_ASSERT(data != IZ_NULL);

        IZ_UINT nBytePerPixel = (m_pWeightTex[m_nWeightTexPos]->GetPixelFormat() == E_GRAPH_PIXEL_FMT_A8
                                    ? 1
                                    : 4);

        for (IZ_INT i = 0; i < nTableStep; ++i) {
            // Z値[0- 1]を逆変換する
            IZ_FLOAT fZ = _ComputeInvZ(
                            fStep * i,
                            fNear, fFar);

            // 0割り対策
            fZ += DELTA;

            // NOTE
            // δ= (D * H) / (2 * L * tan(θ/2) * abs(1 - L/Z)
            // CoC = Circle of Confusion (錯乱円)
            IZ_FLOAT fCoC = (fAperture * fH) / (2.0f * fL * fTan) * abs(1.0f - fL / fZ);
            fCoC += DELTA;

            IZ_UINT8 nWeight = (IZ_UINT8)CMath::Clamp((1.0f / fCoC) * 256.0f, 0.0f, 255.0f);

            for (IZ_UINT n = 0; n < nBytePerPixel; ++n) {
                *(data++) = nWeight;
            }
        }

        m_pWeightTex[m_nWeightTexPos]->Unlock(0);

        // パラメータコンテナにテクスチャをセット
        IZ_BOOL result = pParamContainer->SetTextureByName(
                            "texWeight",
                            m_pWeightTex[m_nWeightTexPos]);
        IZ_ASSERT(result);
    }

    // カラーバッファ描き込み変更
    {
        // 元の状態を保持
        m_bColorWriteEnable[0] = pDevice->GetRenderState().isEnableRenderRGB;
        m_bColorWriteEnable[1] = pDevice->GetRenderState().isEnableRenderA;

        // アルファのみに描き込み
        CGraphicsDeviceUtil::EnableRenderColorRGB(pDevice, IZ_FALSE);
        CGraphicsDeviceUtil::EnableRenderColorA(pDevice, IZ_TRUE);
    }

    return IZ_TRUE;
}
#endif
