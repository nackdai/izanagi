#include "std/StdColor.h"
#include "unMath.h"

using namespace uranus;

#define SET_COLOR_VAL(p, v)	if (p != UN_NULL) { *(p) = (v); }

static const UN_FLOAT _Div = 1.0f / 255.0f;
#define BYTE_TO_FLOAT(n)	n * _Div;

#define SATURATE(v)	CMath::Clamp(v, 0.0f, 1.0f)

///////////////////////////////////////////////////////////////
// YUV

// NOTE
// Y �͋P�x�M���i���邳�j
// U �͐Ԃ̍����M�� (�F���M��)
// V �͐̍����M�� (�F���M��)

// NOTE
// Y =  0.29891 * R + 0.58661 * G + 0.11448 * B
// U = R - Y 
//   =  0.70109 * R - 0.58661 * G - 0.11448 * B
// V = B - Y 
//   = -0.29891 * R - 0.58661 * G + 0.88552 * B
//
// R = Y + (R - Y) = Y + U
// G = Y - 0.29891 / 0.58661 * U - 0.11448 / 0.58661 * V = Y - 0.50955 * U - 0.19516 * V
// B = Y + (B - Y) = Y + V

// NOTE
//  0.00000 <= Y <= 1.00000
// -0.70109 <= U <= 0.70109
// -0.88552 <= V <= 0.88552
// Y�̒l�͈̔͂̕���1.0�ł��邪�AU��1.40218�AV��1.77104�ƂȂ�A�M���U���������Ă��Ȃ��B
// ��������낦��悤�ɃX�P�[�����O�������̂�YCbCr�ɂȂ�B

void CColor::SetYUV(UN_FLOAT _y, UN_FLOAT _u, UN_FLOAT _v)
{
	_y = CMath::Clamp(_y, 0.0f, 1.0f);
	_u = CMath::Clamp(_u, -0.70109f, 0.70109f);
	_v = CMath::Clamp(_v, -0.88552f, 0.88552f);

	UN_FLOAT fR = SATURATE(_y + _u);
	UN_FLOAT fG = SATURATE(_y - 0.50955f * _u - 0.19516f * _v);
	UN_FLOAT fB = SATURATE(_y + _v);

	r = static_cast<UN_BYTE>(fR * 255);
	g = static_cast<UN_BYTE>(fG * 255);
	b = static_cast<UN_BYTE>(fB * 255);
}

void CColor::GetYUV(UN_FLOAT* _y, UN_FLOAT* _u, UN_FLOAT* _v)
{
	UN_FLOAT fR = BYTE_TO_FLOAT(r);
	UN_FLOAT fG = BYTE_TO_FLOAT(g);
	UN_FLOAT fB = BYTE_TO_FLOAT(b);

	UN_FLOAT y = SATURATE(0.29891f * fR + 0.58661f * fG + 0.11448f * fB);

	SET_COLOR_VAL(_y, y);
	SET_COLOR_VAL(_u, fR - y);
	SET_COLOR_VAL(_v, fB - y);
}

///////////////////////////////////////////////////////////////
// YCbCr

// NOTE
// Y �͋P�x�M���i���邳�j
// Cb �͐̍����M�� (�F���M��)
// Cr �͐Ԃ̍����M�� (�F���M��)

// NOTE
//  0.00000 <= Y <= 1.00000
// -0.70109 <= U <= 0.70101
// -0.88552 <= V <= 0.88552
// Y�̒l�͈̔͂̕���1.0�ł��邪�AU��1.40218�AV��1.77104�ƂȂ�A�M���U���������Ă��Ȃ��B
// ��������낦��悤�ɃX�P�[�����O�������̂�YCbCr�ɂȂ�B
//
//  0.00000 <= Y  <= 1.00000
// -0.50000 <= Cb <= 0.50000
// -0.50000 <= Cr <= 0.50000
// Y�ACb�ACr�Ƃ��ɒl�͈̔͂̕���1.0�ɂȂ�B

