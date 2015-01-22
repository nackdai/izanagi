// Bevel Shader

struct SVSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float4 vNextNml : TEXCOORD1;
    float3 vDir     : TEXCOORD2;
};

struct SPSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : TEXCOORD0;
    float4 vNextNml : TEXCOORD1;
    float3 vDir     : TEXCOORD2;
    float4 vTarget  : TEXCOORD3;
};

#define SVSOutput        SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;
float4 g_vEye;

// マテリアル
float4 g_vMtrlDiffuse;
float4 g_vMtrlAmbient;
float4 g_vMtrlSpecular;

// ライト
float4 g_vLitParallelDir;
float4 g_vLitParallelColor;
float4 g_vLitAmbientColor;

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;
    
    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vTarget = mul(In.vPos, g_mL2W);
    
    Out.vNormal = mul(In.vNormal, (float3x3)g_mL2W);
    Out.vNormal = normalize(Out.vNormal);

    Out.vNextNml.xyz = mul(In.vNextNml.xyz, (float3x3)g_mL2W);
    Out.vNextNml.xyz = normalize(Out.vNextNml.xyz);
    Out.vNextNml.a = In.vNextNml.a;

    Out.vDir = mul(In.vDir, (float3x3)g_mL2W);
    
    return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
    //float4 vOut = In.vNextNml * 0.5f + 0.5f;
    //vOut.a = 1.0f;
    //float4 vOut = float4(In.vDir.x, In.vDir.y, In.vDir.z, 1.0f);
    //vOut.rgb = vOut.rgb * 0.5f + 0.5f;

    //float4 vOut = In.vTarget;
    //vOut.xyz *= normalize(In.vDir);

    float4 vOut = float4(In.vDir.x, In.vDir.y, In.vDir.z, 1.0f);
    //vOut.rgb = step(2.5f - length(vOut.xyz), 0.5f);
    vOut.rgb = (1.0f - (2.5f - length(vOut.xyz)) / 0.5f) * step(2.5f - length(vOut.xyz), 0.5f);
        
    return vOut;
}

technique Bevel
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS();
        PixelShader = compile ps_3_0 mainPS();
    }
}
