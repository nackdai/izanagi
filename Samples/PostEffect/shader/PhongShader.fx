// Phong Shader

#define USE_REFLECTION

struct SVSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: NORMAL;
	float4 vColor	: COLOR;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: TEXCOORD0;	// �@��
	float3 vHalf	: TEXCOORD1;	// �n�[�t�x�N�g��
	float4 vColor	: COLOR;
};

#define SVSOutput		SPSInput

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

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;

	// ���_�ւ̕����x�N�g���i���[�J�����W�j
	float3 vV = normalize(g_vEye - In.vPos).xyz;
	
	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);
	
	Out.vNormal = normalize(In.vNormal);

	// Ambient
	Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;

	// NOTE
	// ���[�J�����W�ł̌v�Z�Ȃ̂�
	// ���C�g�̕����x�N�g����CPU���Ń��[�J�����W�ɕϊ�����Ă��邱��

	Out.vHalf = normalize(-g_vLitParallelDir.xyz + vV);
	
	Out.vColor.rgb *= In.vColor.rgb;
	Out.vColor.a = In.vColor.a;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	// ���_�V�F�[�_��Ambient�ɂ��Ă͌v�Z�ς�
	float4 vOut = In.vColor;
	
	// ����̂��E�E�E
	float3 vN = normalize(In.vNormal);
	float3 vH = normalize(In.vHalf);
	float3 vL = -g_vLitParallelDir.xyz;
	
	// Diffuse = Md * ��(C * max(N�EL, 0))
	vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
	
	// Specular = Ms * ��(C * pow(max(N�EH, 0), m))
	vOut.rgb += g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));
	
	return vOut;
}

/////////////////////////////////////////////////////////////

technique Phong
{
	pass P0
	{
		VertexShader = compile vs_3_0 mainVS();
		PixelShader = compile ps_3_0 mainPS();
	}
}
