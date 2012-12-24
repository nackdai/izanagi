// ポストエフェクト　ＨＤＲ

#include "PostEffectShader.fxh"

PES_ID("POSTEFFECT_HDR2");

////////////////////////////////////////////////////
// テクスチャ

#define BASE_TEX_FMT    PIXEL_FORMAT_RGBA16F
//#define BASE_TEX_FMT    PIXEL_FORMAT_RGBA8

#define LUM_TEX_FMT        PIXEL_FORAMT_R32F

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

// 輝度測定バッファ
texture texMeasureLumInit
<
    SetTexFormat(LUM_TEX_FMT);
    SetTexSize(64, 64);
>;
texture texMeasureLumIterate_0
<
    SetTexFormat(LUM_TEX_FMT);
    SetTexSize(16, 16);
>;
texture texMeasureLumIterate_1
<
    SetTexFormat(LUM_TEX_FMT);
    SetTexSize(4, 4);
>;
texture texToneMap
<
    SetTexFormat(LUM_TEX_FMT);
    SetTexSize(1, 1);
>;

// 明順応
texture texAdaptedLumCur : PRIVATE
<
    SetTexFormat(LUM_TEX_FMT);
    SetTexSize(1, 1);
>;
texture texAdaptedLumLast : PRIVATE
<
    SetTexFormat(LUM_TEX_FMT);
    SetTexSize(1, 1);
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

// 輝度測定バッファ
sampler smplMeasureLumInit = sampler_state
{
    BindTex(texMeasureLumInit);
    SAMPLER_FILTER();
};
sampler smplMeasureLumIterate_0 = sampler_state
{
    BindTex(texMeasureLumIterate_0);
    SAMPLER_FILTER();
};
sampler smplMeasureLumIterate_1 = sampler_state
{
    BindTex(texMeasureLumIterate_1);
    SAMPLER_FILTER();
};
sampler smplToneMap = sampler_state
{
    BindTex(texToneMap);
    SAMPLER_FILTER();
};

