// BasicShader.fx

float4x4 g_mL2W;
float4x4 g_mW2C;

////////////////////////////////////////////////

struct SVSInput {
	float4 vPos		: POSITION;
	float3 vNml		: NORMAL;
};

struct SPSInput {
	float4 vPos		: POSITION;
	float4 vColor	: COLOR;
};

#define SVSOutput		SPSInput

////////////////////////////////////////////////
// ���_�V�F�[�_

SVSOutput mainVS(SVSInput sIn)
{
	SVSOutput sOut = (SVSOutput)0;

	sOut.vPos = mul(sIn.vPos, g_mL2W);
	sOut.vPos = mul(sOut.vPos, g_mW2C);

	sOut.vColor = float4(1.0f, 0.0f, 0.0f, 1.0f);

	return sOut;
}

////////////////////////////////////////////////
// �s�N�Z���V�F�[�_

float4 mainPS(SPSInput sIn)	: COLOR
{
	float4 vOut = sIn.vColor;

	return vOut;
}

////////////////////////////////////////////////

// NOTE
// �e�N�j�b�N�̐؂�ւ��͖ʓ|�Ȃ̂ŁA�p�X�̐؂�ւ��őΉ��E�E�E

technique t0
{
	pass p0
	{
		VertexShader = compile vs_2_0 mainVS();
		PixelShader = compile ps_2_0 mainPS();
	}
}
