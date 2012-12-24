// ポストエフェクト　BloomStar

#include "PostEffectShader.fxh"

PES_ID("POSTEFFECT_BloomStar");

////////////////////////////////////////////////////
// テクスチャ

#define BASE_TEX_FMT    PIXEL_FORMAT_RGBA16F
//#define BASE_TEX_FMT    PIXEL_FORMAT_RGBA8

// シーンテクスチャ
texture texScene : INPUT_SCENE
<
    SetTexFormat(BASE_TEX_FMT);
>;

// 1/4 x 1/4 縮小バッファ
texture texDownScale4x4
<
    SetTexFormat(BASE_TEX_FMT);

    SetTexRatio(0.25f, 0.25f);    // 1/4
>;

// 輝度抽出バッファ
texture texBright
<
    SetTexFormat(BASE_TEX_FMT);

    SetTexRatio(0.25f, 0.25f);    // 1/4
    SetTexSize(2, 2);
>;

// スター元
texture texStarSource
<
    SetTexFormat(PIXEL_FORMAT_RGBA8);

    SetTexRatio(0.25f, 0.25f);    // 1/4
    SetTexSize(2, 2);
>;

// ブルーム元
texture texBloomSource
<
    SetTexFormat(PIXEL_FORMAT_RGBA8);

    SetTexRatio(0.125f, 0.125f);    // 1/8
    SetTexSize(2, 2);
>;

// ブルーム途中
texture texBloomIterate
<
    SetTexFormat(PIXEL_FORMAT_RGBA8);

    SetTexRatio(0.125f, 0.125f);    // 1/8
    SetTexSize(2, 2);
>;

// ブルーム
texture texBloom
<
    SetTexFormat(PIXEL_FORMAT_RGBA8);

    SetTexRatio(0.125f, 0.125f);    // 1/8
>;

// スター
texture texStar
<
    SetTexFormat(PIXEL_FORMAT_RGBA8);

    SetTexRatio(0.25f, 0.25f);    // 1/4
    SetTexSize(2, 2);
>;

////////////////////////////////////////////////////
// サンプラ

#ifdef __ENABLE_LINEAR_SAMPLE__
#define SAMPLER_FILTER()\
    SetMinFilter(SMPL_FILTER_LINEAR);\
    SetMagFilter(SMPL_FILTER_LINEAR)
#else    // #ifdef __ENABLE_LINEAR_SAMPLE__
#define SAMPLER_FILTER()\
    SetMinFilter(SMPL_FILTER_POINT);\
    SetMagFilter(SMPL_FILTER_POINT)
#endif    // #ifdef __ENABLE_LINEAR_SAMPLE__

// シーンテクスチャ
sampler smplScene = sampler_state
{
    BindTex(texScene);
    SAMPLER_FILTER();
};

// 1/4 x 1/4 縮小バッファ
sampler smplDownScale4x4 = sampler_state
{
    BindTex(texDownScale4x4);
    SAMPLER_FILTER();
};

// 輝度抽出バッファ
sampler smplBright = sampler_state
{
    BindTex(texBright);
    SAMPLER_FILTER();
};

// スター元
sampler smplStarSource = sampler_state
{
    BindTex(texStarSource);
};

// ブルーム元
sampler smplBloomSource = sampler_state
{
    BindTex(texBloomSource);
};

// ブルーム途中
sampler smplBloomIterate = sampler_state
{
    BindTex(texBloomIterate);
};

// ブルーム最後
sampler smplBloomFinal = sampler_state
{
    BindTex(texBloomSource);
};

// ブルーム
sampler smplBloom = sampler_state
{
    BindTex(texBloom);
};

// スター
sampler smplStar = sampler_state
{
    BindTex(texStar);
};

////////////////////////////////////////////////////
// 1/4 x 1/4 縮小バッファ作成

