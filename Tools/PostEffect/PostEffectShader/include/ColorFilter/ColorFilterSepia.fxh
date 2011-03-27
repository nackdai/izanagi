#if !defined(__IZANAGI_POSTEFFECT_COLOR_FILETER_SEPIA_FXH__)
#define __IZANAGI_POSTEFFECT_COLOR_FILETER_SEPIA_FXH__

#include "../ColorSpace/ColorSpaceYCbCr.fxh"

//////////////////////////////////
// �Z�s�A

float3 _ColorFilterSepia(float3 vIn, float fCb, float fCr)
{
	float3 vYCbCr;
	vYCbCr.r = RGBToY(vIn.rgb);

	// �C�ӂ�Cb�ACr���Z�b�g
	vYCbCr.g = fCb;
	vYCbCr.b = fCr;

	// YCbCr -> RGB
	return YCbCrToRGB(vYCbCr);
}

float4 ColorFilterSepia(float4 vIn, float fCb, float fCr)
{
	return float4(_ColorFilterSepia(vIn.rgb, fCb, fCr), vIn.a);
}

float4 ColorFilterSepia(float4 vIn, float2 vCbCr)
{
	return ColorFilterSepia(vIn, vCbCr.r, vCbCr.g);
}

// ���`��Ԃ���
float4 ColorFilterSepia(float4 vIn, float fCb, float fCr, float fWeight)
{
	float3 tmp = lerp(vIn.rgb, _ColorFilterSepia(vIn.rgb, fCb, fCr), fWeight);
	return float4(tmp, vIn.a);
}

// ���`��Ԃ���
float4 ColorFilterSepia(float4 vIn, float2 vCbCr, float fWeight)
{
	return ColorFilterSepia(vIn, vCbCr.r, vCbCr.g, fWeight);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLOR_FILETER_SEPIA_FXH__)
