#if !defined(__IZANAGI_POSTEFFECT_FILTER_SAMPLING_FXH__)
#define __IZANAGI_POSTEFFECT_FILTER_SAMPLING_FXH__

/***********************************************
   フィルタサンプリング
***********************************************/

#include "../PostEffectShaderIO.fxh"

//////////////////////////////////////////////////////////

// 16ボックスフィルタサンプリング
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

// 16ポイントフィルタサンプリング
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

// 4ボックスフィルタサンプリング
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

// 4ポイントフィルタサンプリング
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

// 9コーンフィルタサンプリング
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
	// サンプリング位置   各ピクセルの重み
	//
	// 中心の重みが高く周辺が低くなっていることから9コーンフィルタサンプリングと言われる

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

// 9ボックスフィルタサンプリング
// ボックスと言えるのかな・・・
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

// 9ポイントフィルタサンプリング
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

#define DownScale4x4_Linear		Box16FilterSampling		// 1/4 x 1/4 縮小（リニアフィルタ）
#define DownScale4x4_Point		Point16FilterSampling	// 1/4 x 1/4 縮小（ポイントフィルタ）
#define DownScale2x2_Linear		Box4FilterSampling		// 1/2 x 1/2 縮小（リニアフィルタ）
#define DownScale2x2_Point		Point4FilterSampling	// 1/2 x 1/2 縮小（ポイントフィルタ）
#define DownScale3x3_Linear		Box9FilterSampling		// 1/3 x 1/3 縮小（リニアフィルタ）
#define DownScale3x3_Point		Point9FilterSampling	// 1/3 x 1/3 縮小（ポイントフィルタ）

#endif	// #if !defined(__IZANAGI_POSTEFFECT_FILTER_SAMPLING_FXH__)
