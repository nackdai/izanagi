// SpotLight Shader

struct SVSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
	float distance	: TEXCOORD0;
	float attn		: TEXCOORD1;
};

#define SVSOutput		SPSInput

/////////////////////////////////////////////////////////////

float4x4 g_mL2W;
float4x4 g_mW2C;

// �ʒu
float4 g_SpotLightPos;

// ����
float4 g_SpotLightDir;

// �W��
float4 g_SpotLight;

// �F
float4 g_SpotLightColor;

/////////////////////////////////////////////////////////////

SVSOutput mainVS(SVSInput In)
{
	SVSOutput Out = (SVSOutput)0;
	
	Out.vPos = mul(In.vPos, g_mL2W);

	// ���C�g�ւ̕����x�N�g��
	float3 vLightDir = g_SpotLightPos.xyz - Out.vPos.xyz;

	// ���C�g�Ƃ̋���
	Out.distance = length(vLightDir);

	// �P�ʃx�N�g���ɂ���
	vLightDir = normalize(vLightDir);

	// ���C�g�x�N�g���̋t�����ƃX�|�b�g���C�g�Ƃ̓���
	Out.attn = max(dot(g_SpotLightDir.xyz, -vLightDir), 0.0f);

	Out.vPos = mul(Out.vPos, g_mW2C);
		
	Out.vColor = In.vColor;
	
	return Out;
}

float4 mainPS(SPSInput In) : COLOR
{
	// �������v�Z
	float d = In.distance;
	float attn = pow(In.attn, g_SpotLight.w) / (g_SpotLight.x + g_SpotLight.y * d + g_SpotLight.z * d * d);

	float4 vOut = In.vColor + attn * g_SpotLightColor;
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
