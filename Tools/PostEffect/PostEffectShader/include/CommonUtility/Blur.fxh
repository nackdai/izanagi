#if !defined(__IZANAGI_POSTEFFECT_BLUR_FXH__)
#define __IZANAGI_POSTEFFECT_BLUR_FXH__

/***********************************************
   ガウスブラー
***********************************************/

#include "../PostEffectShaderIO.fxh"
#include "../PostEffectSamplingParam.fxh"
#include "../ShaderDefs/ShaderDefsInternal.fxh"

// ブラー計算（偶数サンプリング）
#define _COMPUTE_EVEN_SAMPLING_BLUR_COLOR(vRet, smplIn, vUV, vWeight, NUM)\
    for (int i = 0; i < NUM; i++) {\
        vRet += vWeight[i * 2] * tex2D(smplIn, vUV[i].xy);\
        vRet += vWeight[i * 2 + 1] * tex2D(smplIn, vUV[i].zw);\
    }

// ブラー計算（奇数サンプリング）
#define _COMPUTE_ODD_SAMPLING_BLUR_COLOR(vRet, smplIn, vUV, vUV_0, vWeight, NUM)\
    {\
        for (int i = 0; i < NUM; i++) {\
            vRet += vWeight[i * 2] * tex2D(smplIn, vUV[i].xy);\
            vRet += vWeight[i * 2 + 1] * tex2D(smplIn, vUV[i].zw);\
        }\
        vRet += vWeight[NUM * 2] * tex2D(smplIn, vUV_0.xy);\
    }

////////////////////////////////////////////////////////////
// ガウスブラー5x5

// 中心に近い１３個のテクセルをサンプリングする

float4 GaussBlur5x5(
    in sampler smplIn,
    S_PE_PS_IN_SAMPLING_13 sIn)
{
    // NOTE
    //         +---+
    //         | x |
    //     +---+---+---+
    //     | x | x | x |
    // +---+---+---+---+---+
    // | x | x | @ | x | x |
    // +---+---+---+---+---+
    //     | x | x | x |
    //     +---+---+---+
    //         | x |
    //         +---+

    // ウエイト値によって、重みの合計が１になるようになっている

    float4 vRet = (float4)0.0f;

    _COMPUTE_ODD_SAMPLING_BLUR_COLOR(
        vRet,
        smplIn,
        sIn.vUV, sIn.vUV_0,
        g_vGaussBlur5x5Weight,
        NUM_SAMPLING_13);

    return vRet;
}

////////////////////////////////////////////////////////////
// １方向ガウスブラー

// リニアフィルタ
float4 GaussBlur1Direction_Linear(
    in sampler smplIn,
    S_PE_PS_IN_SAMPLING_7 sIn)
{
    // NOTE
    // +---+---+---+---+---+---+---+---+---+---+---+---+---+
    // | x | x | x | x | x | x | @ | x | x | x | x | x | x |
    // +---+---+---+---+---+---+---+---+---+---+---+---+---+
    // |<----->|<----->|<----->|   |<----->|<----->|<----->|
    // 上のように２テクセルずつまとめる

    float4 vRet = (float4)0.0f;

    _COMPUTE_ODD_SAMPLING_BLUR_COLOR(
        vRet,
        smplIn,
        sIn.vUV, sIn.vUV_0,
        g_vBloomBlurWeight_Linear,
        NUM_SAMPLING_7);

    return vRet;
}

// ポイントフィルタ
float4 GaussBlur1Direction_Point(
    in sampler smplIn,
    S_PE_PS_IN_SAMPLING_13 sIn)
{
    // NOTE
    // +---+---+---+---+---+---+---+---+---+---+---+---+---+
    // | x | x | x | x | x | x | @ | x | x | x | x | x | x |
    // +---+---+---+---+---+---+---+---+---+---+---+---+---+

    float4 vRet = (float4)0.0f;

    _COMPUTE_ODD_SAMPLING_BLUR_COLOR(
        vRet,
        smplIn,
        sIn.vUV, sIn.vUV_0,
        g_vBloomBlurWeight_Point,
        NUM_SAMPLING_13);

    return vRet;
}

// ブルーム作成用ブラー
#define BloomBlur_Linear    GaussBlur1Direction_Linear
#define BloomBlur_Point     GaussBlur1Direction_Point

////////////////////////////////////////////////////////////
// スターブラー

// スターブラーウエイト値
PRIVATE(float4 g_vStarWeight[8] : BLUR_WEIGHT);

// スターマージウエイト値
PRIVATE(float g_fStarMergeWeight);

float4 StarBlur(
  in sampler smplIn,
  S_PE_PS_IN_SAMPLING_8 sIn)
{
    float4 vRet = (float4)0.0f;

    for (int i = 0; i < NUM_SAMPLING_8; i++) {
        vRet += g_vStarWeight[i * 2 + 0] * tex2D(smplIn, sIn.vUV[i].xy);
        vRet += g_vStarWeight[i * 2 + 1] * tex2D(smplIn, sIn.vUV[i].zw);
    }

    vRet *= g_fStarMergeWeight;

    return vRet;
}

#endif  // #if !defined(__IZANAGI_POSTEFFECT_BLUR_FXH__)
