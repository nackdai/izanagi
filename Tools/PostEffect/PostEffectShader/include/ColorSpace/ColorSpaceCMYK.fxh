#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_CMYK_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_CMYK_FXH__

//////////////////////////////////
// CMYK

// NOTE
// 理論上、CMYをすべて均等に混ぜると黒色になるが、
// インクや紙の特性上、CMYのインクを混ぜて綺麗な黒色を作るのは技術的に困難であり、
// 通常はすべてを混ぜても濁った茶色にしかならない。
// そこで、黒（Key plate）の発色をよくするために別途黒インクを用いるようになったのがCMYKである。

// NOTE
// K = min(min(1 - R, 1 - G), 1 - B)
// C = (1 - R - K) / (1 - K)
// M = (1 - G - K) / (1 - K)
// Y = (1 - B - K) / (1 - K)
//
// R = 1 - min(1, C * (1 - K) + K)
// G = 1 - min(1, M * (1 - K) + K)
// B = 1 - min(1, Y * (1 - K) + K)

// NOTE
// K = min(min(C, M), Y)
// C' = (C - K) / (1 - K)
// M' = (M - K) / (1 - K)
// Y' = (Y - K) / (1 - K)
//
// C = C' * (1 - K) + K
// M = M' * (1 - K) + K
// Y = Y' * (1 - K) + K

// NOTE
// C - K -> K基準になるように変換？
// (C - K) / (1 - K) -> (1 - K)で割ることで 0 - 1 に変換？

// CMY -> CMYK
float4 CMYToCMYK(float3 vCMY)
{
	float4 vCMYK = (float4)1.0f;

	// K
	vCMYK.a = min(min(vCMY.r, vCMY.g), vCMY.b);

	// CMY
	vCMYK.rgb = min(1.0f, (vCMY.rgb - vCMYK.a) / (1.0f - vCMYK.a + 0.00001f));

	return vCMYK;
}

// CMYK -> CMY
float3 CMYKToCMY(float4 vCMYK)
{
	// NOTE
	// C = C' * (1 - K) + K
	// M = M' * (1 - K) + K
	// Y = Y' * (1 - K) + K

	return float3(vCMYK.rgb * (1.0f - vCMYK.a) + vCMYK.a);
}

// RGB -> CMYK
float4 RGBToCMYK(float3 vRGB)
{
	// RGB -> CMY -> CMYK
	return CMYToCMYK(RGBToCMY(vRGB));
}

// CMYK -> RGB
float3 CMYKToRGB(float4 vCMYK)
{
	// CMYK -> CMY -> RGB
	return CMYToRGB(CMYKToCMY(vCMYK));
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_CMYK_FXH__)
