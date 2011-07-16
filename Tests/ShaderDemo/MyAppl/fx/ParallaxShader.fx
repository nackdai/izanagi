// NormalMap Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float2 vUV		: TEXCOORD0;
	float3 vNormal	: NORMAL;
	float4 vTangent : TANGENT;
};

struct SPSInput {
	float4 vPos			: POSITION;
	float3 vLight		: TEXCOORD0;
	float3 vHalf		: TEXCOORD1;	// �n�[�t�x�N�g��
	float2 vUV			: TEXCOORD2;
	float3 vEye			: TEXCOORD3;
	float4 vColor		: COLOR;		// Ambient
};

#define SVSOutput		SPSInput

struct SVSInput_NoTex {
	float4 vPos		: POSITION;
	float2 vUV		: TEXCOORD0;
	float4 vColor	: COLOR;
};

struct SPSInput_NoTex {
	float4 vPos		: POSITION;
	float2 vUV		: TEXCOORD0;
	float4 vColor	: COLOR;
};

#define SVSOutput_NoTex	SPSInput_NoTex

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

sampler sTex : register(s0);
sampler sNormalMap : register(s1);

/////////////////////////////////////////////////////////////
// ���_�V�F�[�_

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;

	// ���_�ւ̕����x�N�g��
	float3 vV = normalize(g_vEye - In.vPos);
	
	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);
	
	// Ambient
	Out.vColor = g_vMtrlAmbient * g_vLitAmbientColor;

	// NOTE
	// ���[�J�����W�ł̌v�Z�Ȃ̂�
	// ���C�g�̕����x�N�g����CPU���Ń��[�J�����W�ɕϊ�����Ă��邱��

	float3 vL = -g_vLitParallelDir.xyz;

	// Tangent-space
	float3 vN = In.vNormal;
	float3 vT = In.vTangent.xyz;
	float3 vB = In.vTangent.w * cross(vN, vT);

	// Tangent-space�ɕϊ�
	{
		vV.x = dot(vV, vT);
		vV.y = dot(vV, vB);
		vV.z = dot(vV, vN);

		Out.vLight.x = dot(vL, vT);
		Out.vLight.y = dot(vL, vB);
		Out.vLight.z = dot(vL, vN);
		Out.vLight = normalize(Out.vLight);
	}
	
	Out.vHalf = normalize(Out.vLight + vV);
	Out.vEye = vV;
	
	Out.vColor.a = 1.0f;

	Out.vUV = In.vUV;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	// ���_�V�F�[�_��Ambient�ɂ��Ă͌v�Z�ς�
	float4 vOut = In.vColor;

	// For tga
	// TGA�͏㉺���]����ۂ�
	float2 vUV = In.vUV;
	vUV.y = 1.0f - vUV.y;

	float fHeight = tex2D(sNormalMap, vUV).a;

	// 0.03 �͓K���E�E�E
	vUV = vUV + 0.03f * fHeight * In.vEye.xy;
	
	float3 vN = tex2D(sNormalMap, vUV);
	vN = (vN - 0.5f) * 2.0f;
	vN = normalize(vN);

	float3 vL = In.vLight;
	
	// Diffuse = Md * ��(C * max(N�EL, 0))
	vOut.rgb += g_vMtrlDiffuse.rgb * g_vLitParallelColor.rgb * max(0.0f, dot(vN, vL));
	
	float3 vH = normalize(In.vHalf);

	// Specular = Ms * ��(C * pow(max(N�EH, 0), m))
	vOut.rgb += g_vMtrlSpecular.rgb * g_vLitParallelColor.rgb * pow(max(0.0f, dot(vN, vH)), max(g_vMtrlSpecular.w, 0.00001f));

	vOut *= tex2D(sTex, In.vUV);
	
	return vOut;
}

////////////////////////////////////////////////

SVSOutput_NoTex mainVS_Std(SVSInput_NoTex sIn)
{
	SVSOutput_NoTex sOut = (SVSOutput_NoTex)0;

	sOut.vPos = mul(sIn.vPos, g_mL2W);
	sOut.vPos = mul(sOut.vPos, g_mW2C);

	sOut.vColor = sIn.vColor;
	sOut.vUV = sIn.vUV;

	return sOut;
}

float4 mainPS_Std(SPSInput_NoTex sIn)	: COLOR
{
	float4 vOut = sIn.vColor * tex2D(sTex, sIn.vUV);

	return saturate(vOut);
}

SVSOutput_NoTex mainVS_NoTex(SVSInput_NoTex sIn)
{
	SVSOutput_NoTex sOut = (SVSOutput_NoTex)0;

	sOut.vPos = mul(sIn.vPos, g_mL2W);
	sOut.vPos = mul(sOut.vPos, g_mW2C);

	sOut.vColor = sIn.vColor;

	return sOut;
}

float4 mainPS_NoTex(SPSInput_NoTex sIn)	: COLOR
{
	float4 vOut = sIn.vColor;

	return saturate(vOut);
}

/////////////////////////////////////////////////////////////

technique NormalMapShader
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}

#if 1
	pass p1
	{
		VertexShader = compile vs_2_0 mainVS_NoTex();
		PixelShader = compile ps_2_0 mainPS_NoTex();
	}

	pass p2
	{
		VertexShader = compile vs_2_0 mainVS_Std();
		PixelShader = compile ps_2_0 mainPS_Std();
	}
#endif
}
