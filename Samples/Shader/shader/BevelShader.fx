// Bevel Shader

// NOTE
// +----------
// |
// |
// |->
// |
// |�@�@��
// *----------
//
// �P���_�ɂ��Q�����̏�񂪕K�v�ƂȂ�

struct SVSInput {
    float4 vPos         : POSITION;
    float3 vNormal      : NORMAL;
    float4 vNextNml     : TEXCOORD1;    // �אږʁi�ӂ̌��������̖ʁj�̖@�� 
    float3 vDir         : TEXCOORD2;    // ���Α��̕ӂւ̌���
    float4 vNextNml2    : TEXCOORD3;    // �אږʁi�ӂ̌��������̖ʁj�̖@��
    float3 vDir2        : TEXCOORD4;    // ���Α��̕ӂւ̌���
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

// �}�e���A��
float4 g_vMtrlDiffuse;
float4 g_vMtrlAmbient;
float4 g_vMtrlSpecular;

// ���C�g
float4 g_vLitParallelDir;
float4 g_vLitParallelColor;
float4 g_vLitAmbientColor;

// �p�ۂ͈̔�
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

    Out.vNextNml2.xyz = mul(In.vNextNml2.xyz, (float3x3)g_mL2W);
    Out.vNextNml2.xyz = normalize(Out.vNextNml2.xyz);
    Out.vDir2 = mul(In.vDir2, (float3x3)g_mL2W);
    
    return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
    // ���Α��̕ӂւ̌���
    // �����Ɍ������ĂO�ɂȂ�悤�ɐ��`��Ԃ����
    float3 dir = In.vDir;

    // NOTE
    // 0   R         1
    //  +------------+
    //  |
    //  |

    // �@���̕�Ԓl
    // �ӂɋ߂��قǂP�A�ӂ��牓���قǂO�ɂȂ�悤�ɂ�����
    //  length(dir) => dir�͒����Ɍ������ĂO�ɂȂ�悤�ɐ��`��Ԃ����̂ŁAlength(dir)�͕ӂɋ߂��قǂP�ɋ߂�
    //  1.0f - length(dir) => �ӂւ̋߂��A�O�ɋ߂��قǕӂɋ߂�
    //  step(1.0f - length(dir), R) => �p�ۂ͈͓̔��ɂ��邩�ǂ����B�͈͓��Ȃ�P�A�͈͊O�Ȃ�O
    //  (1.0f - length(dir)) / R => �p�ۂ͈̔͂̒[�ւ̋߂��B�O�ɋ߂��قǕӂɋ߂��A�P�ɋ߂��قǊp�ۂ͈̔͂̒[�ɋ߂�
    //  1.0f - (1.0f - length(dir)) / R => �p�ۂ̒��S�ƂȂ�ӂւ̋߂��B�P�ɋ߂��قǕӂɋ߂�
    float f = (1.0f - (1.0f - length(dir)) / R) * step(1.0f - length(dir), R);
    float3 vN = In.vNormal.xyz;

    // NOTE
    // �p�ł͎����̖ʂƗׂ̖ʂ̊Ԃ���肽���̂ŕ�Ԓl�𔼕��܂łɂ���
    float3 nml1 = lerp(vN, In.vNextNml.xyz, f * 0.5f);

    // ��Ɠ������Ƃ�������̖@���ƌ����ōs��
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
