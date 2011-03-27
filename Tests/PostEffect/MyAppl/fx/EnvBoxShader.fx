struct SVSInput {
	float4 vPos		: POSITION;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float3 vTex		: TEXCOORD0;
};

samplerCUBE s0 : register(s0);

///////////////////////////////////////////////////
// ���_�V�F�[�_

float4x4 g_mL2W;
float4x4 g_mW2C;

SPSInput mainVS(in SVSInput sIn)
{
	SPSInput sOut;

	sOut.vPos = mul(sIn.vPos, g_mL2W);
	sOut.vPos = mul(sOut.vPos, g_mW2C);

	// NOTE
	// -1.0f - 1.0f �̃{�b�N�X�ł��邱�ƁI�I
	sOut.vTex = sIn.vPos.xyz;

	return sOut;
}

///////////////////////////////////////////////////
// �s�N�Z���V�F�[�_

float4 mainPS(in SPSInput sIn) : COLOR0
{
	float4 vSample = texCUBE(s0, sIn.vTex.xyz);

	return vSample;
}

///////////////////////////////////////////////////
// �e�N�j�b�N

technique EnvBoxShader
{
	pass p0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}
