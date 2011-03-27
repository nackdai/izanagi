// �|�X�g�G�t�F�N�g�@BloomStar

#include "PostEffectShader.fxh"

PES_ID("POSTEFFECT_BloomStar");

////////////////////////////////////////////////////
// �e�N�X�`��

#define BASE_TEX_FMT	PIXEL_FORMAT_RGBA16F
//#define BASE_TEX_FMT	PIXEL_FORMAT_RGBA8

// �V�[���e�N�X�`��
texture texScene : INPUT_SCENE
<
	SetTexFormat(BASE_TEX_FMT);
>;

// 1/4 x 1/4 �k���o�b�t�@
texture texDownScale4x4
<
	SetTexFormat(BASE_TEX_FMT);

	SetTexRatio(0.25f, 0.25f);	// 1/4
>;

// �P�x���o�o�b�t�@
texture texBright
<
	SetTexFormat(BASE_TEX_FMT);

	SetTexRatio(0.25f, 0.25f);	// 1/4
	SetTexSize(2, 2);
>;

// �X�^�[��
texture texStarSource
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.25f, 0.25f);	// 1/4
	SetTexSize(2, 2);
>;

// �u���[����
texture texBloomSource
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.125f, 0.125f);	// 1/8
	SetTexSize(2, 2);
>;

// �u���[���r��
texture texBloomIterate
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.125f, 0.125f);	// 1/8
	SetTexSize(2, 2);
>;

// �u���[��
texture texBloom
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.125f, 0.125f);	// 1/8
>;

// �X�^�[
texture texStar
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.25f, 0.25f);	// 1/4
	SetTexSize(2, 2);
>;

////////////////////////////////////////////////////
// �T���v��

#ifdef __ENABLE_LINEAR_SAMPLE__
#define SAMPLER_FILTER()\
	SetMinFilter(SMPL_FILTER_LINEAR);\
	SetMagFilter(SMPL_FILTER_LINEAR)
#else	// #ifdef __ENABLE_LINEAR_SAMPLE__
#define SAMPLER_FILTER()\
	SetMinFilter(SMPL_FILTER_POINT);\
	SetMagFilter(SMPL_FILTER_POINT)
#endif	// #ifdef __ENABLE_LINEAR_SAMPLE__

// �V�[���e�N�X�`��
sampler smplScene = sampler_state
{
	BindTex(texScene);
	SAMPLER_FILTER();
};

// 1/4 x 1/4 �k���o�b�t�@
sampler smplDownScale4x4 = sampler_state
{
	BindTex(texDownScale4x4);
	SAMPLER_FILTER();
};

// �P�x���o�o�b�t�@
sampler smplBright = sampler_state
{
	BindTex(texBright);
	SAMPLER_FILTER();
};

// �X�^�[��
sampler smplStarSource = sampler_state
{
	BindTex(texStarSource);
};

// �u���[����
sampler smplBloomSource = sampler_state
{
	BindTex(texBloomSource);
};

// �u���[���r��
sampler smplBloomIterate = sampler_state
{
	BindTex(texBloomIterate);
};

// �u���[���Ō�
sampler smplBloomFinal = sampler_state
{
	BindTex(texBloomSource);
};

// �u���[��
sampler smplBloom = sampler_state
{
	BindTex(texBloom);
};

// �X�^�[
sampler smplStar = sampler_state
{
	BindTex(texStar);
};

////////////////////////////////////////////////////
// 1/4 x 1/4 �k���o�b�t�@�쐬

#ifdef __ENABLE_LINEAR_SAMPLE__
float4 mainDownScale4x4(S_PE_PS_IN_DOWNSCALE_4x4_LINEAR sIn) : COLOR
{
	float4 vRet = DownScale4x4_Linear(smplScene, sIn);
	return vRet;
}
#else	// #ifdef __ENABLE_LINEAR_SAMPLE__
float4 mainDownScale4x4(S_PE_PS_IN_DOWNSCALE_4x4_POINT sIn) : COLOR
{
	float4 vRet = DownScale4x4_Point(smplScene, sIn);
	return vRet;
}
#endif	// #ifdef __ENABLE_LINEAR_SAMPLE__


