struct SVSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : NORMAL;
    float2 vTexCoord    : TEXCOORD0;
};

struct SPSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : NORMAL;
    float2 vTexCoord    : TEXCOORD0;
    float4 vWorldPos    : TEXCOORD1;
};

#define SVSOutput        SPSInput

struct SPSOutput {
    float4 albedo   : COLOR0;
    float4 normal   : COLOR1;
    float4 depth    : COLOR2;
    float4 position : COLOR3;
};

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;

texture tex;

sampler smplTex = sampler_state
{
    Texture = tex;
};

/////////////////////////////////////////////////////////////

SVSOutput mainVSGeometryPass(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vWorldPos = mul(In.vPos, g_mL2W);

    Out.vPos = mul(Out.vWorldPos, g_mW2C);

    Out.vNormal = mul(In.vNormal, (float3x3)g_mL2W);
    Out.vNormal = normalize(Out.vNormal);

    Out.vTexCoord = In.vTexCoord;

    return Out;
}

SPSOutput mainPSGeometryPass(SPSInput In)
{
    SPSOutput Out = (SPSOutput)0;

    Out.albedo = tex2D(smplTex, In.vTexCoord);
    Out.albedo.a = 1.0f;

    Out.normal.rgb = In.vNormal * 0.5f + 0.5f;  // normalize [-1, 1] ->[0, 1]
    Out.normal.a = 1.0f;

    float4 clipPos = mul(In.vWorldPos, g_mW2C);
    float depth = clipPos.z / clipPos.w;
    Out.depth.r = depth;
    Out.depth.g = depth * 256.0f;
    Out.depth.b = depth * 256.0f * 256.0f;
    Out.depth = frac(Out.depth);
    Out.depth.a = 1.0f;

    Out.position = clipPos / clipPos.w;
    Out.position = (Out.position + 1.0f) * 0.5f;
    Out.position.a = 1.0f;

    return Out;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVSLightPass(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vTexCoord = Out.vPos.xy / Out.vPos.w;
    Out.vTexCoord = (Out.vTexCoord + 1.0f) * 0.5f;

    Out.vTexCoord.y = 1.0f - Out.vTexCoord.y;

    return Out;
}

texture texDepth;
texture texAlbedo;

sampler smplDepth = sampler_state
{
    Texture = texDepth;
};
sampler smplAlbedo = sampler_state
{
    Texture = texAlbedo;
};

// 位置
float4 g_PointLightPos;

// 係数
float4 g_PointLightAttn;

// 色
float4 g_PointLightColor;

float4x4 g_mtxInvW2C;

float4 mainPSLightPass(SPSInput In) : COLOR0
{
    float4 vDepth = tex2D(smplDepth, In.vTexCoord);
    float depth = vDepth.r + vDepth.g / 256.0f + vDepth.b / (256.0f * 256.0f);

    float4 pos = float4(In.vTexCoord.x, In.vTexCoord.y, depth, 1.0f);
    pos.xy = pos.xy * 2.0f - 1.0f;  // [0,1] -> [-1, 1]

    pos = mul(pos, g_mtxInvW2C);
    pos /= pos.w;

    // ポイントライトの減衰
    float d = length(g_PointLightPos - pos);

    float attn = 1.0f / (g_PointLightAttn.x + g_PointLightAttn.y * d + g_PointLightAttn.z * d * d);

    float4 Out = attn * g_PointLightColor;
    Out.a = 1.0f;

    float4 color = tex2D(smplAlbedo, In.vTexCoord);
    Out.rgb += color.rgb;

    return Out;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vWorldPos = mul(In.vPos, g_mL2W);

    Out.vPos = mul(Out.vWorldPos, g_mW2C);

    Out.vNormal = mul(In.vNormal, (float3x3)g_mL2W);
    Out.vNormal = normalize(Out.vNormal);

    Out.vTexCoord = In.vTexCoord;

    return Out;
}

float4 mainPS(SPSInput In) : COLOR0
{
    float4 Out = 0;

    Out = tex2D(smplTex, In.vTexCoord);

    return Out;
}

/////////////////////////////////////////////////////////////

technique GBuffer
{
    pass test
    {
        VertexShader = compile vs_3_0 mainVS();
        PixelShader = compile ps_3_0 mainPS();
    }

    pass geometryPass
    {
        VertexShader = compile vs_3_0 mainVSGeometryPass();
        PixelShader = compile ps_3_0 mainPSGeometryPass();
    }

    pass lightPass
    {
        VertexShader = compile vs_3_0 mainVSLightPass();
        PixelShader = compile ps_3_0 mainPSLightPass();

        ZWriteEnable = false;
        ZEnable = false;
    }
}
