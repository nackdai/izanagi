// Phong Shader

#define USE_REFLECTION

struct SVSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float4 vColor   : COLOR;
};

struct SPSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : TEXCOORD0;    // �@��
    float4 ambient      : COLOR0;
    float4 viewSpace    : TEXCOORD1;
};

#define SVSOutput        SPSInput

struct SPSOutput {
    float4 albedo   : COLOR0;
    float4 normal   : COLOR1;
    float4 depth    : COLOR2;
};

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2V;
float4x4 g_mW2C;
float4x4 g_mV2C;

float g_farClip;

// �}�e���A��
float4 g_vMtrlAmbient;

// ���C�g
float4 g_vLitAmbientColor;

/////////////////////////////////////////////////////////////

SVSOutput mainVSGeometryPass(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;

    Out.vPos = mul(In.vPos, g_mL2W);

    Out.viewSpace = mul(Out.vPos, g_mW2V).z;
    Out.viewSpace /= g_farClip;

    Out.vPos = mul(Out.vPos, g_mW2C);

    Out.vNormal = mul(In.vNormal, (float3x3)g_mL2W);
    Out.vNormal = normalize(Out.vNormal);

    // Ambient
    Out.ambient = g_vMtrlAmbient * g_vLitAmbientColor;

    return Out;
}

SPSOutput mainPSGeometryPass(SPSInput In)
{
    SPSOutput Out = (SPSOutput)0;

    Out.normal.rgb = In.vNormal * 0.5f + 0.5f;  // normalize [-1, 1] ->[0, 1]
    Out.normal.a = 1.0f;

    Out.depth = In.viewSpace;
    Out.depth.a = 1.0f;

    Out.albedo = In.ambient;
    Out.albedo.a = 1.0f;

    return Out;
}

/////////////////////////////////////////////////////////////

struct SVSInputSSAO {
    float4 vPos     : POSITION;
    float2 vUV      : TEXCOORD0;
};

struct SPSInputSSAO {
    float4 vPos     : POSITION;
    float2 vUV      : TEXCOORD0;
};

#define SVSOutputSSAO   SPSInputSSAO

texture texAmbient;
texture texNormal;
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

float4x4 g_mC2V;
float4x4 g_mV2W;

#define SAMPLE_NUM  (32)
float4 samples[SAMPLE_NUM];

SPSInputSSAO mainVS_SSAO(SVSInputSSAO sIn)
{
    SVSOutputSSAO sOut;

    // ���_�ʒu
    sOut.vPos = sIn.vPos;

    // [0, 1] -> [-1, 1]
    sOut.vPos.xy = sOut.vPos.xy * 2.0f - 1.0f;

    // �����Y���W�͔��]������
    sOut.vPos.y *= -1.0f;

    // UV���W
    sOut.vUV = sIn.vUV;

    return sOut;
}

