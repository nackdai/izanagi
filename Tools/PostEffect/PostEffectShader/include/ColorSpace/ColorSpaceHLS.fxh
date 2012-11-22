#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_HLS_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_HLS_FXH__

#include "ColorSpaceYUV.fxh"

//////////////////////////////////
// HLS

#if 0
// H : 色相(Hue)
// L : 輝度(Luminance)
// S : 彩度(Saturation)

// NOTE
// MAX = max(max(r, g), b)
// MIN = min(min(r, g), b)
// L = (MAX + MIN) / 2
// S = (MAX - MIN) / (MAX + MIN)     (if L <= 0.5)
//     (MAX - MIN) / (2 - MAX - MIN) (if L > 0.5)
// H = HSV色空間と同じ

// NOTE
// MAX = L x (1 + S)     (if L <= 0.5)
//       L x (1 + S) + S (if L > 0.5)
// MIN = 2 x L - MAX
//
// if S == 0
//   R = G = B = 0
// else
//   h = H + 120
//   h = (h >= 360 ? h - 360 : h)
//      R = MIN + (MAX - MIN) * h / 60         (if 0 <= h < 60)
//          MAX                                (if 60 <= h < 180)
//          MIN + (MAX - MIN) * (240 - h) / 60 (if 180 <= h < 240)
//          MIN                                (if 240 <= h < 360)
//
//   h = H
//      G -> Rと同様
//
//   h = H - 120
//   h = (h < 0 ? h + 360 : h)
//      B -> Rと同様

// RGB -> HLS
float3 RGBToHLS(float3 vRGB)
{
	float3 vHLS = (float3)0.0f;

	float fMax = _GetMaxChannel(vRGB);
	float fMin = _GetMinChannel(vRGB);
	float fDelta = fMax - fMin;
	
	// L
	vHLS.y = (fMax + fMin) * 0.5f;

	if (fDelta != 0.0f) {
		// S
		vHLS.z = (vHLS.y <= 0.5f
					? fDelta / (fMax + fMin)
					: fDelta / (2.0f - fMax - fMin));

		// H(HSVのHと同じ)
		vHLS.x = _ComputeHue(vRGB, fMax, fDelta);
	}

	return vHLS;
}

// HLS -> RGB
float3 HLSToRGB(float3 vHLS)
{
	float3 vRGB = (float3)0.0f;

	float fMax = (vHLS.y <= 0.5f
					? vHLS.y * (1.0f + vHLS.z)
					: vHLS.y * (1.0f - vHLS.z) + vHLS.z);
	float fMin = 2.0f * vHLS.y - fMax;

	if (vHLS.z == 0.0f) {
		vRGB.rgb = vHLS.yyy;
	}
	// TODO
#if 0
	else {
		float3 h = {
			vHLS.x + 1.0f / 3.0f,
			vHLS.x,
			vHLS.x - 1.0f / 3.0f,
		};
		h.x = (h.x >= 1.0f ? h.x - 1.0f : h.x);
		h.z = (h.z < 0.0f ? h.z + 1.0f : h.z);

		if (h < 1.0f / 6.0f) {
			// 0 <= h < 60
			vRGB = fMin + (fMax - fMin) * h * 6.0f;
		}
		else if (h < 1.0f / 2.0f) {
			// 60 <= h < 180
			vRGB = fMax;
		}
		else if (h < 2.0f / 3.0f) {
			// 180 <= h < 240
			//vRGB = fMin + (fMax - fMin) * (2.0f / 3.0f - h) * 6.0f;
			vRGB = fMin + (fMax - fMin) * (4.0f - h * 6.0f);
		}
		else {
			// 240 <= h < 360
			vRGB = fMin;
		}
	}
#endif


	return vRGB;
}
#else
// NOTE
// H : 色相(Hue)
// L : 輝度(Luminance)
// S : 彩度(Saturation)

// NOTE
// U = R - Y
// V = B - Y
//
// L = Y
// H = atan(U / V)
// S = sqrt(U * U + V * V)
//
// U = S * sin(H)
// V = S * cos(H)

// RGB -> HLS
float3 RGBToHLS(float3 vRGB)
{
	// RGB -> YUV
	float3 vYUV = RGBToYUV(vRGB);

	float fH = atan(vYUV.x / (vYUV.z + 0.00001f));		// H
	float fS = sqrt(vYUV.x * vYUV.x + vYUV.z * vYUV.z);	// S

	return float3(fH, vYUV.x, fS);
}

// HLS -> RGB
float3 HLSToRGB(float3 vHLS)
{
	float fU = vHLS.z * sin(vHLS.x);
	float fV = vHLS.z * cos(vHLS.x);

	float3 vRGB = YUVToRGB(float3(vHLS.y, fU, fV));

	return vRGB;
}
#endif

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_HLS_FXH__)
