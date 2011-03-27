#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_YIQ_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_YIQ_FXH__

#include "ColorSpaceUtil.fxh"

//////////////////////////////////
// YIQ

// NOTE
// YIQ��NTSC(National Television System Committee)�����̃J���[�e���r�����Ŏg����J���[���f���ł���B
// Y�M����0�`4MHz�ш�AI�M����1.5MHz�ш�AQ�M����0.6MHz�ш�ł���B
// Y�̓J���[�f���𔒍��f���ɂ��邱�ƂɎg�p�ł���B(�����e���r�ƌ݊���������B)
//
// Y : �P�x
// I : ���F���܂ށA�I�����W����V�A���ɂ����Ă̐F��
// Q : I�ȊO�̐F��

// NOTE
// �J���[�e���r�ł́A�F���M����ό`���AI�AQ�M�Ƃ����`�ŐF����`�����Ă���B
// I�AQ�M���́A�F���M����33�x��]���č���Ă���B
// I�M���̓I�����W����V�A���n�̐M���AQ�M���͗΂���}�[���^�n�̐M���ƂȂ�B

// NOTE
// I = (R - Y) / 1.14 * cos(33) - (B - Y) / 2.03 * sin(33)
// Q = (R - Y) / 1.14 * sin(33) + (B - Y) / 2.03 * cos(33)

// NOTE
// sin(33) = 0.54464
// cos(33) = 0.83867
// Y = 0.29891 * R + 0.58661 * G + 0.11448 * B
// I = (R - Y) / 1.14 * 0.83867 - (B - Y) / 2.03 * 0.54464
//   = 0.73568 * U - 0.26830 * V
//   = 0.59598 * R - 0.27417 * G - 0.32181 * B
// Q = (R - Y) / 1.14 * 0.54464 + (B - Y) / 2.03 * 0.83867
//   = 0.47775 * U + 0.41314 * V
//   = 0.21145 * R - 0.52260 * G + 0.31115 * B

// NOTE
// I = a * U - b * V
// Q = c * U + d * V
//
// 1/b * I + 1/d * Q = (a/b * U - V) + (c/d * U + V) = (a/b + c/d) * U
// <=> U = d/(ad + bc) * I + b/(ad + bc) * Q
//       =  0.95608 * I + 0.62089 * Q
// 1/a * I - 1/c * Q = (U - b/a * V) - (U + d/c * V)
// <=> V = -c/(bc + ad) * I + a/(bc + ad) * Q
//       = -1.10560 * I + 1.70249 * Q
// 
// R = Y + U
//   = Y + 0.95608 * I + 0.62089 * Q
// G = Y - 0.50955 * U - 0.19516 * V
//   = Y - 0.27177 * I - 0.64863 * Q
// B = Y + V
//   = Y - 1.10560 * I + 1.70249 * Q

static const float3 RGB2I = { 0.59598f, -0.27417f, -0.32181f };
static const float3 RGB2Q = { 0.21145f, -0.52260f,  0.31115f };

static const float3 YIQ2R = { 1.00000f,  0.95608f,  0.62089f };
static const float3 YIQ2G = { 1.00000f, -0.27177f, -0.64863f };
static const float3 YIQ2B = { 1.00000f, -1.10560f,  1.70249f };

static const float3x3 RGB2YIQ = {
	{ 0.29891f,  0.59598f,  0.21145f },
	{ 0.58661f, -0.27417f, -0.52260f },
	{ 0.11448f, -0.32181f,  0.31115f },
};
static const float3x3 YIQ2RGB = {
	{ 1.00000f,  1.00000f,  1.00000f },
	{ 0.95608f, -0.27177f, -1.10560f },
	{ 0.62089f, -0.64863f,  1.70249f },
};

// RGB -> YIQ
float3 RGBToYIQ(float3 vRGB)
{
	return mul(vRGB, RGB2YIQ);
}

// RGB -> I
float RGBToI(float vRGB)
{
	return dot(vRGB, RGB2I);
}

// RGB -> Q
float RGBToQ(float vRGB)
{
	return dot(vRGB, RGB2Q);
}

// YIQ -> RGB
float3 YIQToRGB(float3 vYIQ)
{
	return mul(vYIQ, YIQ2RGB);
}

// YIQ -> R
float YIQToR(float3 vYIQ)
{
	return dot(vYIQ, YIQ2R);
}

// YIQ -> G
float YIQToG(float3 vYIQ)
{
	return dot(vYIQ, YIQ2G);
}

// YIQ -> B
float YIQToB(float3 vYIQ)
{
	return dot(vYIQ, YIQ2B);
}


#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_YIQ_FXH__)
