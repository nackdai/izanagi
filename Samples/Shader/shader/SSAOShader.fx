// Phong Shader

#define USE_REFLECTION

struct SVSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float4 vColor   : COLOR;
};

struct SPSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : TEXCOORD0;    // 法線
    float4 ambient      : COLOR0;
    float4 position     : TEXCOORD1;
};

#define SVSOutput        SPSInput

struct SPSOutput {
    float4 ambient  : COLOR0;
    float4 normal   : COLOR1;
    float4 depth    : COLOR2;
    float4 position : COLOR3;
};

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2V;
float4x4 g_mV2C;
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
    Out.vPos = mul(Out.vPos, g_mW2V);
    Out.vPos = mul(Out.vPos, g_mV2C);
    
    Out.vNormal = normalize(mul(In.vNormal, (float3x3)g_mL2W));

    // Ambient
    Out.ambient = g_vMtrlAmbient * g_vLitAmbientColor;

    Out.position = mul(In.vPos, g_mL2W);
    
    return Out;
}

SPSOutput mainPS(SPSInput In)
{
    SPSOutput vOut = (SPSOutput)0;

    vOut.ambient = In.ambient;
    vOut.ambient.a = 1.0f;
    
    vOut.normal.xyz = normalize(In.vNormal);
    vOut.normal.xyz = 0.5f * vOut.normal.xyz + 0.5f;
    vOut.normal.a = 1.0f;
    
    float4 view = mul(In.position, g_mW2V);
    float4 projected = mul(view, g_mV2C);
    float d = projected.z / projected.w;
    vOut.depth = float4(d, d, d, 1.0f);

    vOut.position = In.position;
    
    return vOut;
}

/////////////////////////////////////////////////////////////

struct SVSInput2 {
    float4 vPos     : POSITION;
    float2 vUV      : TEXCOORD0;
};

struct SPSInput2 {
    float4 vPos     : POSITION;
    float2 vUV      : TEXCOORD0;
};

texture texAmbient;
texture texNormal;
texture texPosition;
texture texDepth;

sampler sTexAmbient = sampler_state
{
    Texture = texAmbient;
};

sampler sTexNormal = sampler_state
{
    Texture = texNormal;
};

sampler sTexDepth = sampler_state
{
    Texture = texDepth;
};

sampler sTexPosition = sampler_state
{
    Texture = texPosition;
};

#define SAMPLE_NUM  (32)
float4 samples[SAMPLE_NUM];

SPSInput2 mainVS2(SVSInput2 sIn)
{
    SPSInput2 sOut;

    // 頂点位置
    sOut.vPos = sIn.vPos;

    // [0.0f, 0.0f] - [1.0f, 1.0f] -> [-1.0f, -1.0f] - [1.0f, 1.0f]
    sOut.vPos.xy = sOut.vPos.xy * 2.0f - 1.0f;

    // さらにY座標は反転させる
    sOut.vPos.y *= -1.0f;

    // UV座標
    sOut.vUV = sIn.vUV;

    return sOut;
}

#define USER_NORMAL

float4 mainPS2(SPSInput2 sIn) : COLOR0
{
    float4 ambient = tex2D(sTexAmbient, sIn.vUV);

#ifdef USER_NORMAL
    float3 normal = tex2D(sTexNormal, sIn.vUV).rgb;
    normal.xyz = 2.0f * normal.xyz - 1.0f;
#endif

    float4 position = tex2D(sTexPosition, sIn.vUV);

    int count = 0;

    for (int i = 0; i < SAMPLE_NUM; i++) {
#ifdef USER_NORMAL
        float4 ray = samples[i];
        ray.xyz *= sign(dot(ray.xyz, normal));

        float4 pos = mul(position + ray, g_mW2V);
#else
        float4 pos = mul(position + samples[i], g_mW2V);
#endif
        pos = mul(pos, g_mV2C);

        float z = pos.z / pos.w;

        float2 uv = float2(
            0.5f * (pos.x / pos.w) + 0.5f,
            -0.5f * (pos.y / pos.w) + 0.5f);

        //float depth = tex2D(sTexDepth, sIn.vUV).r;
        float depth = tex2D(sTexDepth, uv).r;

        count += step(z, depth);
    }

    float a = clamp(count * 2.0f / (float)SAMPLE_NUM, 0.0f, 1.0f);

    float4 Out = ambient * (float4)a;
    Out.a = 1.0f;

    return Out;
}

/////////////////////////////////////////////////////////////

technique RenderToMRT
{
    pass P0
    {
        AlphaBlendEnable = false;
        VertexShader = compile vs_3_0 mainVS();
        PixelShader = compile ps_3_0 mainPS();
    }
}

technique Render
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS2();
        PixelShader = compile ps_3_0 mainPS2();
    }
}
