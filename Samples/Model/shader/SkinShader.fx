// Skin Shader

struct SVSInput {
    float4 vPos        : POSITION;
    float3 vNormal    : NORMAL;
    float2 vUV        : TEXCOORD0;

    float4 vIndices    : BLENDINDICES;
    float4 vWeight    : BLENDWEIGHT;
};

struct SPSInput {
    float4 vPos        : POSITION;
    float3 vNormal    : TEXCOORD0;    // 法線
    float2 vUV        : TEXCOORD2;
};

#define SVSOutput        SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mW2C;

texture tex;

sampler sTex = sampler_state
{
    Texture = tex;
};

/////////////////////////////////////////////////////////////
// 頂点シェーダ

#define JOINT_MTX_NUM    (48)
float4x4 vJointMatrix[JOINT_MTX_NUM];

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

#if 0
    for (int i = 0; i < 4; i++) {
        float idx = In.vIndices[i];
        float weight = In.vWeight[i];

        float4x4 mtx = vJointMatrix[idx];

        Out.vPos += mul(In.vPos, mtx) * weight;
        Out.vNormal += mul(In.vNormal, (float3x3)mtx) * weight;
    }
        
    Out.vPos = mul(Out.vPos, g_mW2C);
#else
    Out.vPos = mul(In.vPos, g_mW2C);
#endif

    Out.vUV = In.vUV;
    
    return Out;
}

SVSOutput mainVSWithReversingUV(SVSInput In)
{
   SVSOutput Out = mainVS(In);

   Out.vUV.y = 1.0f - Out.vUV.y;

   return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
    float4 vOut = tex2D(sTex, In.vUV);

    return vOut;
}

/////////////////////////////////////////////////////////////

technique t0
{
    pass p0
    {
        VertexShader = compile vs_2_0 mainVSWithReversingUV();
        PixelShader = compile ps_2_0 mainPS();
    }
    pass p1
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader = compile ps_2_0 mainPS();
    }
}