float4 mainPS_SSAO(SPSInputSSAO sIn) : COLOR0
{
    float falloff = 0.000002;

    float4 ambient = tex2D(sTexAmbient, sIn.vUV);

    // ���ۂ̈ʒu�ł̖@��
    float3 normal = tex2D(sTexNormal, sIn.vUV).rgb;
    normal = normalize(2.0f * normal - 1.0f);

    // Depth format is R32F.
    float centerDepth = tex2D(sTexDepth, sIn.vUV).r;

    // Convert depth [0, 1] -> [0, far] in view-space.
    float depth = centerDepth * g_farClip;

    float4 position = float4(sIn.vUV.x, sIn.vUV.y, 0.0f, 1.0f);

    // Convert from texcoord to clip-space.
    position.y = 1.0f - position.y;
    position.xy = position.xy * 2.0f - 1.0f;  // [0, 1] -> [-1, 1]

    // NOTE
    // P = (X, Y, Z, 1)
    // mtxV2C = W 0 0 0
    //          0 H 0 0
    //          0 0 A 1
    //          0 0 B 0
    // Pview * mtxV2C = (Xclip, Yclip, Zclip, Wclip) = (Xclip, Yclip, Zclip, Zview)
    //  Wclip = Zview = depth
    // X' = Xclip / Wclip = Xclip / Zview = Xclip / depth
    // Y' = Yclip / Wclip = Yclip / Zview = Yclip / depth
    //
    // X' * depth = Xclip
    // Xview = Xclip * mtxC2V

    position.xy *= depth;
    position = mul(position, g_mC2V);

    position.xy /= position.z;
    position.z = depth;

    position = mul(position, g_mV2W);

#if 1
    float bl = 0;

    for (int i = 0; i < SAMPLE_NUM; i++) {
        float4 ray = samples[i];

        // NOTE
        // �@���Ƌt�����������Ă���Ƃ��͔��]���邱�ƂŖ@�������̔��~���ɂ����܂�悤�ɂ���
        ray.xyz *= sign(dot(ray.xyz, normal));

        // �T���v�����O�_�̈ʒu���炵���ʒu�̌v�Z
        float4 pos = mul(position + ray, g_mW2C);
        //pos = mul(pos, g_mV2C);

        // �T���v�����O�_�̂��炵���ʒu��2D���UV�ɕϊ�
        float2 uv = float2(
            0.5f * (pos.x / pos.w) + 0.5f,
            -0.5f * (pos.y / pos.w) + 0.5f);

        // �Օ�����s�N�Z���̖@��
        float3 ocNml = tex2D(sTexNormal, uv).xyz;
        ocNml = normalize(ocNml * 2.0f - 1.0f);

        // �Օ�����s�N�Z���ƌ��݂̃s�N�Z���ł̖@���̊p�x�̍����v�Z
        // �������Ȃ�1.0�A���΂Ȃ�0.0
        // �������������Ă�����Օ�����Ă��Ȃ��A�p�x���傫���قǎՕ�����Ă���A�Ƃ݂Ȃ�
        //  -> �������������Ă��� = �����ʏ�ɂ��� -> �Օ����Ă��Ȃ�
        //  -> �p�x���傫�� = �����ʏ�ɂȂ� -> �Օ����Ă���
        float nmlDiff = 1.0 - dot(ocNml, normal);

        // �Օ�����s�N�Z���̐[�x
        float sampleDepth = tex2D(sTexDepth, uv).r;

        // ���F�Օ�����s�N�Z�������݂̃s�N�Z�����O -> ���݂̃s�N�Z���͎Օ������
        // ���F�Օ�����s�N�Z�������݂̃s�N�Z����艜 -> ���݂̃s�N�Z���͎Օ�����Ȃ�
        float depthDiff = centerDepth - sampleDepth;

        // step(falloff, depthDiff) -> falloff ��菬������� 0.0�A�傫����� 1.0
        //  -> ������x�ȏ�[�x������Ă��Ȃ��ꍇ�͎Օ�����Ă��邱�ƂƂ���
        bl += step(falloff, depthDiff) * nmlDiff;
    }

    // �T���v�����O�_�̍��v�Ȃ̂ŕ��ς��v�Z����
    bl /= (float)SAMPLE_NUM;

    // bl��1.0�ɋ߂��قǎՕ������̂�
    float a = 1.0f - bl;

    float4 Out = ambient * (float4)a;
    Out.a = 1.0f;

    if (centerDepth >= 1.0f) {
        Out.a = 0.0f;
    }

    return Out;
#else
    float4 Out = position;
    Out.a = 1.0f;

    if (centerDepth >= 1.0f) {
        Out.a = 0.0f;
    }

    return Out;
#endif
}

float4 mainPS_Ambient(SPSInputSSAO sIn) : COLOR0
{
    float4 ambient = tex2D(sTexAmbient, sIn.vUV);
    ambient.a = 1.0f;
    return ambient;
}

/////////////////////////////////////////////////////////////

technique RenderToMRT
{
    pass P0
    {
        AlphaBlendEnable = false;
        VertexShader = compile vs_3_0 mainVSGeometryPass();
        PixelShader = compile ps_3_0 mainPSGeometryPass();
    }
}

technique RenderSSAO
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS_SSAO();
        PixelShader = compile ps_3_0 mainPS_SSAO();
    }
}

#if 0
technique RenderNoSSAO
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS_SSAO();
        PixelShader = compile ps_3_0 mainPS_Ambient();
    }
}
#endif
