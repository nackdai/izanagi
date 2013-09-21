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

float4 mainPS(SPSInput In) : COLOR
{
    float4 vOut = tex2D(sTex, In.vUV);

    vOut.rgb *= In.vColor.rgb;
    vOut.rgb *= In.vColor.a;

    return vOut;
}

technique BasicShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader = compile ps_2_0 mainPS();
    }
}