// 明順応
sampler smplAdaptedLumCur = sampler_state
{
    BindTex(texAdaptedLumCur);
    SAMPLER_FILTER();
};
sampler smplAdaptedLumLast = sampler_state
{
    BindTex(texAdaptedLumLast);
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
// 輝度測定

#ifdef __ENABLE_LINEAR_SAMPLE__

// 最初
float4 mainMeasureLuminanceInitial(S_PE_PS_IN_SAMPLING_4 sIn) : COLOR
{
    float4 vRet = ReinherdMeasureLuminanceInitial_Linear(smplDownScale4x4, sIn);
    return vRet;
}

// 途中
float4 mainMeasureLuminanceIterate_0(S_PE_PS_IN_SAMPLING_4 sIn) : COLOR
{
    float4 vRet = ReinherdMeasureLuminanceIterative_Linear(smplMeasureLumInit, sIn);
    return vRet;
}

// 途中
float4 mainMeasureLuminanceIterate_1(S_PE_PS_IN_SAMPLING_4 sIn) : COLOR
{
    float4 vRet = ReinherdMeasureLuminanceIterative_Linear(smplMeasureLumIterate_0, sIn);
    return vRet;
}

// 最後
float4 mainMeasureLuminanceFinal(S_PE_PS_IN_SAMPLING_4 sIn) : COLOR
{
    float4 vRet = ReinherdMeasureLuminanceFinal_Linear(smplMeasureLumIterate_1, sIn);
    return vRet;
}

#else    // #ifdef __ENABLE_LINEAR_SAMPLE__

// 最初
float4 mainMeasureLuminanceInitial(S_PE_PS_IN_SAMPLING_9 sIn) : COLOR
{
    float4 vRet = ReinherdMeasureLuminanceInitial_Point(smplDownScale4x4, sIn);
    return vRet;
}

// 途中
float4 mainMeasureLuminanceIterate_0(S_PE_PS_IN_SAMPLING_16 sIn) : COLOR
{
    float4 vRet = ReinherdMeasureLuminanceIterative_Point(smplMeasureLumInit, sIn);
    return vRet;
}

// 途中
float4 mainMeasureLuminanceIterate_1(S_PE_PS_IN_SAMPLING_16 sIn) : COLOR
{
    float4 vRet = ReinherdMeasureLuminanceIterative_Point(smplMeasureLumIterate_0, sIn);
    return vRet;
}

// 最後
float4 mainMeasureLuminanceFinal(S_PE_PS_IN_SAMPLING_16 sIn) : COLOR
{
    float4 vRet = ReinherdMeasureLuminanceFinal_Point(smplMeasureLumIterate_1, sIn);
    return vRet;
}

#endif    // #ifdef __ENABLE_LINEAR_SAMPLE__

////////////////////////////////////////////////////
// 明順応

float fInitAdaptedColor = 0.5f;
float fAdaptedRate = 0.02f;

// 最初の１回だけクリア
float4 mainAdaptedInit(S_PE_PS_IN sIn) : COLOR
{
    return float4(fInitAdaptedColor, fInitAdaptedColor, fInitAdaptedColor, 1.0f);
}

// 明順応
float4 mainAdaptedLum(S_PE_PS_IN sIn) : COLOR
{
    // NOTE
    // 1x1 テクスチャの中心をサンプリング
    float fAdaptedLum = tex2D(smplAdaptedLumCur, float2(0.5f, 0.5f)).r;
    float fCurrentLum = tex2D(smplToneMap, float2(0.5f, 0.5f)).r;

    // 順応計算
    float4 vRet = ComputeAdaptation(
                    fAdaptedLum,
                    fCurrentLum,
                    fAdaptedRate);
    return vRet;
}

////////////////////////////////////////////////////
// 輝度抽出

float fMiddleGrey = 0.18f;
float fThreshold = 2.0f;

float4 mainBrighPass(S_PE_PS_IN sIn) : COLOR
{
    float4 vSample = tex2D(smplDownScale4x4, sIn.vUV);
    float fAdaptedLum = tex2D(smplAdaptedLumLast, float2(0.5f, 0.5f)).r;

    vSample = ExtractLum(
                vSample, 
                fMiddleGrey,
                fAdaptedLum,
                fThreshold);

    return vSample;
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

float fBloomWeight = 0.25f;
float fStarWeight = 0.4f;

float4 _FinalPass(
    in S_PE_PS_IN sIn,
    bool bEnableBloom,
    bool bEnableStar,
    bool bEnableToneMap)
{
    float4 vRet = tex2D(smplScene, sIn.vUV);

    if (bEnableToneMap) {
        float fAdaptedLum = tex2D(smplAdaptedLumLast, float2(0.5f, 0.5f)).r;

        // トーンマップ計算
        vRet = ComputeTonemap(vRet, fMiddleGrey, fAdaptedLum);
    }

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
    float4 vRet = _FinalPass(sIn, true, true, true);
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

    ////////////////////////////////////////////////////
    // 輝度測定

    // 最初
    pass passMeasureLuminanceInit
    <
        BeginScene(texMeasureLumInit);
#ifdef __ENABLE_LINEAR_SAMPLE__
        SetTextureOffset(g_vDownScale3x3SampleOffset_Linear);
#else
        SetTextureOffset(g_vDownScale3x3SampleOffset_Point);
#endif
    >
    {
#ifdef __ENABLE_LINEAR_SAMPLE__
        SetVertexShader(vs_2_a, VSSampling_4);
#else
        SetVertexShader(vs_2_a, VSSampling_9);
#endif
        SetPixelShader(ps_2_a, mainMeasureLuminanceInitial);
    }

    // 途中
    pass passMeasureLuminanceIterate_0
    <
        BeginScene(texMeasureLumIterate_0);
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
        SetPixelShader(ps_2_a, mainMeasureLuminanceIterate_0);
    }

    // 途中
    pass passMeasureLuminanceIterate_1
    <
        BeginScene(texMeasureLumIterate_1);
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
        SetPixelShader(ps_2_a, mainMeasureLuminanceIterate_1);
    }

    // 最後
    pass passMeasureLuminanceFinal
    <
        BeginScene(texToneMap);
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
        SetPixelShader(ps_2_a, mainMeasureLuminanceFinal);
    }

    ////////////////////////////////////////////////////

    ////////////////////////////////////////////////////
    // 明順応

    // 最初の１回だけクリア
    pass passAdaptedInit
    <
        BeginScene(texAdaptedLumCur);

        // 一度しか呼ばれない
        SetFunctorName("PostEffectFunctorOnce");
    >
    {
        SetVertexShader(vs_2_a, VSSampling_1);
        SetPixelShader(ps_2_a, mainAdaptedInit);
    }

    // 明順応
    pass passAdaptedLum
    <
        BeginScene(texAdaptedLumLast);

        // テクスチャ切り替え
        SetFunctorName("PostEffectFunctorSwitchTex");
        SetFunctorArgsS_2(
            texAdaptedLumCur,
            texAdaptedLumLast);
    >
    {
        SetVertexShader(vs_2_a, VSSampling_1);
        SetPixelShader(ps_2_a, mainAdaptedLum);
    }

    ////////////////////////////////////////////////////

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

    ////////////////////////////////////////////////////
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

    ////////////////////////////////////////////////////

    // スター作成
    pass passRenderStar
    <
        BeginSceneWithClearColor(texStar, CLEAR_COLOR);

        // スター作成ファンクタ
        SetFunctorName("PostEffectFunctorRenderStar");
    >
    {
        // スター作成ファンクタ内で強制的に加算ブレンドが設定されている。
        // そのため、ここでアルファブレンドの設定をしても無視される。

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
