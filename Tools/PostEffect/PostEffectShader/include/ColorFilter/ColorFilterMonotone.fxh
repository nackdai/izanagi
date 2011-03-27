#if !defined(__IZANAGI_POSTEFFECT_COLOR_FILETER_MONOTONE_FXH__)
#define __IZANAGI_POSTEFFECT_COLOR_FILETER_MONOTONE_FXH__

#include "../ColorSpace/ColorSpaceYCbCr.fxh"

//////////////////////////////////
// ���m�g�[��

float4 ColorFilterMonotone(float4 vIn)
{
	float fY = RGBToY(vIn.rgb);
	return float4(fY, fY, fY, vIn.a);
}

// ���`��Ԃ���
float4 ColorFilterMonotone(float4 vIn, float fWeight)
{
	float fY = RGBToY(vIn.rgb);
	float3 tmp = lerp(vIn.rgb, float3(fY, fY, fY), fWeight);
	return float4(tmp, vIn.a);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLOR_FILETER_MONOTONE_FXH__)
