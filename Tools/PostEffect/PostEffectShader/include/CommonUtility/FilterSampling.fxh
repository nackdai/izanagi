#if !defined(__IZANAGI_POSTEFFECT_FILTER_SAMPLING_FXH__)
#define __IZANAGI_POSTEFFECT_FILTER_SAMPLING_FXH__

/***********************************************
   �t�B���^�T���v�����O
***********************************************/

#include "../PostEffectShaderIO.fxh"

//////////////////////////////////////////////////////////

// 16�{�b�N�X�t�B���^�T���v�����O
float4 Box16FilterSampling(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_4 sIn)
{
	// NOTE
	// +-------+-------+
	// |   |   |   |   |
	// +---@---+---@---+
	// |   |   |   |   |
	// +---+---+---+---+
	// |   |   |   |   |
	// +---@-------@---+
	// |   |   |   |   |
	// +---+---+---+---+

	float4 vRet = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_4; i++) {
		vRet += tex2D(smplIn, sIn.vUV[i].xy);
		vRet += tex2D(smplIn, sIn.vUV[i].zw);
	}

	// /= 4
	vRet *= 0.25f;

	return vRet;
}

// 16�|�C���g�t�B���^�T���v�����O
float4 Point16FilterSampling(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_16 sIn)
{
	// NOTE
	// +-------+-------+
	// | @ | @ | @ | @ |
	// +---+---+---+---+
	// | @ | @ | @ | @ |
	// +---+---+---+---+
	// | @ | @ | @ | @ |
	// +---+-------+---+
	// | @ | @ | @ | @ |
	// +---+---+---+---+

	float4 vRet = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_16; i++) {
		vRet += tex2D(smplIn,  sIn.vUV[i].xy);
		vRet += tex2D(smplIn,  sIn.vUV[i].zw);
	}

	// /= 16
	vRet *= 0.0625f;

	return vRet;
}

// 4�{�b�N�X�t�B���^�T���v�����O
float4 Box4FilterSampling(
	in sampler smplIn,
	S_PE_PS_IN sIn)
{
	// NOTE
	// +-------+
	// |   |   |
	// +---@---+
	// |   |   |
	// +---+---+

	return tex2D(smplIn, sIn.vUV);
}

// 4�|�C���g�t�B���^�T���v�����O
float4 Point4FilterSampling(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_4 sIn)
{
	// NOTE
	// +-------+
	// | @ | @ |
	// +---+---+
	// | @ | @ |
	// +---+---+

	float4 vRet = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_4; i++) {
		vRet += tex2D(smplIn, sIn.vUV[i].xy);
		vRet += tex2D(smplIn, sIn.vUV[i].zw);
	}

	// /= 4
	vRet *= 0.25f;

	return vRet;
}

// 9�R�[���t�B���^�T���v�����O
float4 Cone9FilterSampling(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_4 sIn)
{
	// NOTE
	// +---+---+---+      +---+---+---+
	// |   |   |   |      | 1 | 2 | 1 |
	// +---@---@---+      +---+---+---+
	// |   |   |   |  ->  | 2 | 4 | 2 |
	// +---@---@---+      +---+---+---+
	// |   |   |   |      | 1 | 2 | 1 |
	// +---+---+---+      +---+---+---+
	// �T���v�����O�ʒu   �e�s�N�Z���̏d��
	//
	// ���S�̏d�݂��������ӂ��Ⴍ�Ȃ��Ă��邱�Ƃ���9�R�[���t�B���^�T���v�����O�ƌ�����

	float4 vRet = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_4; i++) {
		vRet += tex2D(smplIn, sIn.vUV[i].xy);
		vRet += tex2D(smplIn, sIn.vUV[i].zw);
	}

	// /= 16
	// NOTE : 16 = 1 + 2 + 1 + 2 + 4 + 2 + 1 + 2 + 1
	vRet *= 0.0625f;

	return vRet;
}

// 9�{�b�N�X�t�B���^�T���v�����O
// �{�b�N�X�ƌ�����̂��ȁE�E�E
float4 Box9FilterSampling(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_4 sIn)
{
	// NOTE
	// +---+---+---+     +-------+--
	// | +-+---+---+---> |       |
	// +---+---+---+     |   x   |
	// |   |   |   |     |      @|
	// +---+---+---+     +-------+--
	// |   |   |   |     |       |
	// +---+---+---+

	// NOTE
	//             |
	//         @---+--
	//         |   | |<-1/6
	// --------+---+--
	//         |<->|
	//          1/6

	// NOTE
	// +---+---+---+
	// |  @|   |@  |
	// +---+---+---+
	// |   | x |   |
	// +---+---+---+
	// |  @|   |@  |
	// +---+---+---+

	float4 vRet = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_4; i++) {
		vRet += tex2D(smplIn, sIn.vUV[i].xy);
		vRet += tex2D(smplIn, sIn.vUV[i].zw);
	}

	// /= 4
	vRet *= 0.25f;

	return vRet;
}

// 9�|�C���g�t�B���^�T���v�����O
float4 Point9FilterSampling(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_9 sIn)
{
	// NOTE
	// +---+---+---+
	// | @ | @ | @ |
	// +---+---+---+
	// | @ | @ | @ |
	// +---+---+---+
	// | @ | @ | @ |
	// +---+---+---+

	float4 vRet = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_9; i++) {
		vRet += tex2D(smplIn, sIn.vUV[i].xy);
		vRet += tex2D(smplIn, sIn.vUV[i].zw);
	}

	vRet += tex2D(smplIn, sIn.vUV_0);

	// /= 9
	vRet /= 9;

	return vRet;
}

//////////////////////////////////////////////////////////

#define DownScale4x4_Linear		Box16FilterSampling		// 1/4 x 1/4 �k���i���j�A�t�B���^�j
#define DownScale4x4_Point		Point16FilterSampling	// 1/4 x 1/4 �k���i�|�C���g�t�B���^�j
#define DownScale2x2_Linear		Box4FilterSampling		// 1/2 x 1/2 �k���i���j�A�t�B���^�j
#define DownScale2x2_Point		Point4FilterSampling	// 1/2 x 1/2 �k���i�|�C���g�t�B���^�j
#define DownScale3x3_Linear		Box9FilterSampling		// 1/3 x 1/3 �k���i���j�A�t�B���^�j
#define DownScale3x3_Point		Point9FilterSampling	// 1/3 x 1/3 �k���i�|�C���g�t�B���^�j

#endif	// #if !defined(__IZANAGI_POSTEFFECT_FILTER_SAMPLING_FXH__)
