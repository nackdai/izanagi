#include "../include/Shader.fxh"

// Phong Shader

struct SVSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : NORMAL;
};

struct SVSOutput {
    float4 vPos     : POSITION;
    float3 vNormal  : TEXCOORD0;    // 法線
    float3 vHalf    : TEXCOORD1;    // ハーフベクトル
    float4 vColor   : COLOR;        // Ambient
};

/////////////////////////////////////////////////////////////
// 頂点シェーダ

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;
    
    Out.vPos = ApplyL2W(In.vPos);
    
    // Halfベクトル計算
    Out.vHalf = ComputeHalfVectorEx(0, Out.vPos);
    
    Out.vPos = ApplyW2C(Out.vPos);
    
    Out.vNormal = ApplyL2W(In.vNormal);
    Out.vNormal = normalize(Out.vNormal);

    // Ambient
    Out.vColor = ComputeAmbientLight();
    
    Out.vColor.a = 1.0f;
    
    return Out;
}

/////////////////////////////////////////////////////////////
// ピクセルシェーダ

float4 mainPS(SVSOutput In) : COLOR
{
    // 頂点シェーダでAmbientについては計算済み
    float4 vOut = In.vColor;
    
    // いるのか・・・
    float3 vN = normalize(In.vNormal);
    float3 vH = normalize(In.vHalf);
        
    vOut.rgb += ComputePhongLight(0, vN, vH);

    return vOut;
}

/////////////////////////////////////////////////////////////

technique PhongShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader = compile ps_2_0 mainPS();
    }
}
