#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_YCbCr_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_YCbCr_FXH__

#include "ColorSpaceUtil.fxh"

//////////////////////////////////
// YCbCr

// NOTE
// Y は輝度信号（明るさ）
// Cb は青の差分信号 (色差信号)
// Cr は赤の差分信号 (色差信号)

// NOTE
//  0.00000 <= Y <= 1.00000
// -0.70109 <= U <= 0.70101
// -0.88552 <= V <= 0.88552
// Yの値の範囲の幅が1.0であるが、Uは1.40218、Vは1.77104となり、信号振幅が揃っていない。
// これをそろえるようにスケーリングしたものがYCbCrになる。
//
//  0.00000 <= Y  <= 1.00000
// -0.50000 <= Cb <= 0.50000
// -0.50000 <= Cr <= 0.50000
// Y、Cb、Crともに値の範囲の幅が1.0になる。

// NOTE
// Y  =  0.29891 * R + 0.58661 * G + 0.11448 * B
// Cb = V / (0.88552 * 2) = V / 1.77104 = 0.56464 * V = 0.56464 * (B - Y)
//    = -0.16878 * R - 0.33122 * G + 0.50000 * B
// Cr = U / (0.70109 * 2) = U / 1.40218 = 0.71318 * U = 0.71318 * (R - Y)
//    =  0.50000 * R - 0.41836 * G - 0.08164 * B
//
// R = Y + U = Y + 1.40218 * Cr
// G = Y - 0.50955 * U - 0.19516 * V = Y - 0.50955 * 1.40218 * Cr - 0.19516 * 1.77104 * Cb
// B = Y + V = Y + 1.77104 * Cb
//
// R = Y                + 1.40218 * Cr
// G = Y - 0.34564 * Cb - 0.71448 * Cr
// B = Y + 1.77104 * Cb

static const float3 RGB2Cb = { -0.16878f, -0.33122f,  0.50000f };
static const float3 RGB2Cr = {  0.50000f, -0.41836f, -0.08164f };

static const float3 YCbCr2R = { 1.00000f,  0.00000f,  1.40200f };
static const float3 YCbCr2G = { 1.00000f, -0.34414f, -0.71414f };
static const float3 YCbCr2B = { 1.00000f,  1.77200f,  0.00000f };

static const float3x3 RGB2YCbCCr = {
	{ 0.29891f, -0.16874f,  0.50000f },
	{ 0.58661f, -0.33126f, -0.41869f },
	{ 0.11448f,  0.50000f, -0.08131f },
};
static const float3x3 YCbCr2RGB = {
	{ 1.00000f,  1.00000f, 1.00000f },
	{ 0.00000f, -0.34564f, 1.77104f },
	{ 1.40218f, -0.71448f, 0.00000f },
};

// RGB -> YCbCr
float3 RGBToYCbCr(float3 vRGB)
{
	return mul(vRGB, RGB2YCbCCr);
}

// RGB -> Cb
float RGBToCb(float3 vRGB)
{
	return dot(vRGB, RGB2Cb);
}

// RGB -> Cr
float RGBToCr(float3 vRGB)
{
	return dot(vRGB, RGB2Cr);
}

// YCbCr -> RGB
float3 YCbCrToRGB(float3 vYCbCr)
{
	return mul(vYCbCr, YCbCr2RGB);
}

// YCbCr -> R
float YCbCrToR(float3 vYCbCr)
{
	return dot(vYCbCr, YCbCr2R);
}

// YCbCr -> G
float YCbCrToG(float3 vYCbCr)
{
	return dot(vYCbCr, YCbCr2G);
}

// YCbCr -> B
float YCbCrToB(float3 vYCbCr)
{
	return dot(vYCbCr, YCbCr2B);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_YCbCr_FXH__)
