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

	// Yをどの程度変化させたのか
	float fScale = fY / vYUV.x;

	vYUV.x = fY;
	vYUV.yz *= fScale;	// Yの変化率にあわせる

	// YUV -> RGB
	float3 vRet = YUVToRGB(vYUV);

	return float4(vRet.x, vRet.y, vRet.z, 1.0f);
}

///////////////////////////////////////////////////////////

// トーンマップ計算
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

	// Yをどの程度変化させたのか
	float fScale = fY / vYUV.x;

	vYUV.x = fY;
	vYUV.yz *= fScale;	// Yの変化率にあわせる

	float4 vRet = vIn;

	// YUV -> RGB
	vRet.rgb = YUVToRGB(vYUV);

	return vRet;
#else
	float4 vRet = _ComputeTonemapInternal(vYUV, fMiddleGrey, fAdaptedLum);
	return vRet;
#endif
}

// 輝度抽出
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

	// Yをどの程度変化させたのか
	float fScale = fY / vYUV.x;

	vYUV.x = fY;
	vYUV.yz *= fScale;	// Yの変化率にあわせる

	// YUV -> RGB
	float3 vRet = YUVToRGB(vYUV);

	return float4(vRet.x, vRet.y, vRet.z, 1.0f);
#else
	vYUV.x = max(vYUV.x - fThreshold, 0.0f);
	float4 vRet = _ComputeTonemapInternal(vYUV, fMiddleGrey, fAdaptedLum);
	return vRet;
#endif
}

// 明・暗順応計算
float4 ComputeAdaptation(
	float fAdaptedLum,
	float fCurrentLum,
	float fAdaptedRate)
{
	// TODO
	// 明・暗順応対応
	//float fRatio = min(exp(0.5 * fCurrentLum) - 0.9f, 1.0f);
	//float fRatio = min(pow(fCurrentLum, 3.0f) + 0.2f, 1.0f);

	// NOTE
	// Hermite補間をもとに計算
	// f(x) = (-2 * x^3 + 3 * x^2) * x
	float fRatio = (-2.0f * pow(fCurrentLum, 3.0f) + 3.0f * pow(fCurrentLum, 2.0f)) * fCurrentLum;

	// 0.1f - 1.0f に丸める
	fRatio = clamp(fRatio, 0.1f, 1.0f);
    
	// NOTE
	// AdaptedLumとCurrentLumの間の差を毎フレーム指定された％ずつ近づけるように適応させている。
	// ちなみに、人間の目の適応モデルとは一切関係ない。
    float fNewAdaptation = fAdaptedLum + (fCurrentLum - fAdaptedLum) * fAdaptedRate * fRatio;
    return float4(fNewAdaptation, fNewAdaptation, fNewAdaptation, 1.0f);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_TONEMAP_FXH__)