#ifdef __ENABLE_LINEAR_SAMPLE__
float4 mainDownScale4x4(S_PE_PS_IN_DOWNSCALE_4x4_LINEAR sIn) : COLOR
{
    float4 vRet = DownScale4x4_Linear(smplScene, sIn);
    return vRet;
}
#else    // #ifdef __ENABLE_LINEAR_SAMPLE__
float4 mainDownScale4x4(S_PE_PS_IN_DOWNSCALE_4x4_POINT sIn) : COLOR
{
    float4 vRet = DownScale4x4_Point(smplScene, sIn);
    return vRet;
}
#endif    // #ifdef __ENABLE_LINEAR_SAMPLE__


////////////////////////////////////////////////////
// 輝度抽出

float fBrightThreshold = 0.5f;
float fBrightScale = 1.0f;

// 超簡易
float4 mainBrighPass(S_PE_PS_IN sIn) : COLOR
{
    float4 vRet = tex2D(smplDownScale4x4, sIn.vUV);

    // 0未満にならないようにする
    vRet.rgb = max(vRet.rgb - fBrightThreshold, 0.0f) * fBrightScale;

    return vRet;
}

////////////////////////////////////////////////////
// 縮小バッファへコピーしたことによるエイリアシングを防ぐために
// ガウス型のぼかしを入れる

float4 mainGaussBlur(S_PE_PS_IN_GAUSSBLUR_5x5 sIn) : COLOR
{
    float4 vRet = GaussBlur5x5(smplBright, sIn);
    return vRet;
}

////////////////////////////////////////////////////
// 1/2 x 1/2 縮小バッファ作成

float4 mainDownScale2x2(S_PE_PS_IN sIn) : COLOR
{
    float4 vRet = DownScale2x2_Linear(smplStarSource, sIn);
    return vRet;
}

////////////////////////////////////////////////////
// ブルーム作成

// 最初
float4 mainRenderBloomInit(S_PE_PS_IN_GAUSSBLUR_5x5 sIn) : COLOR
{
    float4 vRet = GaussBlur5x5(smplBloomSource, sIn);
    return vRet;
}

// 途中
float4 mainRenderBloomIterate(S_PE_PS_IN_BLOOMBLUR_LINEAR sIn): COLOR
{
    float4 vRet = BloomBlur_Linear(smplBloomIterate, sIn);
    return vRet;
}

// 最後
float4 mainRenderBloomFinal(S_PE_PS_IN_BLOOMBLUR_LINEAR sIn): COLOR
{
    float4 vRet = BloomBlur_Linear(smplBloomFinal, sIn);
    return vRet;
}

////////////////////////////////////////////////////
// スター作成

float4 mainRenderStar(S_PE_PS_IN_STARBLUR sIn) : COLOR
{
    float4 vRet = StarBlur(smplStarSource, sIn);
    return vRet;
}

////////////////////////////////////////////////////
// 最終描画

float fBloomWeight = 0.5f;
float fStarWeight = 0.8f;

float4 _FinalPass(
    in S_PE_PS_IN sIn,
    bool bEnableBloom,
    bool bEnableStar)
{
    float4 vRet = tex2D(smplScene, sIn.vUV);

    if (bEnableBloom) {
        vRet += tex2D(smplBloom, sIn.vUV) * fBloomWeight;
    }

    if (bEnableStar) {
        vRet += tex2D(smplStar, sIn.vUV) * fStarWeight;
    }

    return vRet;
}

float4 mainFinal(S_PE_PS_IN sIn) : COLOR
{
    float4 vRet = _FinalPass(sIn, true, true);
    return vRet;
}

float4 mainFinalWithoutBloom(S_PE_PS_IN sIn) : COLOR
{
    float4 vRet = _FinalPass(sIn, false, true);
    return vRet;
}

float4 mainFinalWithoutStar(S_PE_PS_IN sIn) : COLOR
{
    float4 vRet = _FinalPass(sIn, true, false);
    return vRet;
}

////////////////////////////////////////////////////
// テクニック

// クリアカラー
#define CLEAR_COLOR        (0)

