#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_HSV_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_HSV_FXH__

#include "ColorSpaceUtil.fxh"

//////////////////////////////////
// HSV

// NOTE
// H : �F���p�x
// 0�`360��
// �Ԃ�0���A����60���A�΂�120���A���F��180���A��240���A����300���A360���ōĂѐԂɖ߂�B
// ---0----60----120----180----240----300----360---
//    R    -->    G     -->     B     -->     R
//
// S : �ʓx
// 0�`1�̎����l�B0���������i���ʐF�j�A�P�����Z���i�L�ʐF�j
//
// V : ���x
// 0�`1�̎����l�B0�����キ�A1���������B0�������G�̋����������ԁA1�������G�̋�������Ȃ���ԁB

// NOTE
// H = 60 x (G - B) / (MAX - MIN) + 0   (if MAX = R)
//     60 x (B - R) / (MAX - MIN) + 120 (if MAX = G)
//     60 x (R - G) / (MAX - MIN) + 240 (if MAX = B)
// S = (MAX - MIN) / MAX
// V = MAX
//
// H < 0 �̂Ƃ��́AH�͏z����̂ŁA+360���Đ��̒l�ɂȂ�悤�ɂ���B
// H > 360 �̂Ƃ��́AH�͏z����̂ŁA-360����360�𒴂��Ȃ��悤�ɂ���B

// NOTE
// �EMAX = MIN �̂Ƃ��A H�͒�`����Ȃ��B
// �EMAX = 0 �̂Ƃ��AS�͖���`�ł���B

// NOTE
// i = H/60�̐�����
// f = H/60�̏�����
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
		// 0 - 1 -> 0 -> 360 �ɕϊ����āA60�Ŋ���
		// H x 360 / 60 -> H x 6
		float val = vHSV.x * 6.0f;

		// H/60�̐�����
		int i = (int)val;

		// H/60�̏�����
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
