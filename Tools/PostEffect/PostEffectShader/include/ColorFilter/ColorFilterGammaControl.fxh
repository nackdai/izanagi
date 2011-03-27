#if !defined(__IZANAGI_POSTEFFECT_COLOR_FILTER_GAMMA_CONTROL_FXH__)
#define __IZANAGI_POSTEFFECT_COLOR_FILTER_GAMMA_CONTROL_FXH__

//////////////////////////////////
// �K���}�␳

// NOTE
// pow(RGB, 1/�K���}�l)

// �W���K���}�l
#define NTSC_GAMMA	(2.2f)	// NTSC�Œ�߂��Ă���K���}�l
#define MAC_GAMMA	(1.8f)	// �}�b�L���g�b�V���ɂ�����W���̃K���}�l�ŁA�t�H�g�V���b�v����������l
#define WIN_GAMMA	(2.35f)	// Windows�̕W���̃K���}�l�� 2.2 - 2.5 �Ȃ̂ŁA���̊Ԃ��Ƃ��Ă݂�

// �W���K���}�l�̋t��
#define INV_NTSC_GAMMA	(1.0f / NTSC_GAMMA)
#define INV_MAC_GAMMA	(1.0f / MAC_GAMMA)
#define INV_WIN_GAMMA	(1.0f / WIN_GAMMA)

// �K���}�l �������Ƃ���
float3 ColorFilterGammaControl(float3 vRGB, float fGamma)
{
	return pow(vRGB, 1.0f / fGamma);
}

// 1/�K���}�l �������Ƃ���
float3 ColorFilterGammaControlEx(float3 vRGB, float fGammmaInv)
{
	return pow(vRGB, fGammmaInv);
}

// ITU-R BT. 709 recommendation (Rec. 709)�ɋK�肳��Ă���ϊ�

// NOTE
// R' = 4.5 * R                         (if R < 0.0018)
//      1.099f * pow(R, 1/2.2) - 0.099  (if R >= 0.0018)
// G'�AB'�����l
//
// R = R' / 4.5f                      (if R' < 0.0812)
//     pow((R + 0.099) / 1.099, 2.2)  (if R' >= 0.0812)
// G�AB�����l

#define __GAMMA_CTRL_REC_709__(c)\
	(c < 0.018f\
		? 4.5f * c\
		: 1.099f * pow(c, INV_NTSC_GAMMA) - 0.099f)

// TU-R BT. 709 recommendation (Rec. 709)�ɋK�肳��Ă���K���}�␳
float3 ColorFilterGammaControlByRec709(float3 vRGB)
{
	float3 vGammaRGB;
	vGammaRGB.r = __GAMMA_CTRL_REC_709__(vRGB.r);
	vGammaRGB.g = __GAMMA_CTRL_REC_709__(vRGB.g);
	vGammaRGB.b = __GAMMA_CTRL_REC_709__(vRGB.b);

	return vGammaRGB;
}

#define __RGB_FROM_GAMMA_CTRL_REC_709__(c)\
	(c < 0.0812f\
		? c / 4.5f\
		: pow((c + 0.099f) / 1.099f, NTSC_GAMMA))

// TU-R BT. 709 recommendation (Rec. 709)�ɋK�肳��Ă���K���}�␳�������̂�RGB�ɖ߂�
float3 ComputeRGBFromGammaControlByRec709(float3 vGammaRGB)
{
	float3 vRGB;
	vRGB.r = __RGB_FROM_GAMMA_CTRL_REC_709__(vGammaRGB.r);
	vRGB.g = __RGB_FROM_GAMMA_CTRL_REC_709__(vGammaRGB.g);
	vRGB.b = __RGB_FROM_GAMMA_CTRL_REC_709__(vGammaRGB.b);

	return vRGB;
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLOR_FILTER_GAMMA_CONTROL_FXH__)
