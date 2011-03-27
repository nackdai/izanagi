#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_CMY_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_CMY_FXH__

//////////////////////////////////
// CMY

// NOTE
// CMY�͈���̉ߒ��ŗ��p���錸�@���F�̕\���@�ł���B
// �G�̋�̎O���F�B��{�F�͔��ŁA����ɐF�̓x�����������āA���F�ɂ��Ă����B
// ���Ȃ킿�A�n�߂͔����L�����o�X����n�߁A�C���N�������ĈÂ����Ă���
// �i���ˌ������炷�A���Ȃ킿���@�j�Ƃ������Ƃł���B

// NOTE
// CMY -> �V�A�� (Cyan)�A�}�[���^ (Magenta)�A�C�G���[ (Yellow) 

// NOTE
// CMY = 1 - RGB

// RGB -> CMY
float3 RGBToCMY(float3 vRGB)
{
	return (float3(1.0f, 1.0f, 1.0f) - vRGB);
}

// CMY -> RGB
float3 CMYToRGB(float3 vCMY)
{
	return (float3(1.0f, 1.0f, 1.0f) - vCMY);
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_CMY_FXH__)
