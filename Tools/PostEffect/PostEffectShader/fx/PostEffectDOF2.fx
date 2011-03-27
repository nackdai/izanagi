// �|�X�g�G�t�F�N�g�@��ʊE�[�x

// ATI�̃T���v�����x�[�X�ɂ�������
// �������E�E�E

#include "PostEffectShader.fxh"

PES_ID("POSTEFFECT_DOF");

////////////////////////////////////////////////////
 
// �V�[���e�N�X�`��
texture texScene : INPUT_SCENE
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);
>;

// 1/4 x 1/4 �k���o�b�t�@
texture texDownScale4x4
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.25f, 0.25f);	// 1/4
>;

// �u���[�e�N�X�`��
texture texBlur
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);

	SetTexRatio(0.25f, 0.25f);	// 1/4
>;

// �O������------------------------------

// Z�l�e�N�X�`��
texture texZ : INPUT
<
	SetTexFormat(PIXEL_FORMAT_RGBA8);
>;

// �E�G�C�g�e�N�X�`��
texture texWeight : INPUT
<
	SetTexFormat(PIXEL_FORMAT_A8);
	SetTexSize(512, 1);
>;

////////////////////////////////////////////////////

// �V�[��
sampler smplScene = sampler_state
{
	BindTex(texScene);

#ifdef __DOF_POINT__
	SetMinFilter(SMPL_FILTER_POINT);
	SetMagFilter(SMPL_FILTER_POINT);
#else
	SetMinFilter(SMPL_FILTER_LINEAR);
	SetMagFilter(SMPL_FILTER_LINEAR);
#endif
};

// �E�G�C�g
sampler smplWeight = sampler_state
{
	BindTex(texWeight);
};

// Z�l
sampler smplZ = sampler_state
{
	BindTex(texZ);
};

// 1/4 x 1/4 �k��
sampler smplDownScale4x4 = sampler_state
{
	BindTex(texDownScale4x4);
};

// �u���[
sampler smplBlur = sampler_state
{
	BindTex(texBlur);
};

////////////////////////////////////////////////////

// ���_���璍���_�܂ł̋���
float fEyeToAtLength : CAMERA_PARAM_EYE_TO_AT_LENGTH
<
	SetDoNotStrip(true);
>;

// �J�����i��
float fAperture : CAMERA_PARAM_APERTURE
<
	SetDoNotStrip(true);
>;

// �J������p
float fFov : CAMERA_PARAM_FOV
<
	SetDoNotStrip(true);
>;


// �J����Near
float fNear : CAMERA_PARAM_NEAR
<
	SetDoNotStrip(true);
>;


// �J����Far
float fFar : CAMERA_PARAM_FAR
<
	SetDoNotStrip(true);
>;

////////////////////////////////////////////////////
// �}�[�W

// NOTE
// �J���[�o�b�t�@��A�����݂̂ւ̕`�����ݐݒ�ɂ��Ă�������

float4 mainMerge(S_PE_PS_IN sIn) : COLOR
{
	float fZ = ConvertZ(smplZ, sIn.vUV);
	float fWeight = 1.0f - tex2D(smplWeight, float2(fZ, 0.5f)).a;
	return float4(fWeight, fWeight, fWeight, fWeight);
}

////////////////////////////////////////////////////
// 1/4 x 1/4 �k��

#ifdef __DOF_POINT__
float4 mainDownScale4x4(S_PE_PS_IN_DOWNSCALE_4x4_POINT sIn) : COLOR
{
	float4 vRet = DownScale4x4_Point(smplScene, sIn);
	return vRet;
}
#else	// #ifdef __DOF_POINT__
float4 mainDownScale4x4(S_PE_PS_IN_DOWNSCALE_4x4_LINEAR sIn) : COLOR
{
	float4 vRet = DownScale4x4_Linear(smplScene, sIn);
	return vRet;
}
#endif	// #ifdef __ENABLE_LINEAR_SAMPLE__

////////////////////////////////////////////////////
// 5 x 5�K�E�X�u���[

float4 mainBlur(S_PE_PS_IN_SAMPLING_13 sIn) : COLOR
{
	float4 vRet = GaussBlur5x5(smplDownScale4x4, sIn);
	return vRet;
}

////////////////////////////////////////////////////
// �ŏI�`��

