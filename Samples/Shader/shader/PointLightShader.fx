// PointLight Shader

struct SVSInput {
    float4 vPos        : POSITION;
    float4 vColor    : COLOR;
};

struct SPSInput {
    float4 vPos        : POSITION;
    float4 vColor    : COLOR;
    float distance    : TEXCOORD0;
};

#define SVSOutput        SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;

// �ʒu
float4 g_PointLightPos;

// �W��
float4 g_PointLight;

// �F
float4 g_PointLightColor;

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;
    
    Out.vPos = mul(In.vPos, g_mL2W);

    // ���C�g�Ƃ̋���
    Out.distance = length(g_PointLightPos - Out.vPos);

    Out.vPos = mul(Out.vPos, g_mW2C);
        
    Out.vColor = In.vColor;
    
    return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
    // �|�C���g���C�g�̌���
    float d = In.distance;
    float attn = 1.0f / (g_PointLight.x + g_PointLight.y * d + g_PointLight.z * d * d);

    float4 vOut = In.vColor + attn * g_PointLightColor;
    vOut.a = In.vColor.a;
    
    return vOut;
}

/////////////////////////////////////////////////////////////

SVSOutput mainVS_NoLight(SVSInput In)
{
    SVSOutput Out = (SVSOutput)0;
    
    Out.vPos = mul(In.vPos, g_mL2W);
    Out.vPos = mul(Out.vPos, g_mW2C);
        
    Out.vColor = In.vColor;
    
    return Out;
}

float4 mainPS_NoLight(SPSInput In) : COLOR
{
    float4 vOut = In.vColor;
    
    return vOut;
}

/////////////////////////////////////////////////////////////

technique PointLight
{
    pass P0
    {
        VertexShader = compile vs_3_0 mainVS();
        PixelShader = compile ps_3_0 mainPS();
    }

    pass P1
    {
        VertexShader = compile vs_3_0 mainVS_NoLight();
        PixelShader = compile ps_3_0 mainPS_NoLight();
    }
}