// NOTE
// Y  =  0.29891 * R + 0.58661 * G + 0.11448 * B
// Cb = V / (0.88552 * 2) = V / 1.77104 = 0.56464 * V = 0.56464 * (B - Y)
//    = -0.16878 * R - 0.33122 * G + 0.50000 * B
// Cr = U / (0.70109 * 2) = U / 1.40218 = 0.71318 * U = 0.71318 * (R - Y)
//    =  0.50000 * R - 0.41836 * G - 0.08164 * B
//
// R = Y + U = Y + 1.40218 * Cr
// G = Y - 0.50955 * U - 0.19516 * V = Y - 0.50955 * 1.40218 * Cr - 0.19516 * 1.77104 * Cb
// B = Y + V = Y + 1.77104 * Cb
//
// R = Y                + 1.40218 * Cr
// G = Y - 0.34564 * Cb - 0.71448 * Cr
// B = Y + 1.77104 * Cb

void CColor::SetYCbCr(UN_FLOAT y, UN_FLOAT cb, UN_FLOAT cr)
{
	y = CMath::Clamp(y,  0.0f, 1.0f);
	cb = CMath::Clamp(cb, -0.5f, 0.5f);
	cr = CMath::Clamp(cr, -0.5f, 0.5f);

	UN_FLOAT fR = SATURATE(y + 1.40218f * cr);
	UN_FLOAT fG = SATURATE(y - 0.50955f * 1.40218f * cr - 0.19516f * 1.77104f * cb);
	UN_FLOAT fB = SATURATE(y + 1.77104f * cb);

	r = static_cast<UN_BYTE>(fR * 255);
	g = static_cast<UN_BYTE>(fG * 255);
	b = static_cast<UN_BYTE>(fB * 255);
}

void CColor::GetYCbCr(UN_FLOAT* y, UN_FLOAT* cb, UN_FLOAT* cr)
{
	UN_FLOAT fR = BYTE_TO_FLOAT(r);
	UN_FLOAT fG = BYTE_TO_FLOAT(g);
	UN_FLOAT fB = BYTE_TO_FLOAT(b);

	SET_COLOR_VAL(y,   0.29891f * fR + 0.58661f * fG + 0.11448f * fB);
	SET_COLOR_VAL(cb, -0.16878f * fR - 0.33122f * fG + 0.50000f * fB);
	SET_COLOR_VAL(cr,  0.50000f * fR - 0.41836f * fG - 0.08164f * fB);
}

///////////////////////////////////////////////////////////////
// YIQ

// NOTE
// YIQ��NTSC(National Television System Committee)�����̃J���[�e���r�����Ŏg����J���[���f���ł���B
// Y�M����0�`4MHz�ш�AI�M����1.5MHz�ш�AQ�M����0.6MHz�ш�ł���B
// Y�̓J���[�f���𔒍��f���ɂ��邱�ƂɎg�p�ł���B(�����e���r�ƌ݊���������B)
//
// Y : �P�x
// I : ���F���܂ށA�I�����W����V�A���ɂ����Ă̐F��
// Q : I�ȊO�̐F��

// NOTE
// �J���[�e���r�ł́A�F���M����ό`���AI�AQ�M�Ƃ����`�ŐF����`�����Ă���B
// I�AQ�M���́A�F���M����33�x��]���č���Ă���B
// I�M���̓I�����W����V�A���n�̐M���AQ�M���͗΂���}�[���^�n�̐M���ƂȂ�B

// NOTE
// I = (R - Y) / 1.14 * cos(33) - (B - Y) / 2.03 * sin(33)
// Q = (R - Y) / 1.14 * sin(33) + (B - Y) / 2.03 * cos(33)

// NOTE
// sin(33) = 0.54464
// cos(33) = 0.83867
// Y = 0.29891 * R + 0.58661 * G + 0.11448 * B
// I = (R - Y) / 1.14 * 0.83867 - (B - Y) / 2.03 * 0.54464
//   = 0.73568 * U - 0.26830 * V
//   = 0.59598 * R - 0.27417 * G - 0.32181 * B
// Q = (R - Y) / 1.14 * 0.54464 + (B - Y) / 2.03 * 0.83867
//   = 0.47775 * U + 0.41314 * V
//   = 0.21145 * R - 0.52260 * G + 0.31115 * B

