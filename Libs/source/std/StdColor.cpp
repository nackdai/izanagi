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
// Y は輝度信号（明るさ）
// U は赤の差分信号 (色差信号)
// V は青の差分信号 (色差信号)

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
// Yの値の範囲の幅が1.0であるが、Uは1.40218、Vは1.77104となり、信号振幅が揃っていない。
// これをそろえるようにスケーリングしたものがYCbCrになる。

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
// Y は輝度信号（明るさ）
// Cb は青の差分信号 (色差信号)
// Cr は赤の差分信号 (色差信号)

// NOTE
//  0.00000 <= Y <= 1.00000
// -0.70109 <= U <= 0.70101
// -0.88552 <= V <= 0.88552
// Yの値の範囲の幅が1.0であるが、Uは1.40218、Vは1.77104となり、信号振幅が揃っていない。
// これをそろえるようにスケーリングしたものがYCbCrになる。
//
//  0.00000 <= Y  <= 1.00000
// -0.50000 <= Cb <= 0.50000
// -0.50000 <= Cr <= 0.50000
// Y、Cb、Crともに値の範囲の幅が1.0になる。

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
// YIQはNTSC(National Television System Committee)方式のカラーテレビ放送で使われるカラーモデルである。
// Y信号は0～4MHz帯域、I信号は1.5MHz帯域、Q信号は0.6MHz帯域である。
// Yはカラー映像を白黒映像にすることに使用できる。(白黒テレビと互換性がある。)
//
// Y : 輝度
// I : 肌色を含む、オレンジからシアンにかけての色調
// Q : I以外の色調

// NOTE
// カラーテレビでは、色差信号を変形し、I、Q信という形で色情報を伝送している。
// I、Q信号は、色差信号を33度回転して作られている。
// I信号はオレンジからシアン系の信号、Q信号は緑からマゼンタ系の信号となる。

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
// CMYは印刷の過程で利用する減法混色の表現法である。
// 絵の具の三原色。基本色は白で、それに色の度合いを加えて、黒色にしていく。
// すなわち、始めは白いキャンバスから始め、インクを加えて暗くしていく
// （反射光を減らす、すなわち減法）ということである。

// NOTE
// CMY -> シアン (Cyan)、マゼンタ (Magenta)、イエロー (Yellow) 

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