////////////////////////////////////////////////////
// �P�x���o

float fBrightThreshold = 0.5f;
float fBrightScale = 1.0f;

// ���Ȉ�
float4 mainBrighPass(S_PE_PS_IN sIn) : COLOR
{
	float4 vRet = tex2D(smplDownScale4x4, sIn.vUV);

	// 0�����ɂȂ�Ȃ��悤�ɂ���
	vRet.rgb = max(vRet.rgb - fBrightThreshold, 0.0f) * fBrightScale;

	return vRet;
}

////////////////////////////////////////////////////
// �k���o�b�t�@�փR�s�[�������Ƃɂ��G�C���A�V���O��h�����߂�
// �K�E�X�^�̂ڂ���������

float4 mainGaussBlur(S_PE_PS_IN_GAUSSBLUR_5x5 sIn) : COLOR
{
	float4 vRet = GaussBlur5x5(smplBright, sIn);
	return vRet;
}

////////////////////////////////////////////////////
// 1/2 x 1/2 �k���o�b�t�@�쐬

float4 mainDownScale2x2(S_PE_PS_IN sIn) : COLOR
{
	float4 vRet = DownScale2x2_Linear(smplStarSource, sIn);
	return vRet;
}

////////////////////////////////////////////////////
// �u���[���쐬

// �ŏ�
float4 mainRenderBloomInit(S_PE_PS_IN_GAUSSBLUR_5x5 sIn) : COLOR
{
	float4 vRet = GaussBlur5x5(smplBloomSource, sIn);
	return vRet;
}

// �r��
float4 mainRenderBloomIterate(S_PE_PS_IN_BLOOMBLUR_LINEAR sIn): COLOR
{
	float4 vRet = BloomBlur_Linear(smplBloomIterate, sIn);
	return vRet;
}

// �Ō�
float4 mainRenderBloomFinal(S_PE_PS_IN_BLOOMBLUR_LINEAR sIn): COLOR
{
	float4 vRet = BloomBlur_Linear(smplBloomFinal, sIn);
	return vRet;
}

////////////////////////////////////////////////////
// �X�^�[�쐬

float4 mainRenderStar(S_PE_PS_IN_STARBLUR sIn) : COLOR
{
	float4 vRet = StarBlur(smplStarSource, sIn);
	return vRet;
}

////////////////////////////////////////////////////
// �ŏI�`��

float fBloomWeight = 0.5f;
float fStarWeight = 0.8f;

float4 _FinalPass(
	in S_PE_PS_IN sIn,
	bool bEnableBloom,
	bool bEnableStar)
{
	float4 vRet = tex2D(smplScene, sIn.vUV);

	if (bEnableBloom) {
		vRet += tex2D(smplBloom, sIn.vUV) * fBloomWeight;
	}

	if (bEnableStar) {
		vRet += tex2D(smplStar, sIn.vUV) * fStarWeight;
	}

	return vRet;
}

float4 mainFinal(S_PE_PS_IN sIn) : COLOR
{
	float4 vRet = _FinalPass(sIn, true, true);
	return vRet;
}

float4 mainFinalWithoutBloom(S_PE_PS_IN sIn) : COLOR
{
	float4 vRet = _FinalPass(sIn, false, true);
	return vRet;
}

float4 mainFinalWithoutStar(S_PE_PS_IN sIn) : COLOR
{
	float4 vRet = _FinalPass(sIn, true, false);
	return vRet;
}

////////////////////////////////////////////////////
// �e�N�j�b�N

// �N���A�J���[
#define CLEAR_COLOR		(0)

