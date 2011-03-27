// �|�X�g�G�t�F�N�g�@�g�c�q

#include "PostEffectShader.fxh"

PES_ID("POSTEFFECT_HDR");

////////////////////////////////////////////////////
// �e�N�X�`��

#define BASE_TEX_FMT	PIXEL_FORMAT_RGBA16F
//#define BASE_TEX_FMT	PIXEL_FORMAT_RGBA8

#define LUM_TEX_FMT		PIXEL_FORAMT_R32F

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

// �P�x����o�b�t�@
texture texMeasureLumInit
<
	SetTexFormat(LUM_TEX_FMT);
	SetTexSize(64, 64);
>;
texture texMeasureLumIterate_0
<
	SetTexFormat(LUM_TEX_FMT);
	SetTexSize(16, 16);
>;
texture texMeasureLumIterate_1
<
	SetTexFormat(LUM_TEX_FMT);
	SetTexSize(4, 4);
>;
texture texToneMap
<
	SetTexFormat(LUM_TEX_FMT);
	SetTexSize(1, 1);
>;

// ������
texture texAdaptedLumCur : PRIVATE
<
	SetTexFormat(LUM_TEX_FMT);
	SetTexSize(1, 1);
>;
texture texAdaptedLumLast : PRIVATE
<
	SetTexFormat(LUM_TEX_FMT);
	SetTexSize(1, 1);
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

// �P�x����o�b�t�@
sampler smplMeasureLumInit = sampler_state
{
	BindTex(texMeasureLumInit);
	SAMPLER_FILTER();
};
sampler smplMeasureLumIterate_0 = sampler_state
{
	BindTex(texMeasureLumIterate_0);
	SAMPLER_FILTER();
};
sampler smplMeasureLumIterate_1 = sampler_state
{
	BindTex(texMeasureLumIterate_1);
	SAMPLER_FILTER();
};
sampler smplToneMap = sampler_state
{
	BindTex(texToneMap);
	SAMPLER_FILTER();
};

