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

samplerCUBE sTex = sampler_state
{
    Texture = texCube;
};

sampler texLatLong = sampler_state
{
    Texture = tex;
};

/////////////////////////////////////////////////////////////

SVSOutput mainVS_CrossMap(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    // NOTE
    // -1.0f - 1.0f のボックスであること！！
    Out.vUV = In.vPos.xyz;
    
    return Out;
}

float4 mainPS_CrossMap(SPSInput In) : COLOR
{
    float4 vOut = texCUBE(sTex, In.vUV);

    return vOut;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVS_LatLong(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    // 反射ベクトルを計算
    float3 ref = normalize(In.vPos.xyz - g_vEye.xyz);

    Out.vUV = ref;
    
    return Out;
}

float4 mainPS_LatLong(SPSInput In) : COLOR
{
    float3 ref = normalize(In.vUV);

    // 反射ベクトルからUVを計算

    // y = sin(θ) -> θ=asin(y)
    // [-pi/2 : pi/2]
    float latitude = asin(ref.y);
    float v = latitude / IZ_MATH_PI1_2;

    float cosLat = cos(latitude);

    // z = cos(θ)cos(φ) -> cos(φ) = z / cos(θ) -> φ = acos(z / cos(θ))
    float cosLong = ref.z / (cosLat + 0.00001f);
    cosLong = clamp(cosLong, -1.0f, 1.0f);

    float longitude = acos(cosLong);
    
    // NOTE
    // acosf -> 範囲[0, +π]

    longitude = (ref.x < 0.0f ? -longitude : longitude);

    float u = longitude / IZ_MATH_PI;

    u = (u + 1.0f) * 0.5f;
    v = (v + 1.0f) * 0.5f;

    // TODO
    // Why minus?
    float4 vOut = tex2D(texLatLong, float2(u, -v));

    return vOut;
}

/////////////////////////////////////////////////////////////

technique CrossMap
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS_CrossMap();
        PixelShader = compile ps_2_0 mainPS_CrossMap();
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
