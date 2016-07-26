// Basic Shader

struct SVSInput {
    float4 vPos     : POSITION;
};

struct SPSInput {
    float4 vPos     : POSITION;
    float3 vUV      : TEXCOORD0;
};

#define SVSOutput        SPSInput

/////////////////////////////////////////////////////////////

#define IZ_MATH_PI          (3.14159265358979323846f)   // pi
#define IZ_MATH_PI2         (IZ_MATH_PI * 2.0f)         // 2pi
#define IZ_MATH_PI1_2       (IZ_MATH_PI * 0.5f)         // pi/2

float4x4 g_mL2W;
float4x4 g_mW2C;

float4 g_vEye;

texture texCube;
texture tex;

samplerCUBE sTexCube = sampler_state
{
    Texture = texCube;
};

sampler sTex = sampler_state
{
    Texture = tex;
};

/////////////////////////////////////////////////////////////

SVSOutput mainVS_Cube(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    // NOTE
    // -1.0f - 1.0f のボックスであること！！
    Out.vUV = In.vPos.xyz;
    
    return Out;
}

float4 mainPS_Cube(SPSInput In) : COLOR
{
    float4 vOut = texCUBE(sTexCube, In.vUV);

    return vOut;
}

/////////////////////////////////////////////////////////////

// For Equirect

SVSOutput mainVS_LatLong(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);

    // Compute reflection vector.
    float3 ref = normalize(Out.vPos.xyz - g_vEye.xyz);

    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vUV = ref;
    
    return Out;
}

float4 mainPS_LatLong(SPSInput In) : COLOR
{
    float3 ref = normalize(In.vUV);

    // [-pi/2, pi/2] -> [0, pi]
    float theta = asin(ref.y) + IZ_MATH_PI1_2;

    // [-pi, pi] -> [0, 2pi]
    float phi = atan2(ref.x, ref.z) + IZ_MATH_PI;

    // Normalize [0, 1]
    float2 uv = float2(phi / IZ_MATH_PI2, 1.0f - theta / IZ_MATH_PI);

    float4 vOut = tex2D(sTex, uv);

    return vOut;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVS_Angular(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);

    // 反射ベクトルを計算
    float3 ref = normalize(Out.vPos.xyz - g_vEye.xyz);

    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vUV = ref;
    
    return Out;
}

// NOTE
// Mirrorとちがって、Angularは全方位をカバーするので
// 単純にθ、φに落とし込めない
//
// 全方向に向く反射ベクトルがあり、
// そのZ値は、z = cosα と考えることができる
// （αは全方位をカバーする値でθ、φから算出できるはずのもの）
//
// Angularは全方位を線形に描画にしているので、αはAngularMapのU,V位置に比例する
// つまり、r = sqrt(U * U + V * V) に比例する
// α = r * π となる

float4 mainPS_Angular(SPSInput In) : COLOR
{
    float3 ref = normalize(In.vUV);

    float u = 0.0f;
    float v = 0.0f;

    // TODO
    // なぜか中央がまだらになる
    // PIXでみると、RGBA = 0 が入っているが
    // Debuggerでみるとそんなことになっていない・・・

    if (1.0f - 0.001f <= ref.z && ref.z <= 1.0f + 0.001f) {
        // zが手前に向いているときは中心
        u = 0.0f;
        v = 0.0f;
    }
    else {
        // z = cosα
        // [-π:π]
        float alpha = acos(ref.z);

        float r = alpha / IZ_MATH_PI;

        // sin(alpha)を計算
        //float sin = sqrt(1.0f - ref.z * ref.z);
        float s = sin(alpha);

        // NOTE
        // ref.x = sin * u / r <-> u = ref.x * r / sin
        // ref.y = sin * v / r <-> v = ref.y * r / sin

        u = ref.x * r / (s + 0.00001f);
        v = ref.y * r / (s + 0.00001f);

        u = clamp(u, -1.0f, 1.0f);
        v = clamp(v, -1.0f, 1.0f);
    }

    u = (u + 1.0f) * 0.5f;
    v = (v + 1.0f) * 0.5f;

    float4 vOut = tex2D(sTex, float2(u, v));

    return vOut;
}


/////////////////////////////////////////////////////////////

technique Cube
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS_Cube();
        PixelShader = compile ps_2_0 mainPS_Cube();
    }
}

technique LatLong
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS_LatLong();
        PixelShader = compile ps_3_0 mainPS_LatLong();
    }
}

technique Angular
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS_Angular();
        PixelShader = compile ps_3_0 mainPS_Angular();
    }
}
