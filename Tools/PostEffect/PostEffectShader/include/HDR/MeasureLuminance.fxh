#if !defined(__IZANAGI_POSTEFFECT_MEASURE_LUMINANCE_FXH__)
#define __IZANAGI_POSTEFFECT_MEASURE_LUMINANCE_FXH__

#include "../ColorSpace/ColorSpaceUtil.fxh"
#include "../CommonUtility/FilterSampling.fxh"

/////////////////////////////////////////////
// Reinherd 輝度測定

// NOTE
// Reinherd の平均輝度計算
// Lavg =  exp(1/N * Σlog(δ + L(x, y)))

// NOTE
// W x H -> 64 x 64 -> 16 x 16 -> 4 x 4 -> 1 x 1
// ・Initial   : W x H -> 64 x 64
// ・Iterative : 64 x 64 -> 16 x 16
//               16 x 16 -> 4 x 4
// ・Final     : 4 x 4 -> 1 x 1

//---------------------------
// 最初

// ポイントフィルタ
float4 ReinherdMeasureLuminanceInitial_Point(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_9 sIn)
{
	// NOTE
	// ここでは以下の計算を行う
	// 1/N * Σlog(δ + L(x, y))

	float4 vSample = 0.0f;
	float fLogLumSum = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_9; i++) {
		vSample = tex2D(smplIn, sIn.vUV[i].xy);
		fLogLumSum += log(dot(vSample.rgb, RGB2Y) + 0.00001f);

		vSample = tex2D(smplIn, sIn.vUV[i].zw);
		fLogLumSum += log(dot(vSample.rgb, RGB2Y) + 0.00001f);
	}

	vSample = tex2D(smplIn, sIn.vUV_0);
	fLogLumSum += log(dot(vSample.rgb, RGB2Y) + 0.00001f);

	fLogLumSum /= 9;

	return float4(fLogLumSum, fLogLumSum, fLogLumSum, 1.0f);
}

// リニアフィルタ
float4 ReinherdMeasureLuminanceInitial_Linear(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_4 sIn)
{
	// NOTE
	// ここでは以下の計算を行う
	// 1/N * Σlog(δ + L(x, y))

	// NOTE
	// +---+---+---+
	// |  @|   |@  |
	// +---+---+---+
	// |   | x |   |
	// +---+---+---+
	// |  @|   |@  |
	// +---+---+---+

	float3 vSample = 0.0f;
	float fLogLumSum = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_4; i++) {
		vSample = tex2D(smplIn, sIn.vUV[i].xy).rgb;
		fLogLumSum += log(dot(vSample, RGB2Y) + 0.00001f);

		vSample = tex2D(smplIn, sIn.vUV[i].zw).rgb;
		fLogLumSum += log(dot(vSample, RGB2Y) + 0.00001f);
	}

	fLogLumSum /= 4;

	return float4(fLogLumSum, fLogLumSum, fLogLumSum, 1.0f);
}

//---------------------------
// 途中

// ポイントフィルタ
float4 ReinherdMeasureLuminanceIterative_Point(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_16 sIn)
{
	// 1/4 x 1/4 縮小をするだけ
	return DownScale4x4_Point(smplIn, sIn);
}

// リニアフィルタ
float4 ReinherdMeasureLuminanceIterative_Linear(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_4 sIn)
{
	// 1/4 x 1/4 縮小をするだけ
	return DownScale4x4_Linear(smplIn, sIn);
}

//---------------------------
// 最後

// ポイントフィルタ
float4 ReinherdMeasureLuminanceFinal_Point(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_16 sIn)
{
	// NOTE
	// 1/4 x 1/4 縮小しつつ以下の計算を行う
	// Lavg = exp(sample)
	// sample = 1/N * Σlog(δ + L(x, y))

	float fRet = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_16; i++) {
		fRet += tex2D(smplIn, sIn.vUV[i].xy).r;
		fRet += tex2D(smplIn, sIn.vUV[i].zw).r;
	}

	fRet = exp(fRet / 16);

	return float4(fRet, fRet, fRet, 1.0f);
}

// リニアフィルタ
float4 ReinherdMeasureLuminanceFinal_Linear(
	in sampler smplIn,
	S_PE_PS_IN_SAMPLING_4 sIn)
{
	// NOTE
	// 1/4 x 1/4 縮小しつつ以下の計算を行う
	// Lavg = exp(sample)
	// sample = 1/N * Σlog(δ + L(x, y))

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

	float fRet = 0.0f;

	for (int i = 0; i < NUM_SAMPLING_4; i++) {
		fRet += tex2D(smplIn, sIn.vUV[i].xy).r;
		fRet += tex2D(smplIn, sIn.vUV[i].zw).r;
	}

	fRet = exp(fRet / 4);

	return float4(fRet, fRet, fRet, 1.0f);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_MEASURE_LUMINANCE_FXH__)