// NOTE
// I = a * U - b * V
// Q = c * U + d * V
//
// 1/b * I + 1/d * Q = (a/b * U - V) + (c/d * U + V) = (a/b + c/d) * U
// <=> U = d/(ad + bc) * I + b/(ad + bc) * Q
//       =  0.95608 * I + 0.62089 * Q
// 1/a * I - 1/c * Q = (U - b/a * V) - (U + d/c * V)
// <=> V = -c/(bc + ad) * I + a/(bc + ad) * Q
//       = -1.10560 * I + 1.70249 * Q
// 
// R = Y + U
//   = Y + 0.95608 * I + 0.62089 * Q
// G = Y - 0.50955 * U - 0.19516 * V
//   = Y - 0.27177 * I - 0.64863 * Q
// B = Y + V
//   = Y - 1.10560 * I + 1.70249 * Q

void CColor::SetYIQ(UN_FLOAT y, UN_FLOAT i, UN_FLOAT q)
{
	UN_FLOAT u = 0.95608f * i + 0.62089f * q;
	UN_FLOAT _v = -1.10560f * i + 1.70249f *q;

	SetYUV(y, u, _v);
}

void CColor::GetYIQ(UN_FLOAT* y, UN_FLOAT* i, UN_FLOAT* q)
{
	UN_FLOAT fR = BYTE_TO_FLOAT(r);
	UN_FLOAT fG = BYTE_TO_FLOAT(g);
	UN_FLOAT fB = BYTE_TO_FLOAT(b);

	SET_COLOR_VAL(y, 0.29891f * fR + 0.58661f * fG + 0.11448f * fB);
	SET_COLOR_VAL(i, 0.59598f * fR - 0.27417f * fG - 0.32181f * fB);
	SET_COLOR_VAL(q, 0.21145f * fR - 0.52260f * fG + 0.31115f * fB);
}

///////////////////////////////////////////////////////////////
// HLS

// NOTE
// H : �F��(Hue)
// L : �P�x(Luminance)
// S : �ʓx(Saturation)

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

void CColor::SetHLS(UN_FLOAT h, UN_FLOAT l, UN_FLOAT s)
{
	UN_FLOAT fSin, fCos;
	CMath::GetSinCosF(h, &fSin, &fCos);

	UN_FLOAT u = s * fSin;
	UN_FLOAT _v = s * fCos;

	SetYUV(l, u, _v);
}

void CColor::GetHLS(UN_FLOAT* h, UN_FLOAT* l, UN_FLOAT* s)
{
	UN_FLOAT fR = BYTE_TO_FLOAT(r);
	UN_FLOAT fG = BYTE_TO_FLOAT(g);
	UN_FLOAT fB = BYTE_TO_FLOAT(b);

	UN_FLOAT _y = SATURATE(0.29891f * fR + 0.58661f * fG + 0.11448f * fB);
	UN_FLOAT _u = fR - _y;
	UN_FLOAT _v = fB - _y;

	SET_COLOR_VAL(l, _y);
	SET_COLOR_VAL(h, atanf(_u / _v));
	SET_COLOR_VAL(s, sqrtf(_u * _u + _v * _v));
}

///////////////////////////////////////////////////////////////
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

void CColor::SetCMY(UN_UINT8 c, UN_UINT8 m, UN_UINT8 y)
{
	r = 255 - c;
	g = 255 - m;
	b = 255 - y;
}

void CColor::GetCMY(UN_UINT8* c, UN_UINT8* m, UN_UINT8* y)
{
	SET_COLOR_VAL(c, 255 - r);
	SET_COLOR_VAL(m, 255 - g);
	SET_COLOR_VAL(y, 255 - b);
}
