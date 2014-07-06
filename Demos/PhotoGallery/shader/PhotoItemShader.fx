// Basic Shader

struct SVSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float2 vUV      : TEXCOORD0;
    float4 vColor   : COLOR;
};

struct SPSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : TEXCOORD0;    // 法線
    float3 vHalf    : TEXCOORD1;    // ハーフベクトル
    float2 vUV      : TEXCOORD2;
    float4 vColor   : COLOR;
};

#define SVSOutput        SPSInput

struct SVSInput_NoTex {
    float4 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float4 vColor   : COLOR;
};

struct SPSInput_NoTex {
    float4 vPos     : POSITION;
    float3 vNormal  : TEXCOORD0;    // 法線
    float3 vHalf    : TEXCOORD1;    // ハーフベクトル
    float4 vColor   : COLOR;
};

#define SVSOutput_NoTex    SPSInput_NoTex

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

texture tex;

sampler sTex = sampler_state
{
    Texture = tex;
};

// x : scale x
// y : scale y
// z : height factor
// w : alpha factor
float4 g_Params;

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    float4 pos = In.vPos;
    pos.xy *= g_Params.xy;

    pos.y += g_Params.z;

    // 視点への方向ベクトル（ローカル座標）
    float3 vV = normalize(g_vEye - pos).xyz;

    Out.vPos = mul(pos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vUV = In.vUV;

    Out.vColor = In.vColor;
    Out.vColor.a *= g_Params.a;

    // NOTE
    // ローカル座標での計算なので
    // ライトの方向ベクトルはCPU側でローカル座標に変換されていること

    Out.vHalf = normalize(-g_vLitParallelDir.xyz + vV);
    Out.vNormal = In.vNormal;
    
    return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
    float4 vOut = tex2D(sTex, In.vUV);
    vOut.rgb = 0.0f;

    float3 vN = normalize(In.vNormal);
    float3 vH = normalize(In.vHalf);
    float3 vL = -g_vLitParallelDir.xyz;

    float4 ambient = g_vMtrlAmbient * g_vLitAmbientColor;
    
    // Diffuse = Md * ∑(C * max(N・L, 0))
    float3 diffuse = g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
    
    // Specular = Ms * ∑(C * pow(max(N・H, 0), m))
    float3 speculer = g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));

    vOut.rgb = In.vColor.rgb * vOut.rgb + (diffuse + speculer);
    vOut.a *= In.vColor.a;

    return vOut;
}

////////////////////////////////////////////////

SVSOutput_NoTex mainVS_NoTex(SVSInput_NoTex In)
{
    SVSOutput_NoTex Out = (SVSOutput_NoTex)0;

    float4 pos = In.vPos;
    pos.xy *= g_Params.xy;

    pos.y += g_Params.z;

    // 視点への方向ベクトル（ローカル座標）
    float3 vV = normalize(g_vEye - pos).xyz;

    Out.vPos = mul(pos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    // Ambient
    Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;

    Out.vColor = In.vColor;
    Out.vColor.a *= g_Params.a;

    // NOTE
    // ローカル座標での計算なので
    // ライトの方向ベクトルはCPU側でローカル座標に変換されていること

    Out.vHalf = normalize(-g_vLitParallelDir.xyz + vV);
    Out.vNormal = In.vNormal;

    return Out;
}

float4 mainPS_NoTex(SPSInput_NoTex In)    : COLOR
{
    float4 vOut = In.vColor;

    float3 vN = normalize(In.vNormal);
    float3 vH = normalize(In.vHalf);
    float3 vL = -g_vLitParallelDir.xyz;
    
    // Diffuse = Md * ∑(C * max(N・L, 0))
    vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
    
    // Specular = Ms * ∑(C * pow(max(N・H, 0), m))
    vOut.rgb += g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));

    return vOut;
}

/////////////////////////////////////////////////////////////

technique PhotoItemShader
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader = compile ps_2_0 mainPS();
    }
}

technique PhotoItemShader_NoTex
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS_NoTex();
        PixelShader = compile ps_2_0 mainPS_NoTex();
    }
}
