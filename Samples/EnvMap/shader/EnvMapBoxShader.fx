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
    // -1.0f - 1.0f �̃{�b�N�X�ł��邱�ƁI�I
    Out.vUV = In.vPos.xyz;
    
    return Out;
}

float4 mainPS_Cube(SPSInput In) : COLOR
{
    float4 vOut = texCUBE(sTexCube, In.vUV);

    return vOut;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVS_LatLong(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    // ���˃x�N�g�����v�Z
    float3 ref = normalize(In.vPos.xyz - g_vEye.xyz);

    Out.vUV = ref;
    
    return Out;
}

float4 mainPS_LatLong(SPSInput In) : COLOR
{
    float3 ref = normalize(In.vUV);

    // ���˃x�N�g������UV���v�Z

    // y = sin(��) -> ��=asin(y)
    // [-pi/2 : pi/2]
    float latitude = asin(ref.y);
    float v = latitude / IZ_MATH_PI1_2;

    float cosLat = cos(latitude);

    // z = cos(��)cos(��) -> cos(��) = z / cos(��) -> �� = acos(z / cos(��))
    float cosLong = ref.z / (cosLat + 0.00001f);
    cosLong = clamp(cosLong, -1.0f, 1.0f);

    float longitude = acos(cosLong);
    
    // NOTE
    // acosf -> �͈�[0, +��]

    longitude = (ref.x < 0.0f ? -longitude : longitude);

    float u = longitude / IZ_MATH_PI;

    u = (u + 1.0f) * 0.5f;
    v = (v + 1.0f) * 0.5f;

    // TODO
    // Why minus?
    float4 vOut = tex2D(sTex, float2(u, -v));

    return vOut;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVS_Angular(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);

    // ���˃x�N�g�����v�Z
    float3 ref = normalize(In.vPos.xyz - g_vEye.xyz);

    Out.vUV = ref;
    
    return Out;
}

// NOTE
// Mirror�Ƃ������āAAngular�͑S���ʂ��J�o�[����̂�
// �P���ɃƁA�ӂɗ��Ƃ����߂Ȃ�
//
// �S�����Ɍ������˃x�N�g��������A
// ����Z�l�́Az = cos�� �ƍl���邱�Ƃ��ł���
// �i���͑S���ʂ��J�o�[����l�ŃƁA�ӂ���Z�o�ł���͂��̂��́j
//
// Angular�͑S���ʂ���`�ɕ`��ɂ��Ă���̂ŁA����AngularMap��U,V�ʒu�ɔ�Ⴗ��
// �܂�Ar = sqrt(U * U + V * V) �ɔ�Ⴗ��
// �� = r * �� �ƂȂ�

float4 mainPS_Angular(SPSInput In) : COLOR
{
    float3 ref = normalize(In.vUV);

    float u = 0.0f;
    float v = 0.0f;

    // TODO
    // �Ȃ����������܂���ɂȂ�
    // PIX�ł݂�ƁARGBA = 0 �������Ă��邪
    // Debugger�ł݂�Ƃ���Ȃ��ƂɂȂ��Ă��Ȃ��E�E�E

    if (1.0f - 0.001f <= ref.z && ref.z <= 1.0f + 0.001f) {
        // z����O�Ɍ����Ă���Ƃ��͒��S
        u = 0.0f;
        v = 0.0f;
    }
    else {
        // z = cos��
        // [-��:��]
        float alpha = acos(ref.z);

        float r = alpha / IZ_MATH_PI;

        // sin(alpha)���v�Z
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

    // TODO
    // Why minus?
    float4 vOut = tex2D(sTex, float2(u, -v));

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
