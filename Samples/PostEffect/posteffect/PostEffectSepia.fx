// ポストエフェクト　セピア

#include "PostEffectShader.fxh"

PES_ID("POSTEFFECT_Sepia");

////////////////////////////////////////////////////

// シーンテクスチャ
texture texScene : INPUT_SCENE
<
    SetTexFormat(PIXEL_FORMAT_RGBA8);
>;

// サンプラ
sampler smplScene = sampler_state
{
    BindTex(texScene);
};

float fWeight;

float fCb = -0.2f;
float fCr = 0.1f;

////////////////////////////////////////////////////

float4 mainPS_0(S_PE_PS_IN sIn) : COLOR
{
    float4 vColor = tex2D(smplScene, sIn.vUV);
    return ColorFilterSepia(vColor, fCb, fCr);
}

// 線形補間あり
float4 mainPS_1(S_PE_PS_IN sIn) : COLOR
{
    float4 vColor = tex2D(smplScene, sIn.vUV);
    return ColorFilterSepia(vColor, fCb, fCr, fWeight);
}

////////////////////////////////////////////////////
// テクニック

technique t0
{
    pass p0
    {
        SetAlphaBlendEnable(false);
        SetAlphaTestEnable(false);

        SetVertexShader(vs_2_a, VSSampling_1);
        SetPixelShader(ps_2_0, mainPS_0);
    }
}

technique t1
{
    pass p0
    {
        SetAlphaBlendEnable(false);
        SetAlphaTestEnable(false);

        SetVertexShader(vs_2_a, VSSampling_1);
        SetPixelShader(ps_2_0, mainPS_1);
    }
}
