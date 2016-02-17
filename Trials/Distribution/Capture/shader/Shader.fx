// Basic Shader

struct SVSInput {
    float4 vPos     : POSITION;
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD0;
    float4 L2W_0    : TEXCOORD1;
    float4 L2W_1    : TEXCOORD2;
    float4 L2W_2    : TEXCOORD3;
    float4 L2W_3    : TEXCOORD4;
};

struct SPSInput {
    float4 vPos     : POSITION;
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD0;
    float4 vDepth   : TEXCOORD1;
};

#define SVSOutput        SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;

texture tex;

sampler sTex = sampler_state
{
    Texture = tex;
};

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos.x = dot(In.vPos, In.L2W_0);
    Out.vPos.y = dot(In.vPos, In.L2W_1);
    Out.vPos.z = dot(In.vPos, In.L2W_2);
    Out.vPos.w = dot(In.vPos, In.L2W_3);

    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vUV = In.vUV;
    Out.vColor = In.vColor;
    
    return Out;
}

SVSOutput mainVS_Basic(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vUV = In.vUV;
    Out.vColor = In.vColor;

    Out.vDepth = Out.vPos.z / Out.vPos.w;

    return Out;
}

#if 0
struct SPSOutput {
    float4 color    : COLOR0;
    float4 depth    : COLOR1;
};

SPSOutput mainPS(SPSInput In)
{
    SPSOutput outClr;

    outClr.depth = In.vDepth;
    outClr.depth.a = 1.0f;

    outClr.color = tex2D(sTex, In.vUV);

    outClr.color.rgb *= In.vColor.rgb;
    outClr.color.rgb *= In.vColor.a;

    return outClr;
}
#else
float4 mainPS(SPSInput In) : COLOR
{
    float4 vOut = tex2D(sTex, In.vUV);

    vOut.rgb *= In.vColor.rgb;
    vOut.rgb *= In.vColor.a;

    return vOut;
}

#endif

technique InstancingShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader = compile ps_2_0 mainPS();
    }
}

technique BasicShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS_Basic();
        PixelShader = compile ps_2_0 mainPS();
    }
}
