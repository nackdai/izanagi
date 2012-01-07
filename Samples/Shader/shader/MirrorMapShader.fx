// MirrorMap Shader

#define USE_REFLECTION

struct SVSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: NORMAL;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float3 vNormal	: TEXCOORD0;
};

#define SVSOutput		SPSInput

struct SVSInput_NoTex {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

struct SPSInput_NoTex {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

#define SVSOutput_NoTex	SPSInput_NoTex

/////////////////////////////////////////////////////////////

#define PI	(3.14159265358979323846f)

float4x4 g_mL2W;
float4x4 g_mW2C;
float4x4 g_mW2V;

texture tex;

sampler sTex = sampler_state
{
	Texture = tex;
};

/////////////////////////////////////////////////////////////

SVSOutput mainVS_Mirror(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;

	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);

	float3 vNormal = mul(In.vNormal, (float3x3)g_mL2W);
	vNormal = normalize(vNormal);

#ifdef USE_REFLECTION
	// �r���[���W�n�̖@�����v�Z
	vNormal = mul(vNormal, (float3x3)g_mW2V);

	// �r���[���W�n�ōl����̂ŁA���_�ւ̃x�N�g���͏��(0, 0, 1)
	float3 eye = float3(0.0f, 0.0f, 1.0f);

	// ���˃x�N�g�����v�Z
	float3 vRef = 2.0f * dot(vNormal, eye) * vNormal - eye;

	// �@���͎����x�N�g���Ɣ��˃x�N�g�������Z�������̂ƈ�v����
	// -> ���ǖ@���܂܂ƂȂ�
	vNormal = vRef + float3(0.0f, 0.0f, 1.0f);
	
	Out.vNormal = normalize(vNormal);
#else
	// �r���[���W�n�̖@�����v�Z
	Out.vNormal = mul(vNormal, (float3x3)g_mW2V);
#endif
	
	return Out;
}

float4 mainPS_Mirror(SPSInput In) : COLOR
{
	float2 vUV = float2(In.vNormal.x * 0.5f + 0.5f, In.vNormal.y * 0.5f + 0.5f);
	vUV.y = 1.0f - vUV.y;

	float4 vOut = tex2D(sTex, vUV);

	return vOut;
}

////////////////////////////////////////////////

SVSOutput_NoTex mainVS_NoTex(SVSInput_NoTex In)
{
	SVSOutput_NoTex Out = (SVSOutput_NoTex)0;

	Out.vPos = mul(In.vPos, g_mL2W);
	Out.vPos = mul(Out.vPos, g_mW2C);

	Out.vColor = In.vColor;

	return Out;
}

float4 mainPS_NoTex(SPSInput_NoTex In)	: COLOR
{
	float4 vOut = In.vColor;

	return vOut;
}

/////////////////////////////////////////////////////////////

technique Mirror
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS_Mirror();
		PixelShader = compile ps_2_0 mainPS_Mirror();
	}
}

technique NoTex
{
	pass P0
	{
		VertexShader = compile vs_2_0 mainVS_NoTex();
		PixelShader = compile ps_2_0 mainPS_NoTex();
	}
}
