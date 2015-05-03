// Bevel Shader

// NOTE
// +----------
// |
// |
// |->
// |
// |　　↑
// *----------
//
// １頂点につき２方向の情報が必要となる

struct SVSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : NORMAL;
    float4 vNextNml     : TEXCOORD1;    // 隣接面（辺の向こう側の面）の法線 
    float3 vDir         : TEXCOORD2;    // 反対側の辺への向き
    float4 vNextNml2    : TEXCOORD3;    // 隣接面（辺の向こう側の面）の法線
    float3 vDir2        : TEXCOORD4;    // 反対側の辺への向き
};

struct SPSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : TEXCOORD0;
    float4 vNextNml     : TEXCOORD1;
    float3 vDir         : TEXCOORD2;
    float3 vHalf        : TEXCOORD3;
    float4 vColor       : COLOR;
    float4 vNextNml2    : TEXCOORD4;
    float3 vDir2        : TEXCOORD5;
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

// 角丸の範囲
#define R   0.05f

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;
    
    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);
    
    Out.vNormal = mul(In.vNormal, (float3x3)g_mL2W);
    Out.vNormal = normalize(Out.vNormal);

    Out.vNextNml.xyz = mul(In.vNextNml.xyz, (float3x3)g_mL2W);
    Out.vNextNml.xyz = normalize(Out.vNextNml.xyz);
    Out.vNextNml.a = In.vNextNml.a;

    Out.vDir = mul(In.vDir, (float3x3)g_mL2W);

    // 視点への方向ベクトル（ローカル座標）
    float3 vV = normalize(g_vEye - In.vPos).xyz;

    Out.vHalf = normalize(-g_vLitParallelDir.xyz + vV);

    // Ambient
    Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;

    Out.vNextNml2.xyz = mul(In.vNextNml2.xyz, (float3x3)g_mL2W);
    Out.vNextNml2.xyz = normalize(Out.vNextNml2.xyz);
    Out.vDir2 = mul(In.vDir2, (float3x3)g_mL2W);
    
    return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
    // 反対側の辺への向き
    // 中央に向かって０になるように線形補間される
    float3 dir = In.vDir;

    // NOTE
    // 0   R         1
    //  +------------+
    //  |
    //  |

    // 法線の補間値
    // 辺に近いほど１、辺から遠いほど０になるようにしたい
    //  length(dir) => dirは中央に向かって０になるように線形補間されるので、length(dir)は辺に近いほど１に近い
    //  1.0f - length(dir) => 辺への近さ、０に近いほど辺に近い
    //  step(1.0f - length(dir), R) => 角丸の範囲内にいるかどうか。範囲内なら１、範囲外なら０
    //  (1.0f - length(dir)) / R => 角丸の範囲の端への近さ。０に近いほど辺に近く、１に近いほど角丸の範囲の端に近い
    //  1.0f - (1.0f - length(dir)) / R => 角丸の中心となる辺への近さ。１に近いほど辺に近い
    float f = (1.0f - (1.0f - length(dir)) / R) * step(1.0f - length(dir), R);
    float3 vN = In.vNormal.xyz;

    // NOTE
    // 角では自分の面と隣の面の間を取りたいので補間値を半分までにする
    float3 nml1 = lerp(vN, In.vNextNml.xyz, f * 0.5f);

    // 上と同じことをもう一つの法線と向きで行う
    {
        dir = In.vDir2;
        f = (1.0f - (1.0f - length(dir)) / R) * step(1.0f - length(dir), R);
        float3 nml2 = In.vNextNml2;
        nml2 = lerp(vN, nml2, f * 0.5f);
    }

    vN = lerp(nml1, nml2, 0.5f);
    vN = normalize(vN);

    float3 vH = normalize(In.vHalf);
    float3 vL = -g_vLitParallelDir.xyz;

    // 頂点シェーダでAmbientについては計算済み
    float4 vOut = In.vColor;

    // Diffuse = Md * ∑(C * max(N・L, 0))
    vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
    
    // Specular = Ms * ∑(C * pow(max(N・H, 0), m))
    vOut.rgb += g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));

    vOut.a = 1.0f;
        
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