technique t0
{
	// 1/4 x 1/4 �k���o�b�t�@�쐬
	pass passDownScale4x4
	<
		BeginScene(texDownScale4x4);
#ifdef __ENABLE_LINEAR_SAMPLE__
		SetTextureOffset(g_vDownScale4x4SampleOffset_Linear);
#else
		SetTextureOffset(g_vDownScale4x4SampleOffset_Point);
#endif
	>
	{
#ifdef __ENABLE_LINEAR_SAMPLE__
		SetVertexShader(vs_2_a, VSSampling_4);
#else
		SetVertexShader(vs_2_a, VSSampling_16);
#endif
		SetPixelShader(ps_2_a, mainDownScale4x4);
	}

	// �P�x���o
	pass passBright
	<
		BeginSceneWithClearColor(texBright, CLEAR_COLOR);
		SetScissorRectInflate(-1, -1);
	>
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainBrighPass);
	}

	// �K�E�X�u���[
	pass passGaussBlur
	<
		BeginSceneWithClearColor(texStarSource, CLEAR_COLOR);
		SetTextureOffset(g_vGaussBlur5x5SampleOffset);

		SetScissorRectInflate(-1, -1);
	>
	{
		SetVertexShader(vs_2_a, VSSampling_13);
		SetPixelShader(ps_2_a, mainGaussBlur);
	}

	// 1/2 x 1/2 �k���o�b�t�@�쐬
	pass passDownScale2x2
	<
		BeginSceneWithClearColor(texBloomSource, CLEAR_COLOR);

		SetSrcTexRectInflate(-1, -1);
		SetScissorRectInflate(-1, -1);
	>
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainDownScale2x2);
	}

	//--------------------------------------------
	// �u���[���쐬

	// �ŏ�
	pass passRenderBloomInit
	<
		BeginSceneWithClearColor(texBloomIterate, CLEAR_COLOR);
		SetTextureOffset(g_vGaussBlur5x5SampleOffset);

		SetSrcTexRectInflate(-1, -1);
		SetScissorRectInflate(-1, -1);
	>
	{
		SetVertexShader(vs_2_a, VSSampling_13);
		SetPixelShader(ps_2_a, mainRenderBloomInit);
	}

	// �r��
	pass passRenderBloomIterate
	<
		BeginSceneWithClearColor(texBloomSource, CLEAR_COLOR);
		SetTextureOffset(g_vBloomBlurSampleOffsetH_Linear);	// ���ڂ���

		SetSrcTexRectInflate(-1, -1);
		SetScissorRectInflate(-1, -1);
	>
	{
		SetVertexShader(vs_2_a, VSSampling_7);
		SetPixelShader(ps_2_a, mainRenderBloomIterate);
	}

	// �Ō�
	pass passRenderBloomFinal
	<
		BeginScene(texBloom);
		SetTextureOffset(g_vBloomBlurSampleOffsetV_Linear);	// �c�ڂ���

		SetSrcTexRectInflate(-1, -1);
	>
	{
		SetVertexShader(vs_2_a, VSSampling_7);
		SetPixelShader(ps_2_a, mainRenderBloomFinal);
	}

	//--------------------------------------------

	// �X�^�[�쐬
	pass passRenderStar
	<
		BeginSceneWithClearColor(texStar, CLEAR_COLOR);

		// �X�^�[�쐬�t�@���N�^
		SetFunctorName("PostEffectFunctorRenderStar");
	>
	{
		// �X�^�[�쐬�t�@���N�^���ŋ����I�ɉ��Z�u�����h���ݒ肳��Ă���B
		// ���̂��߁A�����ŃA���t�@�u�����h�̐ݒ肵�Ă����������B

		SetVertexShader(vs_2_a, VSSampling_8);
		SetPixelShader(ps_2_a, mainRenderStar);
	}

	// �ŏI�`��
	pass passFinal
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainFinal);
	}
}
