// 2D用ピクセルシェーダ

#include "2DShader.hlsl"

sampler sTex : register(s0);

float4 main_OpVtx(SPSInput sIn) : COLOR
{
    // Dst = Vtx
    float4 vColor = sIn.vColor;
    return vColor;
}

float4 main_OpTex(SPSInput sIn) : COLOR
{
    // Dst = Tex
    float4 vColor = tex2D(sTex, sIn.vUV);
    return vColor;
}

float4 main_OpModulate(SPSInput sIn) : COLOR
{
    // Dst = Vtx x Tex
    float4 vColor = sIn.vColor * tex2D(sTex, sIn.vUV);
    return saturate(vColor);
}

float4 main_OpModulate2x(SPSInput sIn) : COLOR
{
    // Dst = (Vtx x Tex) << 1
    float4 vColor = sIn.vColor * tex2D(sTex, sIn.vUV) * 2.0f;
    return saturate(vColor);
}

float4 main_OpModulate4x(SPSInput sIn) : COLOR
{
    // Dst = (Vtx x Tex) << 2
    float4 vColor = sIn.vColor * tex2D(sTex, sIn.vUV) * 4.0f;
    return saturate(vColor);
}

float4 main_OpAdd(SPSInput sIn) : COLOR
{
    // Dst = Vtx + Tex
    float4 vColor = sIn.vColor + tex2D(sTex, sIn.vUV);
    return saturate(vColor);
}

float4 main_OpModulateAlpha(SPSInput sIn) : COLOR
{
    // Dst = Vtx x Tex.a
#if 1
    float4 vColor = sIn.vColor * float4(1.0f, 1.0f, 1.0f, tex2D(sTex, sIn.vUV).a);
#else
    float4 vColor = sIn.vColor;

    float alpha = tex2D(sTex, sIn.vUV).a;
    vColor.rgb *= alpha;
#endif
    return saturate(vColor);
}

float4 main_OpNoTexAlpha(SPSInput sIn) : COLOR
{
    // Dst = Vtx x (Tex.rgb, 1.0)
    float4 vColor = sIn.vColor;
    vColor.rgb *= tex2D(sTex, sIn.vUV).rgb;
    return saturate(vColor);
}
