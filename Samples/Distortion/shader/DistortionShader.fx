struct SVSInput {
    float4 vPos     : POSITION;
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD0;
};

struct SPSInput {
    float4 vPos     : POSITION;
    float4 vColor   : COLOR;
    float2 vUV      : TEXCOORD0;
    float2 scrPos   : TEXCOORD1;
};

texture tex;

sampler sTex = sampler_state
{
    Texture = tex;
};

#define SVSOutput SPSInput

// �X�N���[���T�C�Y�̋t��
float4 g_vInvScreen = (float4)1.0f;

SVSOutput mainVS(SVSInput sIn)
{
    SVSOutput sOut;

    // ���_�ʒu
    sOut.vPos = sIn.vPos;
    sOut.vPos.xy *= g_vInvScreen.xy;

    // [0.0f, 0.0f] - [1.0f, 1.0f] -> [-1.0f, -1.0f] - [1.0f, 1.0f]
    sOut.vPos.xy = sOut.vPos.xy * 2.0f - 1.0f;

    // �����Y���W�͔��]������
    sOut.vPos.y *= -1.0f;

    sOut.scrPos.x = sOut.vPos.x * 0.5f + 0.5f;
    sOut.scrPos.y = sOut.vPos.y * -0.5f + 0.5f;

    // ���_�J���[
    sOut.vColor = sIn.vColor;

    // UV���W
    sOut.vUV = sIn.vUV;

    return sOut;
}

float4 mainPS(SPSInput sIn) : COLOR
{
    float4 outColor = tex2D(sTex, sIn.vUV);

    return outColor;
}

technique Distortion
{
    pass P0
    {
        VertexShader = compile vs_2_0 mainVS();
        PixelShader = compile ps_2_0 mainPS();
    }
}