technique t0
{
    // 1/4 x 1/4 縮小バッファ作成
    pass passDownScale4x4
    <
        BeginScene(texDownScale4x4);
#ifdef __ENABLE_LINEAR_SAMPLE__
        SetTextureOffset(g_vDownScale4x4SampleOffset_Linear);
#else
        SetTextureOffset(g_vDownScale4x4SampleOffset_Point);
#endif
    >
    {
#ifdef __ENABLE_LINEAR_SAMPLE__
        SetVertexShader(vs_2_a, VSSampling_4);
#else
        SetVertexShader(vs_2_a, VSSampling_16);
#endif
        SetPixelShader(ps_2_a, mainDownScale4x4);
    }

    // 輝度抽出
    pass passBright
    <
        BeginSceneWithClearColor(texBright, CLEAR_COLOR);
        SetScissorRectInflate(-1, -1);
    >
    {
        SetVertexShader(vs_2_a, VSSampling_1);
        SetPixelShader(ps_2_a, mainBrighPass);
    }

    // ガウスブラー
    pass passGaussBlur
    <
        BeginSceneWithClearColor(texStarSource, CLEAR_COLOR);
        SetTextureOffset(g_vGaussBlur5x5SampleOffset);

        SetScissorRectInflate(-1, -1);
    >
    {
        SetVertexShader(vs_2_a, VSSampling_13);
        SetPixelShader(ps_2_a, mainGaussBlur);
    }

    // 1/2 x 1/2 縮小バッファ作成
    pass passDownScale2x2
    <
        BeginSceneWithClearColor(texBloomSource, CLEAR_COLOR);

        SetSrcTexRectInflate(-1, -1);
        SetScissorRectInflate(-1, -1);
    >
    {
        SetVertexShader(vs_2_a, VSSampling_1);
        SetPixelShader(ps_2_a, mainDownScale2x2);
    }

    //--------------------------------------------
    // ブルーム作成

    // 最初
    pass passRenderBloomInit
    <
        BeginSceneWithClearColor(texBloomIterate, CLEAR_COLOR);
        SetTextureOffset(g_vGaussBlur5x5SampleOffset);

        SetSrcTexRectInflate(-1, -1);
        SetScissorRectInflate(-1, -1);
    >
    {
        SetVertexShader(vs_2_a, VSSampling_13);
        SetPixelShader(ps_2_a, mainRenderBloomInit);
    }

    // 途中
    pass passRenderBloomIterate
    <
        BeginSceneWithClearColor(texBloomSource, CLEAR_COLOR);
        SetTextureOffset(g_vBloomBlurSampleOffsetH_Linear);    // 横ぼかし

        SetSrcTexRectInflate(-1, -1);
        SetScissorRectInflate(-1, -1);
    >
    {
        SetVertexShader(vs_2_a, VSSampling_7);
        SetPixelShader(ps_2_a, mainRenderBloomIterate);
    }

    // 最後
    pass passRenderBloomFinal
    <
        BeginScene(texBloom);
        SetTextureOffset(g_vBloomBlurSampleOffsetV_Linear);    // 縦ぼかし

        SetSrcTexRectInflate(-1, -1);
    >
    {
        SetVertexShader(vs_2_a, VSSampling_7);
        SetPixelShader(ps_2_a, mainRenderBloomFinal);
    }

    //--------------------------------------------

    // スター作成
    pass passRenderStar
    <
        BeginSceneWithClearColor(texStar, CLEAR_COLOR);

        // スター作成ファンクタ
        SetFunctorName("PostEffectFunctorRenderStar");
    >
    {
        // スター作成ファンクタ内で強制的に加算ブレンドが設定されている。
        // そのため、ここでアルファブレンドの設定しても無視される。

        SetVertexShader(vs_2_a, VSSampling_8);
        SetPixelShader(ps_2_a, mainRenderStar);
    }

    // 最終描画
    pass passFinal
    {
        SetVertexShader(vs_2_a, VSSampling_1);
        SetPixelShader(ps_2_a, mainFinal);
    }
}