// ������
sampler smplAdaptedLumCur = sampler_state
{
	BindTex(texAdaptedLumCur);
	SAMPLER_FILTER();
};
sampler smplAdaptedLumLast = sampler_state
{
	BindTex(texAdaptedLumLast);
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
// �P�x����

#ifdef __ENABLE_LINEAR_SAMPLE__

// �ŏ�
float4 mainMeasureLuminanceInitial(S_PE_PS_IN_SAMPLING_4 sIn) : COLOR
{
	float4 vRet = ReinherdMeasureLuminanceInitial_Linear(smplDownScale4x4, sIn);
	return vRet;
}

// �r��
float4 mainMeasureLuminanceIterate_0(S_PE_PS_IN_SAMPLING_4 sIn) : COLOR
{
	float4 vRet = ReinherdMeasureLuminanceIterative_Linear(smplMeasureLumInit, sIn);
	return vRet;
}

// �r��
float4 mainMeasureLuminanceIterate_1(S_PE_PS_IN_SAMPLING_4 sIn) : COLOR
{
	float4 vRet = ReinherdMeasureLuminanceIterative_Linear(smplMeasureLumIterate_0, sIn);
	return vRet;
}

// �Ō�
float4 mainMeasureLuminanceFinal(S_PE_PS_IN_SAMPLING_4 sIn) : COLOR
{
	float4 vRet = ReinherdMeasureLuminanceFinal_Linear(smplMeasureLumIterate_1, sIn);
	return vRet;
}

#else	// #ifdef __ENABLE_LINEAR_SAMPLE__

// �ŏ�
float4 mainMeasureLuminanceInitial(S_PE_PS_IN_SAMPLING_9 sIn) : COLOR
{
	float4 vRet = ReinherdMeasureLuminanceInitial_Point(smplDownScale4x4, sIn);
	return vRet;
}

// �r��
float4 mainMeasureLuminanceIterate_0(S_PE_PS_IN_SAMPLING_16 sIn) : COLOR
{
	float4 vRet = ReinherdMeasureLuminanceIterative_Point(smplMeasureLumInit, sIn);
	return vRet;
}

// �r��
float4 mainMeasureLuminanceIterate_1(S_PE_PS_IN_SAMPLING_16 sIn) : COLOR
{
	float4 vRet = ReinherdMeasureLuminanceIterative_Point(smplMeasureLumIterate_0, sIn);
	return vRet;
}

// �Ō�
float4 mainMeasureLuminanceFinal(S_PE_PS_IN_SAMPLING_16 sIn) : COLOR
{
	float4 vRet = ReinherdMeasureLuminanceFinal_Point(smplMeasureLumIterate_1, sIn);
	return vRet;
}

#endif	// #ifdef __ENABLE_LINEAR_SAMPLE__

////////////////////////////////////////////////////
// ������

float fInitAdaptedColor = 0.5f;
float fAdaptedRate = 0.02f;

// �ŏ��̂P�񂾂��N���A
float4 mainAdaptedInit(S_PE_PS_IN sIn) : COLOR
{
	return float4(fInitAdaptedColor, fInitAdaptedColor, fInitAdaptedColor, 1.0f);
}

// ������
float4 mainAdaptedLum(S_PE_PS_IN sIn) : COLOR
{
	// NOTE
	// 1x1 �e�N�X�`���̒��S���T���v�����O
	float fAdaptedLum = tex2D(smplAdaptedLumCur, float2(0.5f, 0.5f)).r;
    float fCurrentLum = tex2D(smplToneMap, float2(0.5f, 0.5f)).r;
    
	// NOTE
	// AdaptedLum��CurrentLum�̊Ԃ̍��𖈃t���[���w�肳�ꂽ�����߂Â���悤�ɓK�������Ă���B
	// ���Ȃ݂ɁA�l�Ԃ̖ڂ̓K�����f���Ƃ͈�؊֌W�Ȃ��B
    float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * fAdaptedRate;
    return float4(fNewAdaptation, fNewAdaptation, fNewAdaptation, 1.0f);
}

////////////////////////////////////////////////////
// �P�x���o

float fMiddleGrey = 0.18f;
float fThreshold = 0.25f;
float fBrightPassOffset = 10.0f;

float4 mainBrighPass(S_PE_PS_IN sIn) : COLOR
{
	float4 vSample = tex2D(smplDownScale4x4, sIn.vUV);
	float fAdaptedLum = tex2D(smplAdaptedLumLast, float2(0.5f, 0.5f)).r;

	// NOTE
	// DirectX9�̃T���v�����̂܂܁E�E�E

	// Determine what the pixel's value will be after tone-mapping occurs
	vSample.rgb *= fMiddleGrey / (fAdaptedLum + 0.00001f);
	
	// Subtract out dark pixels
	vSample.rgb -= fThreshold;
	
	// Clamp to 0
	vSample = max(vSample, 0.0f);
	
	// Map the resulting value into the 0 to 1 range. Higher values for
	// BRIGHT_PASS_OFFSET will isolate lights from illuminated scene objects.
	vSample.rgb /= (fBrightPassOffset + vSample.rgb);

	return vSample;
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
	bool bEnableStar,
	bool bEnableToneMap)
{
	float4 vRet = tex2D(smplScene, sIn.vUV);

	if (bEnableToneMap) {
		float fAdaptedLum = tex2D(smplAdaptedLumLast, float2(0.5f, 0.5f)).r;
		vRet.rgb *= fMiddleGrey / (fAdaptedLum + 0.00001f);
		vRet.rgb /= (1.0f + vRet.rgb);
	}

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
	float4 vRet = _FinalPass(sIn, true, true, true);
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

	////////////////////////////////////////////////////
	// �P�x����

	// �ŏ�
	pass passMeasureLuminanceInit
	<
		BeginScene(texMeasureLumInit);
#ifdef __ENABLE_LINEAR_SAMPLE__
		SetTextureOffset(g_vDownScale3x3SampleOffset_Linear);
#else
		SetTextureOffset(g_vDownScale3x3SampleOffset_Point);
#endif
	>
	{
#ifdef __ENABLE_LINEAR_SAMPLE__
		SetVertexShader(vs_2_a, VSSampling_4);
#else
		SetVertexShader(vs_2_a, VSSampling_9);
#endif
		SetPixelShader(ps_2_a, mainMeasureLuminanceInitial);
	}

	// �r��
	pass passMeasureLuminanceIterate_0
	<
		BeginScene(texMeasureLumIterate_0);
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
		SetPixelShader(ps_2_a, mainMeasureLuminanceIterate_0);
	}

	// �r��
	pass passMeasureLuminanceIterate_1
	<
		BeginScene(texMeasureLumIterate_1);
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
		SetPixelShader(ps_2_a, mainMeasureLuminanceIterate_1);
	}

	// �Ō�
	pass passMeasureLuminanceFinal
	<
		BeginScene(texToneMap);
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
		SetPixelShader(ps_2_a, mainMeasureLuminanceFinal);
	}

	////////////////////////////////////////////////////

	////////////////////////////////////////////////////
	// ������

	// �ŏ��̂P�񂾂��N���A
	pass passAdaptedInit
	<
		BeginScene(texAdaptedLumCur);

		// ��x�����Ă΂�Ȃ�
		SetFunctorName("PostEffectFunctorOnce");
	>
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainAdaptedInit);
	}

	// ������
	pass passAdaptedLum
	<
		BeginScene(texAdaptedLumLast);

		// �e�N�X�`���؂�ւ�
		SetFunctorName("PostEffectFunctorSwitchTex");
		SetFunctorArgsS_2(
			texAdaptedLumCur,
			texAdaptedLumLast);
	>
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainAdaptedLum);
	}

	////////////////////////////////////////////////////

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

	////////////////////////////////////////////////////
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

	////////////////////////////////////////////////////

	// �X�^�[�쐬
	pass passRenderStar
	<
		BeginSceneWithClearColor(texStar, CLEAR_COLOR);

		// �X�^�[�쐬�t�@���N�^
		SetFunctorName("PostEffectFunctorRenderStar");
	>
	{
		// �X�^�[�쐬�t�@���N�^���ŋ����I�ɉ��Z�u�����h���ݒ肳��Ă���B
		// ���̂��߁A�����ŃA���t�@�u�����h�̐ݒ�����Ă����������B

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
