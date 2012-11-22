#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_HSV_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_HSV_FXH__

#include "ColorSpaceUtil.fxh"

//////////////////////////////////
// HSV

// NOTE
// H : 色相角度
// 0～360°
// 赤が0°、黄が60°、緑が120°、水色が180°、青が240°、紫が300°、360°で再び赤に戻る。
// ---0----60----120----180----240----300----360---
//    R    -->    G     -->     B     -->     R
//
// S : 彩度
// 0～1の実数値。0側が薄く（無彩色）、１側が濃い（有彩色）
//
// V : 明度
// 0～1の実数値。0側が弱く、1側が強い。0側が黒絵の具を加えた状態、1側が黒絵の具を加えない状態。

// NOTE
// H = 60 x (G - B) / (MAX - MIN) + 0   (if MAX = R)
//     60 x (B - R) / (MAX - MIN) + 120 (if MAX = G)
//     60 x (R - G) / (MAX - MIN) + 240 (if MAX = B)
// S = (MAX - MIN) / MAX
// V = MAX
//
// H < 0 のときは、Hは循環するので、+360して正の値になるようにする。
// H > 360 のときは、Hは循環するので、-360して360を超えないようにする。

// NOTE
// ・MAX = MIN のとき、 Hは定義されない。
// ・MAX = 0 のとき、Sは未定義である。

// NOTE
// i = H/60の整数部
// f = H/60の小数部
// p1 = V * (1 - S)
// p2 = V * (1 - S * f)
// p3 = V * (1 - S * (1 - f))
// i = 0: R = V,  G = p3, B = p1
// i = 1: R = p2, G = V,  B = p1
// i = 2: R = p1, G = V,  B = p3
// i = 3: R = p1, G = p2, B = V
// i = 4: R = p3, G = p1, B = V
// i = 5: R = V,  G = p1, B = p2

// RGB -> HSV
float3 RGBToHSV(float3 vRGB)
{
	float3 vHSV = (float3)0.0f;

	float fMax = _GetMaxChannel(vRGB);
	float fMin = _GetMinChannel(vRGB);
	float fDelta = fMax - fMin;
	
	// V
	vHSV.z = fMax;

	if (fDelta != 0.0f) {
		// S
		vHSV.y = fDelta / fMax;

		// H
		vHSV.x = _ComputeHue(vRGB, fMax, fDelta);
	}

	return vHSV;
}

// HSV -> RGB
float3 HSVToRGB(float3 vHSV)
{
	float3 vRGB = vHSV.z;

	if (vHSV.y != 0.0f) {
		// H/60
		// 0 - 1 -> 0 -> 360 に変換して、60で割る
		// H x 360 / 60 -> H x 6
		float val = vHSV.x * 6.0f;

		// H/60の整数部
		int i = (int)val;

		// H/60の小数部
		float f = frac(val);

		float p1 = vHSV.z * (1.0f - vHSV.y);				// V * (1 - S)
		float p2 = vHSV.z * (1.0f - vHSV.y * f);			// V * (1 - S * f)
		float p3 = vHSV.z * (1.0f - vHSV.y * (1.0f - f));	// V * (1 - S * (1 - f))

		if (i == 0) {
			// i = 0: R = V,  G = p3, B = p1
			vRGB.r = vHSV.z;
			vRGB.g = p3;
			vRGB.b = p1;
		}
		else if (i == 1) {
			// i = 1: R = p2, G = V,  B = p1
			vRGB.r = p2;
			vRGB.g = vHSV.z;
			vRGB.b = p1;
		}
		else if (i == 2) {
			// i = 2: R = p1, G = V,  B = p3
			vRGB.r = p1;
			vRGB.g = vHSV.z;
			vRGB.b = p3;
		}
		else if (i == 3) {
			// i = 3: R = p1, G = p2, B = V
			vRGB.r = p1;
			vRGB.g = p2;
			vRGB.b = vHSV.z;
		}
		else if (i == 4) {
			// i = 4: R = p3, G = p1, B = V
			vRGB.r = p3;
			vRGB.g = p1;
			vRGB.b = vHSV.z;
		}
		else if (i == 5) {
			// i = 5: R = V,  G = p1, B = p2
			vRGB.r = vHSV.z;
			vRGB.g = p1;
			vRGB.b = p2;
		}
	}

	return vRGB;
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_HSV_FXH__)
