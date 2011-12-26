#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_YUV_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_YUV_FXH__

#include "ColorSpaceUtil.fxh"

//////////////////////////////////
// YUV

// NOTE
// Y は輝度信号（明るさ）
// U は赤の差分信号 (色差信号)
// V は青の差分信号 (色差信号)

// NOTE
// Y =  0.29891 * R + 0.58661 * G + 0.11448 * B
// U = R - Y 
//   =  0.70109 * R - 0.58661 * G - 0.11448 * B
// V = B - Y 
//   = -0.29891 * R - 0.58661 * G + 0.88552 * B
//
// R = Y + (R - Y) = Y + U
// G = Y - 0.29891 / 0.58661 * U - 0.11448 / 0.58661 * V = Y - 0.50955 * U - 0.19516 * V
// B = Y + (B - Y) = Y + V

// NOTE
//  0.00000 <= Y <= 1.00000
// -0.70109 <= U <= 0.70109
// -0.88552 <= V <= 0.88552
// Yの値の範囲の幅が1.0であるが、Uは1.40218、Vは1.77104となり、信号振幅が揃っていない。
// これをそろえるようにスケーリングしたものがYCbCrになる。

static const float3 RGB2U = {  0.70109f, -0.58661f, -0.11448f };
static const float3 RGB2V = { -0.29891f, -0.58661f,  0.88552f };

static const float3 YUV2G = {  1.00000f, -0.50955f, -0.19516f };

// YUV2R、YUV2B は簡単にできるので・・・

// 一応・・・
static const float3x3 RGB2YUV = {
	{ 0.29891f,  0.70109f, -0.29891f },
	{ 0.58661f, -0.58661f, -0.58661f },
	{ 0.11448f, -0.11448f,  0.88552f },
};
static const float3x3 YUV2RGB = {
	{ 1.00000f,  1.00000f, 1.00000f },
	{ 1.00000f, -0.50955f, 0.00000f },
	{ 0.00000f, -0.19516f, 1.00000f },
};

// RGB -> YUV
float3 RGBToYUV(float3 vRGB)
{
	float fY = dot(RGB2Y, vRGB);
	return float3(fY, vRGB.r - fY, vRGB.b - fY);
}

// RGB -> U
float RGBToU(float3 vRGB)
{
	return dot(vRGB, RGB2U);
}

// RGB -> V
float RGBToV(float3 vRGB)
{
	return dot(vRGB, RGB2V);
}

// YUV -> RGB
float3 YUVToRGB(float3 vYUV)
{
	float fG = dot(vYUV, YUV2G);
	return float3(vYUV.x + vYUV.y, fG, vYUV.x + vYUV.z);
}

// YUV -> R
float YUVToR(float3 vYUV)
{
	// NOTE
	// R = Y + U = Y + (R - Y)
	return (vYUV.x + vYUV.y);
}

// YUV -> G
float YUVToG(float3 vYUV)
{
	return dot(vYUV, YUV2G);
}

// YUV -> B
float YUVToB(float3 vYUV)
{
	// NOTE
	// B = Y + V = Y + (B - Y)
	return (vYUV.x + vYUV.z);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_YUV_FXH__)
