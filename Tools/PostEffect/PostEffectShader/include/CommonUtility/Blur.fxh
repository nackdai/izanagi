#if !defined(__IZANAGI_POSTEFFECT_BLUR_FXH__)
#define __IZANAGI_POSTEFFECT_BLUR_FXH__

/***********************************************
   �K�E�X�u���[
***********************************************/

#include "../PostEffectShaderIO.fxh"
#include "../PostEffectSamplingParam.fxh"
#include "../ShaderDefs/ShaderDefsInternal.fxh"

// �u���[�v�Z�i�����T���v�����O�j
#define _COMPUTE_EVEN_SAMPLING_BLUR_COLOR(vRet, smplIn, vUV, vWeight, NUM)\
	for (int i = 0; i < NUM; i++) {\
		vRet += vWeight[i * 2] * tex2D(smplIn, vUV[i].xy);\
		vRet += vWeight[i * 2 + 1] * tex2D(smplIn, vUV[i].zw);\
	}

// �u���[�v�Z�i��T���v�����O�j
#define _COMPUTE_ODD_SAMPLING_BLUR_COLOR(vRet, smplIn, vUV, vUV_0, vWeight, NUM)\
	{\
		for (int i = 0; i < NUM; i++) {\
			vRet += vWeight[i * 2] * tex2D(smplIn, vUV[i].xy);\
			vRet += vWeight[i * 2 + 1] * tex2D(smplIn, vUV[i].zw);\
		}\
		vRet += vWeight[NUM * 2] * tex2D(smplIn, vUV_0.xy);\
	}

////////////////////////////////////////////////////////////
// �K�E�X�u���[5x5

// ���S�ɋ߂��P�R�̃e�N�Z�����T���v�����O����

float4 GaussBlur5x5(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_13 sIn)
{
	// NOTE
	//         +---+
	//         | x |
	//     +---+---+---+
	//     | x | x | x |
	// +---+---+---+---+---+
	// | x | x | @ | x | x |
	// +---+---+---+---+---+
	//     | x | x | x |
	//     +---+---+---+
	//         | x |
	//         +---+

	// �E�G�C�g�l�ɂ���āA�d�݂̍��v���P�ɂȂ�悤�ɂȂ��Ă���

	float4 vRet = (float4)0.0f;

	_COMPUTE_ODD_SAMPLING_BLUR_COLOR(
		vRet,
		smplIn,
		sIn.vUV, sIn.vUV_0,
		g_vGaussBlur5x5Weight,
		NUM_SAMPLING_13);

	return vRet;
}

////////////////////////////////////////////////////////////
// �P�����K�E�X�u���[

// ���j�A�t�B���^
float4 GaussBlur1Direction_Linear(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_7 sIn)
{
	// NOTE
	// +---+---+---+---+---+---+---+---+---+---+---+---+---+
	// | x | x | x | x | x | x | @ | x | x | x | x | x | x |
	// +---+---+---+---+---+---+---+---+---+---+---+---+---+
	// |<----->|<----->|<----->|   |<----->|<----->|<----->|
	// ��̂悤�ɂQ�e�N�Z�����܂Ƃ߂�

	float4 vRet = (float4)0.0f;

	_COMPUTE_ODD_SAMPLING_BLUR_COLOR(
		vRet,
		smplIn,
		sIn.vUV, sIn.vUV_0,
		g_vBloomBlurWeight_Linear,
		NUM_SAMPLING_7);

	return vRet;
}

// �|�C���g�t�B���^
float4 GaussBlur1Direction_Point(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_13 sIn)
{
	// NOTE
	// +---+---+---+---+---+---+---+---+---+---+---+---+---+
	// | x | x | x | x | x | x | @ | x | x | x | x | x | x |
	// +---+---+---+---+---+---+---+---+---+---+---+---+---+

	float4 vRet = (float4)0.0f;

	_COMPUTE_ODD_SAMPLING_BLUR_COLOR(
		vRet,
		smplIn,
		sIn.vUV, sIn.vUV_0,
		g_vBloomBlurWeight_Point,
		NUM_SAMPLING_13);

	return vRet;
}

// �u���[���쐬�p�u���[
#define BloomBlur_Linear	GaussBlur1Direction_Linear
#define BloomBlur_Point		GaussBlur1Direction_Point

////////////////////////////////////////////////////////////
// �X�^�[�u���[

// �X�^�[�u���[�E�G�C�g�l
PRIVATE(float4 g_vStarWeight[8] : BLUR_WEIGHT);

// �X�^�[�}�[�W�E�G�C�g�l
PRIVATE(float g_fStarMergeWeight);

float4 StarBlur(
  in sampler smplIn,
  S_PE_PS_IN_SAMPLING_8 sIn)
{
	float4 vRet = (float4)0.0f;

	for (int i = 0; i < NUM_SAMPLING_8; i++) {
		vRet += g_vStarWeight[i * 2 + 0] * tex2D(smplIn, sIn.vUV[i].xy);
		vRet += g_vStarWeight[i * 2 + 1] * tex2D(smplIn, sIn.vUV[i].zw);
	}

	vRet *= g_fStarMergeWeight;

	return vRet;
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_BLUR_FXH__)
