// �|�X�g�G�t�F�N�g�@���m�g�[��

#include "PostEffectShader.fxh"

PES_ID("POSTEFFECT_Monotone");

////////////////////////////////////////////////////

// �V�[���e�N�X�`��
texture texScene : INPUT_SCENE
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);
>;

// �T���v��
sampler smplScene = sampler_state
{
	BindTex(texScene);
};

sampler smplTest = sampler_state
{
	BindTex(texScene);
};

float fWeight;

////////////////////////////////////////////////////

float4 mainPS_0(S_PE_PS_IN sIn) : COLOR
{
	float4 vColor = tex2D(smplScene, sIn.vUV);
	return ColorFilterMonotone(vColor);
}

// ���`��Ԃ���
float4 mainPS_1(S_PE_PS_IN sIn) : COLOR
{
	float4 vColor = tex2D(smplScene, sIn.vUV);
	return ColorFilterMonotone(vColor, fWeight);
}

////////////////////////////////////////////////////
// �e�N�j�b�N

technique t0
{
	pass p0
	{
		SetAlphaBlendEnable(false);
		SetAlphaTestEnable(false);

		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_0, mainPS_0);
	}
}

technique t1
{
	pass p0
	{
		SetAlphaBlendEnable(false);
		SetAlphaTestEnable(false);

		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_0, mainPS_1);
	}
}
