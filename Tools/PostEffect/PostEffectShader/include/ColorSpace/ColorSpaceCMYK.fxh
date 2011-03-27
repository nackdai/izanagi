#if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_CMYK_FXH__)
#define __IZANAGI_POSTEFFECT_COLORSPACE_CMYK_FXH__

//////////////////////////////////
// CMYK

// NOTE
// ���_��ACMY�����ׂċϓ��ɍ�����ƍ��F�ɂȂ邪�A
// �C���N�⎆�̓�����ACMY�̃C���N���������Y��ȍ��F�����̂͋Z�p�I�ɍ���ł���A
// �ʏ�͂��ׂĂ������Ă����������F�ɂ����Ȃ�Ȃ��B
// �����ŁA���iKey plate�j�̔��F���悭���邽�߂ɕʓr���C���N��p����悤�ɂȂ����̂�CMYK�ł���B

// NOTE
// K = min(min(1 - R, 1 - G), 1 - B)
// C = (1 - R - K) / (1 - K)
// M = (1 - G - K) / (1 - K)
// Y = (1 - B - K) / (1 - K)
//
// R = 1 - min(1, C * (1 - K) + K)
// G = 1 - min(1, M * (1 - K) + K)
// B = 1 - min(1, Y * (1 - K) + K)

// NOTE
// K = min(min(C, M), Y)
// C' = (C - K) / (1 - K)
// M' = (M - K) / (1 - K)
// Y' = (Y - K) / (1 - K)
//
// C = C' * (1 - K) + K
// M = M' * (1 - K) + K
// Y = Y' * (1 - K) + K

// NOTE
// C - K -> K��ɂȂ�悤�ɕϊ��H
// (C - K) / (1 - K) -> (1 - K)�Ŋ��邱�Ƃ� 0 - 1 �ɕϊ��H

// CMY -> CMYK
float4 CMYToCMYK(float3 vCMY)
{
	float4 vCMYK = (float4)1.0f;

	// K
	vCMYK.a = min(min(vCMY.r, vCMY.g), vCMY.b);

	// CMY
	vCMYK.rgb = min(1.0f, (vCMY.rgb - vCMYK.a) / (1.0f - vCMYK.a + 0.00001f));

	return vCMYK;
}

// CMYK -> CMY
float3 CMYKToCMY(float4 vCMYK)
{
	// NOTE
	// C = C' * (1 - K) + K
	// M = M' * (1 - K) + K
	// Y = Y' * (1 - K) + K

	return float3(vCMYK.rgb * (1.0f - vCMYK.a) + vCMYK.a);
}

// RGB -> CMYK
float4 RGBToCMYK(float3 vRGB)
{
	// RGB -> CMY -> CMYK
	return CMYToCMYK(RGBToCMY(vRGB));
}

// CMYK -> RGB
float3 CMYKToRGB(float4 vCMYK)
{
	// CMYK -> CMY -> RGB
	return CMYToRGB(CMYKToCMY(vCMYK));
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_COLORSPACE_CMYK_FXH__)
