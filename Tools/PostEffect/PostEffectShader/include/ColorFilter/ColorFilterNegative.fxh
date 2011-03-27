#if !defined(__IZANAGI_POSTEFFECT_COLOR_FILETER_NEGATIVE_FXH__)
#define __IZANAGI_POSTEFFECT_COLOR_FILETER_NEGATIVE_FXH__

#include "../ColorSpace/ColorSpaceCMY.fxh"

//////////////////////////////////
// �l�K�e�B�u

float4 ColorFilterNegative(float4 vIn)
{
	return float4(RGBToCMY(vIn.rgb), vIn.a);
}

// ���`��Ԃ���
float4 ColorFilterNegative(float4 vIn, float fWeight)
{
	float3 tmp = lerp(vIn.rgb, RGBToCMY(vIn.rgb), fWeight);
	return float4(tmp, vIn.a);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLOR_FILETER_NEGATIVE_FXH__)