#define NUM_POISSON_TAPS	8
static const float2 vPoisson[NUM_POISSON_TAPS] = 
{
	{ 0.000000f, 0.000000f},
	{ 0.527837f,-0.085868f},
	{-0.040088f, 0.536087f},
	{-0.670445f,-0.179949f},
	{-0.419418f,-0.616039f},
	{ 0.440453f,-0.639399f},
	{-0.757088f, 0.349334f},
	{ 0.574619f, 0.685879f},
};

float2 vTexelSizeHigh = { 1.0f / 1280.0f, 1.0f / 720.0f };
float2 vTexelSizeLow = { 1.0f / 320.0f, 1.0f / 180.0f };

float fRadiusScale = 0.25f;

float fCutOff = 10.0f;

float4 mainFinal(S_PE_PS_IN sIn) : COLOR
{
	// ���S
	float4 vOut = tex2D(smplScene, sIn.vUV);
	float fCenterWeight = vOut.a;

	vOut = 0.0f;

	// �{�P�T�C�Y
	float fDiscRadiusHigh = min(fCutOff, 1.0f / (1.0f - fCenterWeight + 0.00001f));
	float fDiscRadiusLow = fDiscRadiusHigh * fRadiusScale;

	for (int i = 0; i < NUM_POISSON_TAPS; i++) {
		float2 vCoordHigh = sIn.vUV + vTexelSizeHigh * vPoisson[i] * fDiscRadiusHigh;
		float2 vCoordLow = sIn.vUV + vTexelSizeLow * vPoisson[i] * fDiscRadiusLow;

		float4 vHigh = tex2D(smplScene, vCoordHigh);
		float4 vLow = tex2D(smplBlur, vCoordLow);

		float4 vTap = lerp(vHigh, vLow, vHigh.a);

		vTap.a = (vTap.a >= fCenterWeight ? 1.0f : vTap.a);

		vOut.rgb += vTap.rgb * vTap.a;
		vOut.a += vTap.a;
	}

	return vOut / vOut.a;
}

float4 mainFinalDebug(S_PE_PS_IN sIn) : COLOR
{
#if 0
	float fOut = 1.0f - saturate(ConvertW(smplW, sIn.vUV) * 10.0f);
	return float4(fOut, fOut, fOut, fOut);
#else
	float fOut = 1.0f - tex2D(smplScene, sIn.vUV).a;
	//fOut = saturate(1.0f / (fOut + 0.00001f));
	return float4(fOut, fOut, fOut, fOut);
#endif
}

////////////////////////////////////////////////////

// ���ʏ���
DEFINE_PASSES(CommonPasses)
{
	// ����
	pass passMerge
	<
		BeginScene(texScene);
		SetFunctorName("PostEffectFunctorDOFMerge");
	>
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainMerge);
	}

	// 1/4 x 1/4 �k��
	pass passDownScale4x4
	<
		BeginScene(texDownScale4x4);
#ifdef __DOF_POINT__
		SetTextureOffset(g_vDownScale4x4SampleOffset_Point);
#else
		SetTextureOffset(g_vDownScale4x4SampleOffset_Linear);
#endif
	>
	{
#ifdef __DOF_POINT__
		SetVertexShader(vs_2_a, VSSampling_16);
#else
		SetVertexShader(vs_2_a, VSSampling_4);
#endif
		SetPixelShader(ps_2_a, mainDownScale4x4);
	}

	// �u���[
	pass passBlur
	<
		BeginScene(texBlur);
		SetTextureOffset(g_vGaussBlur5x5SampleOffset);
	>
	{
		SetVertexShader(vs_2_a, VSSampling_13);
		SetPixelShader(ps_2_a, mainBlur);
	}
}

////////////////////////////////////////////////////

technique t0
{	
	EmbedDefinePasses(CommonPasses)

	// �ŏI�`��
	pass passFinal
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainFinal);
	}
}

// �f�o�b�O�p
technique t1
{
	// ����
	pass passMerge
	<
		BeginScene(texScene);
		SetFunctorName("PostEffectFunctorDOFMerge");
	>
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainMerge);
	}

	// �ŏI�`��
	pass passFinalDebug
	{
		SetVertexShader(vs_2_a, VSSampling_1);
		SetPixelShader(ps_2_a, mainFinalDebug);
	}
}
