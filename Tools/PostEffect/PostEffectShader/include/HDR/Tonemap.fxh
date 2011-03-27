#if !defined(__IZANAGI_POSTEFFECT_TONEMAP_FXH__)
#define __IZANAGI_POSTEFFECT_TONEMAP_FXH__

#include "../ColorSpace/ColorSpaceUtil.fxh"

float4 _ComputeTonemapInternal(
	float3 vYUV,
	float fMiddleGrey,
	float fAdaptedLum)
{
	float yy = vYUV.x * fMiddleGrey / (fAdaptedLum + 0.00001f);

	// x' = (1 - exp(2 * x)) ^ 1.5
	float fY = pow(1.0f - exp(-yy * 2.0f), 1.5f);

	// Y���ǂ̒��x�ω��������̂�
	float fScale = fY / vYUV.x;

	vYUV.x = fY;
	vYUV.yz *= fScale;	// Y�̕ω����ɂ��킹��

	// YUV -> RGB
	float3 vRet = YUVToRGB(vYUV);

	return float4(vRet.x, vRet.y, vRet.z, 1.0f);
}

///////////////////////////////////////////////////////////

// �g�[���}�b�v�v�Z
float4 ComputeTonemap(
	float4 vIn,
	float fMiddleGrey,
	float fAdaptedLum)
{
	// RGB -> YUV
	float3 vYUV = RGBToYUV(vIn.rgb);

#if 0
	float yy = vYUV.x * fMiddleGrey / (fAdaptedLum + 0.00001f);

	// x' = (1 - exp(2 * x)) ^ 1.5
	float fY = pow(1.0f - exp(-yy * 2.0f), 1.5f);

	// Y���ǂ̒��x�ω��������̂�
	float fScale = fY / vYUV.x;

	vYUV.x = fY;
	vYUV.yz *= fScale;	// Y�̕ω����ɂ��킹��

	float4 vRet = vIn;

	// YUV -> RGB
	vRet.rgb = YUVToRGB(vYUV);

	return vRet;
#else
	float4 vRet = _ComputeTonemapInternal(vYUV, fMiddleGrey, fAdaptedLum);
	return vRet;
#endif
}

// �P�x���o
float4 ExtractLum(
	float4 vIn,
	float fMiddleGrey,
	float fAdaptedLum,
	float fThreshold)
{
	// RGB -> YUV
	float3 vYUV = RGBToYUV(vIn.rgb);

#if 0
	float yy = max(vYUV.x - fThreshold, 0.0f);

	yy = yy * fMiddleGrey / (fAdaptedLum + 0.00001f);
	
	// x' = (1 - exp(2 * x)) ^ 1.5
	float fY = pow(1.0f - exp(-yy * 2.0f), 1.5f);

	// Y���ǂ̒��x�ω��������̂�
	float fScale = fY / vYUV.x;

	vYUV.x = fY;
	vYUV.yz *= fScale;	// Y�̕ω����ɂ��킹��

	// YUV -> RGB
	float3 vRet = YUVToRGB(vYUV);

	return float4(vRet.x, vRet.y, vRet.z, 1.0f);
#else
	vYUV.x = max(vYUV.x - fThreshold, 0.0f);
	float4 vRet = _ComputeTonemapInternal(vYUV, fMiddleGrey, fAdaptedLum);
	return vRet;
#endif
}

// ���E�Ï����v�Z
float4 ComputeAdaptation(
	float fAdaptedLum,
	float fCurrentLum,
	float fAdaptedRate)
{
	// TODO
	// ���E�Ï����Ή�
	//float fRatio = min(exp(0.5 * fCurrentLum) - 0.9f, 1.0f);
	//float fRatio = min(pow(fCurrentLum, 3.0f) + 0.2f, 1.0f);

	// NOTE
	// Hermite��Ԃ����ƂɌv�Z
	// f(x) = (-2 * x^3 + 3 * x^2) * x
	float fRatio = (-2.0f * pow(fCurrentLum, 3.0f) + 3.0f * pow(fCurrentLum, 2.0f)) * fCurrentLum;

	// 0.1f - 1.0f �Ɋۂ߂�
	fRatio = clamp(fRatio, 0.1f, 1.0f);
    
	// NOTE
	// AdaptedLum��CurrentLum�̊Ԃ̍��𖈃t���[���w�肳�ꂽ�����߂Â���悤�ɓK�������Ă���B
	// ���Ȃ݂ɁA�l�Ԃ̖ڂ̓K�����f���Ƃ͈�؊֌W�Ȃ��B
    float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * fAdaptedRate * fRatio;
    return float4(fNewAdaptation, fNewAdaptation, fNewAdaptation, 1.0f);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_TONEMAP_FXH__)
