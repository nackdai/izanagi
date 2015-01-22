// Bevel Shader

struct SVSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : NORMAL;
    float4 vNextNml : TEXCOORD1;
    float3 vDir     : TEXCOORD2;
};

struct SPSInput {
    float4 vPos     : POSITION;
    float3 vNormal  : TEXCOORD0;
    float4 vNextNml : TEXCOORD1;
    float3 vDir     : TEXCOORD2;
    float3 vHalf    : TEXCOORD3;
    float4 vColor   : COLOR;
};

#define SVSOutput        SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;
float4 g_vEye;

// �}�e���A��
float4 g_vMtrlDiffuse;
float4 g_vMtrlAmbient;
float4 g_vMtrlSpecular;

// ���C�g
float4 g_vLitParallelDir;
float4 g_vLitParallelColor;
float4 g_vLitAmbientColor;

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

    // ���_�ւ̕����x�N�g���i���[�J�����W�j
    float3 vV = normalize(g_vEye - In.vPos).xyz;

    Out.vHalf = normalize(-g_vLitParallelDir.xyz + vV);

    // Ambient
    Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;
    
    return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
    float f = (1.0f - (1.0f - length(In.vDir.xyz)) / R) * step(1.0f - length(In.vDir.xyz), R);
    float3 vN = In.vNormal.xyz;

    // NOTE
    // �p�ł͎����̖ʂƗׂ̖ʂ̊Ԃ���肽���̂ŕ�Ԓl�𔼕��܂łɂ���
    vN = lerp(vN, In.vNextNml.xyz, f * 0.5f);

    vN = normalize(vN);

    float3 vH = normalize(In.vHalf);
    float3 vL = -g_vLitParallelDir.xyz;

    // ���_�V�F�[�_��Ambient�ɂ��Ă͌v�Z�ς�
    float4 vOut = In.vColor;

    // Diffuse = Md * ��(C * max(N�EL, 0))
    vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
    
    // Specular = Ms * ��(C * pow(max(N�EH, 0), m))
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
