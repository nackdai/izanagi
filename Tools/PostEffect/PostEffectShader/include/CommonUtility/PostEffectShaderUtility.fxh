#if !defined(__IZANAGI_POSTEFFECT_SHADER_UTILITY_FXH__)
#define __IZANAGI_POSTEFFECT_SHADER_UTILITY_FXH__

//////////////////////////////////////////////////
// 深度値変換

// Z値
float ConvertZ(float4 vIn)
{
#if defined(__Z_RGB__)
	// NOTE
	// RGB 24bit精度
#if 0
	return dot(vIn.rgb, float3(16711680.0f / 16777215.0f, 65280.0f / 16777215.0f, 255.0f / 16777215.0f));
#else
	return dot(vIn.rgb, float3(255.0f / 256.0f, 255.0f / (256.0f * 256.0f), 256.0f/ (256.0f * 256.0f * 256.0f)));
#endif
#elif defined(__Z_GBA__)
	// NOTE
	// GBA 24bit精度
	return dot(vIn.gba, float3(256.0f/ (256.0f * 256.0f * 256.0f), 255.0f / (256.0f * 256.0f), 255.0f / 256.0f));
#else
	// NOTE
	// 8bit精度 RGBそれぞれに同じ値が入っている
	return vIn.r;
#endif
}

// Z値
float ConvertZ(in sampler smplIn, float2 vUV)
{
	return ConvertZ(tex2D(smplIn, vUV));
}

// W値
float ConvertW(float4 vIn)
{
#if defined(__W_RGB__)
	// NOTE
	// RGB 24bit精度
#if 0
	return dot(vIn.rgb, float3(16711680.0f / 16777215.0f, 65280.0f / 16777215.0f, 255.0f / 16777215.0f));
#else
	return dot(vIn.rgb, float3(255.0f / 256.0f, 255.0f / (256.0f * 256.0f), 256.0f/ (256.0f * 256.0f * 256.0f)));
#endif
#else
	// NOTE
	// 8bit精度 RGBそれぞれに同じ値が入っている
	return vIn.r;
#endif
}

// W値
float ConvertW(in sampler smplIn, float2 vUV)
{
	return ConvertW(tex2D(smplIn, vUV));
}

//////////////////////////////////////////////////
// ピクセルシェーダ入力構造体ヘルパ

// 中心点取得
// ３ｘ３縮小（ポイントフィルタ）
float2 GetCenterUVFromDownScale3x3(S_PE_PS_IN_DOWNSCALE_3x3_POINT sIn)
{
	return sIn.vUV[2].xy;
}

// 中心点取得
// ９ポイントサンプリング（リニアフィルタ）
float2 GetCenterUVFrom9PointSample(S_PE_PS_IN_9POINTSAMPLE_LINEAR sIn)
{
	// NOTE
	// +---+---+---+
	// |   |   3   |
	// +-0-+---+---+
	// |   | 4 |   |
	// +---+---+-2-+
	// |   1   |   |
	// +---+---+---+

	return sIn.vUV_0;
}

// 中心点取得
// ９ポイントサンプリング（ポイントフィルタ）
float2 GetCenterUVFrom9PointSample(S_PE_PS_IN_9POINTSAMPLE_POINT sIn)
{
	// NOTE
	// +---+---+---+
	// | 0 | 1 | 2 |
	// +---+---+---+
	// | 3 | 4 | 5 |
	// +---+---+---+
	// | 6 | 7 | 8 |
	// +---+---+---+

	return GetCenterUVFromDownScale3x3(sIn);
}

// サンプリング点取得
// ４ｘ４縮小（リニアフィルタ）
float2 GetSamplePointFromDownScale4x4(int idx, S_PE_PS_IN_DOWNSCALE_4x4_LINEAR sIn)
{
	// NOTE
	// +---+---+---+---+
	// |   |   |   |   |
	// +---0---+---1---+
	// |   |   |   |   |
	// +---+---x---+---+
	// |   |   |   |   |
	// +---2---+---3---+
	// |   |   |   |   |
	// +---+---+---+---+

	if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// サンプリング点取得
// ４ｘ４縮小（ポイントフィルタ）
float2 GetSamplePointFromDownScale4x4(int idx, S_PE_PS_IN_DOWNSCALE_4x4_POINT sIn)
{
	// NOTE
	// +---+---+---+---+
	// | 0 | 1 | 2 | 3 |
	// +---+---+---+---+
	// | 4 | 5 | 6 | 7 |
	// +---+---x---+---+
	// | 8 | 9 | 10| 11|
	// +---+---+---+---+
	// | 12| 13| 14| 15|
	// +---+---+---+---+

	if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// サンプリング点取得
// ２ｘ２縮小（リニアフィルタ）
float2 GetSamplePointFromDownScale2x2(int idx, S_PE_PS_IN_DOWNSCALE_2x2_LINEAR sIn)
{
	return sIn.vUV;
}

// サンプリング点取得
// ２ｘ２縮小（ポイントフィルタ）
float2 GetSamplePointFromDownScale2x2(int idx, S_PE_PS_IN_DOWNSCALE_2x2_POINT sIn)
{
	// NOTE
	// +---+---+
	// | 0 | 1 |
	// +---x---+
	// | 2 | 3 |
	// +---+---+

	if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// サンプリング点取得
// ３ｘ３縮小（リニアフィルタ）
float2 GetSamplePointFromDownScale3x3(int idx, S_PE_PS_IN_DOWNSCALE_3x3_LINEAR sIn)
{
	// NOTE
	// +---+---+---+
	// |  0|   |1  |
	// +---+---+---+
	// |   | x |   |
	// +---+---+---+
	// |  2|   |3  |
	// +---+---+---+

	if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// サンプリング点取得
// ３ｘ３縮小（ポイントフィルタ）
float2 GetSamplePointFromDownScale3x3(int idx, S_PE_PS_IN_DOWNSCALE_3x3_POINT sIn)
{
	// NOTE
	// +---+---+---+
	// | 0 | 1 | 2 |
	// +---+---+---+
	// | 3 | 4 | 5 |
	// +---+---+---+
	// | 6 | 7 | 8 |
	// +---+---+---+

	if (idx == 8) {
		return sIn.vUV_0;
	}
	else if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// サンプリング点取得
// ９ポイントサンプリング（リニアフィルタ）
float2 GetSamplePointFrom9PointSample(int idx, S_PE_PS_IN_9POINTSAMPLE_LINEAR sIn)
{
	// NOTE
	// +---+---+---+
	// |   |   3   |
	// +-0-+---+---+
	// |   | 4 |   |
	// +---+---+-2-+
	// |   1   |   |
	// +---+---+---+

	if (idx == 4) {
		return sIn.vUV_0;
	}
	else if (idx % 2 == 0) {
		return sIn.vUV[idx / 2].xy;
	}
	else {
		return sIn.vUV[idx / 2].zw;
	}
}

// サンプリング点取得
// ９ポイントサンプリング（ポイントフィルタ）
float2 GetSamplePointFrom9PointSample(int idx, S_PE_PS_IN_9POINTSAMPLE_POINT sIn)
{
	// NOTE
	// +---+---+---+
	// | 0 | 1 | 2 |
	// +---+---+---+
	// | 3 | 4 | 5 |
	// +---+---+---+
	// | 6 | 7 | 8 |
	// +---+---+---+

	return GetSamplePointFromDownScale3x3(idx, sIn);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SHADER_UTILITY_FXH__)
