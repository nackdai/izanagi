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
    float3 vHalf        : TEXCOORD1;    // ハーフベクトル
    float4 ambient      : COLOR0;
    float4 position     : TEXCOORD2;
};

#define SVSOutput        SPSInput

struct SPSOutput {
    float4 ambient  : COLOR0;
    float4 diffspec : COLOR1;
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

    // 視点への方向ベクトル（ローカル座標）
    float3 vV = normalize(g_vEye - In.vPos).xyz;
    
    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2V);
    Out.vPos = mul(Out.vPos, g_mV2C);
    
    Out.vNormal = normalize(In.vNormal);

    // Ambient
    Out.ambient = g_vMtrlAmbient * g_vLitAmbientColor;

    // NOTE
    // ローカル座標での計算なので
    // ライトの方向ベクトルはCPU側でローカル座標に変換されていること

    Out.vHalf = normalize(-g_vLitParallelDir.xyz + vV);

    Out.position = mul(In.vPos, g_mL2W);
    
    return Out;
}

SPSOutput mainPS(SPSInput In)
{
    // 頂点シェーダでAmbientについては計算済み
    SPSOutput vOut = (SPSOutput)0;

    vOut.ambient = In.ambient;
    vOut.ambient.a = 1.0f;
    
    // いるのか・・・
    float3 vN = normalize(In.vNormal);
    float3 vH = normalize(In.vHalf);
    float3 vL = -g_vLitParallelDir.xyz;
    
    // Diffuse = Md * ∑(C * max(N・L, 0))
    vOut.diffspec.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
    
    // Specular = Ms * ∑(C * pow(max(N・H, 0), m))
    vOut.diffspec.rgb += g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));

    vOut.diffspec.a = 1.0f;

    float4 view = mul(In.position, g_mW2V);
    float4 projected = mul(view, g_mV2C);
    float d = projected.z / projected.w;
    vOut.depth = float4(d, d, d, 1.0f);

#if 0
    float t = length(view.xyz);
    view.xyz = normalize(view.xyz);
    vOut.position.xyz = view.xyz * 0.5f + 0.5f;
    vOut.position.a = t;
#else
    vOut.position = In.position;
#endif
    
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
texture texDifSpec;
texture texPosition;
texture texDepth;

sampler sTexAmbient = sampler_state
{
    Texture = texAmbient;
};

sampler sTexDifSpec = sampler_state
{
    Texture = texDifSpec;
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

float4 mainPS2(SPSInput2 sIn) : COLOR0
{
    //float4 ambient = tex2D(sTexAmbient, sIn.vUV);
    //float4 difspec = tex2D(sTexDifSpec, sIn.vUV);

    float4 position = tex2D(sTexPosition, sIn.vUV);

    int count = 0;

    for (int i = 0; i < SAMPLE_NUM; i++) {
        float4 pos = mul(position + samples[i], g_mW2V);
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

    //float4 Out = float4(a, a, a, 1.0f);
    float4 Out = (float4)a;
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
