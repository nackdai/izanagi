#if !defined(__IZANAGI_POSTEFFECT_SAMPLING_PARAM_FXH__)
#define __IZANAGI_POSTEFFECT_SAMPLING_PARAM_FXH__

#include "ShaderDefs/ShaderDefsInternal.fxh"

// サンプリングパラメータ

///////////////////////////////////////////////////
// 1/4 x 1/4 縮小サンプリングオフセット
// リニアフィルタ

// NOTE
// +---+---+---+---+  +---+---+---+---+
// |   |   |   |   |  |   |   |   |   |
// +---@---+---@---+  +---0---+---1---+
// |   |   |   |   |  |   |   |   |   |
// +---+---x---+---+  +---+---x---+---+
// |   |   |   |   |  |   |   |   |   |
// +---@-------@---+  +---2---+---3---+
// |   |   |   |   |  |   |   |   |   |
// +---+---+---+---+  +---+---+---+---+

PRIVATE(const float4 g_vDownScale4x4SampleOffset_Linear[] : TEX_OFFSET) = {
	{
		-1.0f, -1.0f,	// 0
	  	 1.0f, -1.0f	// 1
	},
	{
		-1.0f, 1.0f,	// 2
		 1.0f, 1.0f		// 3
	},
};

///////////////////////////////////////////////////
// 1/4 x 1/4 縮小サンプリングオフセット
// ポイントフィルタ

// NOTE
// +---+---+---+---+  +---+---+---+---+
// | @ | @ | @ | @ |  | 0 | 1 | 2 | 3 |
// +---+---+---+---+  +---+---+---+---+
// | @ | @ | @ | @ |  | 4 | 5 | 6 | 7 |
// +---+---x---+---+  +---+---x---+---+
// | @ | @ | @ | @ |  | 8 | 9 | 10| 11|
// +---+-------+---+  +---+---+---+---+
// | @ | @ | @ | @ |  | 12| 13| 14| 15|
// +---+---+---+---+  +---+---+---+---+

PRIVATE(float4 g_vDownScale4x4SampleOffset_Point[] : TEX_OFFSET) = {
	{
		-1.5f, -1.5f,	// 0
		-0.5f, -1.5f,	// 1
	},
	{
		 0.5f, -1.5f,	// 2
		 1.5f, -1.5f,	// 3
	},
	{
		-1.5f, -0.5f,	// 4
		-0.5f, -0.5f,	// 5
	},
	{
		 0.5f, -0.5f,	// 6
		 1.5f, -0.5f,	// 7
	},
	{
		-1.5f,  0.5f,	// 8
		-0.5f,  0.5f,	// 9
	},
	{
		 0.5f,  0.5f,	// 10
		 1.5f,  0.5f,	// 11
	},
	{
		-1.5f,  1.5f,	// 12
		-0.5f,  1.5f,	// 13
	},
	{
		 0.5f,  1.5f,	// 14
		 1.5f,  1.5f,	// 15
	},
};

///////////////////////////////////////////////////
// 1/2 x 1/2 縮小サンプリングオフセット
// リニアフィルタ

// NOTE
// +---+---+
// |   |   |
// +---x---+
// |   |   |
// +---+---+

PRIVATE(float4 g_vDownScale2x2SampleOffset_Linear : TEX_OFFSET) = {
	0.0f, 0.0f, 0.0f, 0.0f,
};

///////////////////////////////////////////////////
// 1/2 x 1/2 縮小サンプリングオフセット
// ポイントフィルタ

// NOTE
// +---+---+  +---+---+
// | @ | @ |  | 0 | 1 |
// +---x---+  +---x---+
// | @ | @ |  | 2 | 3 |
// +---+---+  +---+---+

PRIVATE(float4 g_vDownScale2x2SampleOffset_Point[] : TEX_OFFSET) = {
	{
		-0.5f, -0.5f,	// 0
		 0.5f, -0.5f	// 1
	},
	{
		-0.5f, 0.5f,	// 2
		 0.5f, 0.5f		// 3
	},
};

///////////////////////////////////////////////////
// 1/3 x 1/3 縮小サンプリングオフセット
// リニアフィルタ

// NOTE
// +---+---+---+     +-------+--
// | +-+---+---+---> |       |
// +---+---+---+     |   x   |
// |   |   |   |     |      @|
// +---+---+---+     +-------+--
// |   |   |   |     |       |
// +---+---+---+

// NOTE
//             |
//         @---+--
//         |   | |<-1/6
// --------+---+--
//         |<->|
//          1/6

// NOTE
// +---+---+---+  +---+---+---+
// |  @|   |@  |  |  0|   |1  |
// +---+---+---+  +---+---+---+
// |   | x |   |  |   | x |   |
// +---+---+---+  +---+---+---+
// |  @|   |@  |  |  2|   |3  |
// +---+---+---+  +---+---+---+

// NOTE
// 1/2 + 1/6 = 2/3

PRIVATE(float4 g_vDownScale3x3SampleOffset_Linear[] : TEX_OFFSET) = {
	{
		-2.0f / 3.0f, -2.0f / 3.0f,	// 0
		 2.0f / 3.0f, -2.0f / 3.0f	// 1
	},
	{
		-2.0f / 3.0f, 2.0f / 3.0f,	// 2
		 2.0f / 3.0f, 2.0f / 3.0f	// 3
	},
};

///////////////////////////////////////////////////
// 1/3 x 1/3 縮小サンプリングオフセット
// ポイントフィルタ

// NOTE
// +---+---+---+  +---+---+---+
// | @ | @ | @ |  | 0 | 1 | 2 |
// +---+---+---+  +---+---+---+
// | @ | x | @ |  | 3 | 4 | 5 |
// +---+---+---+  +---+---+---+
// | @ | @ | @ |  | 6 | 7 | 8 |
// +---+---+---+  +---+---+---+

PRIVATE(float4 g_vDownScale3x3SampleOffset_Point[] : TEX_OFFSET) = {
	{
		-1.0f, -1.0f,	// 0
		 0.0f, -1.0f,	// 1
	},
	{
		 1.0f, -1.0f,	// 2
		-1.0f,  0.0f,	// 3
	},
	{
		 0.0f,  0.0f,	// 4
		 1.0f,  0.0f,	// 5
	},
	{
		-1.0f,  1.0f,	// 6
		 0.0f,  1.0f,	// 7
	},
	{
		 1.0f,  1.0f,	// 8
		 0.0f,  0.0f,	// dummy
	},
};

///////////////////////////////////////////////////
// 5 x 5 ガウスブラーサンプリングオフセット

// NOTE
//         +---+
//         | x |
//     +---+---+---+
//     | x | x | x |
// +---+---+---+---+---+
// | x | x | @ | x | x |
// +---+---+---+---+---+
//     | x | x | x |
//     +---+---+---+
//         | x |
//         +---+
//
// 円状にサンプリングする

// NOTE
//         +---+
//         | 0 |
//     +---+---+---+
//     | 1 | 2 | 3 |
// +---+---+---+---+---+
// | 4 | 5 | 6 | 7 | 8 |
// +---+---+---+---+---+
//     | 9 | 10| 11|
//     +---+---+---+
//         | 12|
//         +---+

PRIVATE(float4 g_vGaussBlur5x5SampleOffset[] : TEX_OFFSET) = {
	{
		 0.0f, -2.0f,	// 0
		-1.0f, -1.0f,	// 1
	},
	{
		 0.0f, -1.0f,	// 2
		 1.0f, -1.0f,	// 3
	},
	{
		-2.0f,  0.0f,	// 4
		-1.0f,  0.0f,	// 5
	},
	{
		 0.0f,  0.0f,	// 6
		 1.0f,  0.0f,	// 7
	},
	{
		 2.0f,  0.0f,	// 8
		-1.0f,  1.0f,	// 9
	},
	{
		 0.0f,  1.0f,	// 10
		 1.0f,  1.0f,	// 11
	},
	{
		 0.0f,  2.0f,	// 12
		 0.0f,  0.0f,	// dummy
	},
};

///////////////////////////////////////////////////
// 5 x 5 ガウスブラーウエイト

// NOTE
// ガウス関数
// f(x) = c * exp(-(x * x) / (2 * σ * σ))
// (c : 規格化定数 / σ * σ : 分散)
//
//       ｜
//       ○
//     ○○○
// ‐○○●○○→x
//     ○○○
//       ○
//       ↓y
// 円状にサンプリングする

// NOTE
// ↓C++ソースコード
//MY_FLOAT _GaussianDistribution(
//	MY_FLOAT x, MY_FLOAT y, 
//	MY_FLOAT rho)	// 分散
//{
//
//	// 円状になるようなウエイト値になるよう(?)に 1 / 2π の係数
//	MY_FLOAT g = 1.0f / sqrtf(MATH_PI2 * rho * rho);
//
//	// x * x + y * y = r * r -> 円の式？
//	g *= expf(-(x * x + y * y) / (2 * rho * rho));
//
//	return g;
//}
//
//void CPostEffectSamplingUtil::GetGaussBlur5x5Weight(
//	MY_FLOAT pWeight[],
//	MY_FLOAT fMultiplier,	// ウエイトへの乗数
//	MY_FLOAT fDeviation)	// ウエイト計算時に使用する偏差
//{
//	MY_FLOAT fTotalWeight = 0.0f;
//	MY_UINT nIdx = 0;
//
//	for (MY_INT y = -2; y <= 2; y++) {
//		for (MY_INT x = -2; x <= 2; x++) {
//			if (abs(y) + abs(x) > 2) {
//				continue;
//			}
//
//			// ウエイト
//			MY_FLOAT fGauss = _GaussianDistribution(
//								(MY_FLOAT)x,
//								(MY_FLOAT)y,
//								fDeviation);
//
//			pWeight[nIdx] = fGauss;
//			fTotalWeight += fGauss;
//
//			nIdx++;
//		}
//	}
//
//	MY_FLOAT d = fMultiplier / fTotalWeight;
//	for(MY_UINT i = 0; i < nIdx; i++) {
//		pWeight[i] *= d;
//	}
//}

// NOTE
// fMultiplier = 1.0f
// fDeviation = 1.0f

static const float g_vGaussBlur5x5Weight[] = {
	0.024882466f,
	0.067637555f,
	0.11151548f,
	0.067637555f,
	0.024882466f,
	0.11151548f,
	0.18385795f,
	0.11151548f,
	0.024882466f,
	0.067637555f,
	0.11151548f,
	0.067637555f,
	0.024882466f,
};

///////////////////////////////////////////////////
// ブルームブラーサンプリングオフセット
// ポイントフィルタ

// NOTE
// +---+---+---+---+---+---+---+---+---+---+---+---+---+
// | x | x | x | x | x | x | @ | x | x | x | x | x | x |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+

// 縦
PRIVATE(float4 g_vBloomBlurSampleOffsetV_Point[] : TEX_OFFSET) = {
	{
		0.0f,  0.0f,
		0.0f, -1.0f,
	},
	{
		0.0f, -2.0f,
		0.0f, -3.0f,
	},
	{
		0.0f, -4.0f,
		0.0f, -5.0f,
	},
	{
		0.0f, -6.0f,
		0.0f,  1.0f,
	},
	{
		0.0f,  2.0f,
		0.0f,  3.0f,
	},
	{
		0.0f,  4.0f,
		0.0f,  5.0f,
	},
	{
		0.0f,  6.0f,
		0.0f,  0.0f,	// dummy
	},
};

// 横
PRIVATE(float4 g_vBloomBlurSampleOffsetH_Point[] : TEX_OFFSET) = {
	{
		 0.0f, 0.0f,
		-1.0f, 0.0f,
	},
	{
		-2.0f, 0.0f,
		-3.0f, 0.0f,
	},
	{
		-4.0f, 0.0f,
		-5.0f, 0.0f,
	},
	{
		-6.0f, 0.0f,
		 1.0f, 0.0f,
	},
	{
		 2.0f, 0.0f,
		 3.0f, 0.0f,
	},
	{
		 4.0f, 0.0f,
		 5.0f, 0.0f,
	},
	{
		 6.0f, 0.0f,
		 0.0f, 0.0f,	// dummy
	},
};

///////////////////////////////////////////////////
// ブルームブラーサンプリングウエイト
// ポイントフィルタ

// NOTE
// +---+---+---+---+---+---+---+---+---+---+---+---+---+
// | x | x | x | x | x | x | @ | x | x | x | x | x | x |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+

// NOTE
// ↓C++ソースコード
//void _Get1DirectionGaussBlurWeight_Point(
//	MY_FLOAT pWeight[],
//	MY_FLOAT fMultiplier,
//	MY_FLOAT fDeviation)
//{
//	pWeight[0] = fMultiplier * _GaussianDistribution(0.0f, 0.0f, fDeviation);
//
//	for (MY_UINT i = 1; i < 7; i++) {
//		pWeight[i] = fMultiplier;
//
//		// NOTE
//		// 計算結果はどちらが０であっても同じになる
//		pWeight[i] *= _GaussianDistribution(
//						(MY_FLOAT)i, 0.0f,
//						fDeviation);
//	}
//
//	// 上の部分では半分なので、逆側半分にコピーする
//	for (MY_UINT i = 7; i < 13; i++) {
//		pWeight[i] = pWeight[i - 6];
//	}
//}

static const float g_vBloomBlurWeight_Point[] = {
	0.59841347f,
	0.52809799f,
	0.36295611f,
	0.19427639f,
	0.080986455f,
	0.026292451f,
	0.0066477726f,
	0.52809799f,
	0.36295611f,
	0.19427639f,
	0.080986455f,
	0.026292451f,
	0.0066477726f,
};

///////////////////////////////////////////////////
// ブルームブラーサンプリングオフセット
// リニアフィルタ

// NOTE
// +---+---+---+---+---+---+---+---+---+---+---+---+---+
// | x | x | x | x | x | x | @ | x | x | x | x | x | x |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+
// |<----->|<----->|<----->|   |<----->|<----->|<----->|
// 上のように２テクセルずつまとめる

// NOTE
// +-------+-------+ 
// |       |       |
// |   2   |@  1   |
// |       |       |
// +-------+-------+ 
// １と２の間をウエイト値で線形補間したものがオフセットになる
// １からのオフセットになる
//   Offset = Weight2 / (Weight1 + Weight2)

// NOTE
// ↓C++ソースコード
//void _Get1DirectionGaussBlurSampleOffset_Linear(
//	S_FLOAT_VECTOR4 pOffset[],
//	const MY_FLOAT pWeight[],
//	MY_INT nTexSize,
//	MY_BOOL bIsVertical)
//{
//	// ウエイト値を元にオフセットを計算する
//	MY_FLOAT fOffsetArray[7];
//	{
//		fOffsetArray[0] = 0.0f;
//
//		MY_UINT nWeightPos = 1;
//		for (MY_UINT i = 1; i < COUNTOF(fOffsetArray); i++) {
//			fOffsetArray[i] = pWeight[nWeightPos + 1] / (pWeight[nWeightPos] + pWeight[nWeightPos + 1]);
//			nWeightPos += 2;
//		}
//	}
//
//	// 最終的なオフセットを計算
//	pOffset[0].x = 0.0f;
//	pOffset[0].y = 0.0f;
//
//	pOffset[0].z = 1.0f + fOffsetArray[1];
//	pOffset[0].w = 1.0f + fOffsetArray[1];
//
//	pOffset[1].x = 3.0f + fOffsetArray[2];
//	pOffset[1].y = 3.0f + fOffsetArray[2];
//
//	pOffset[1].z = 5.0f + fOffsetArray[3];
//	pOffset[1].w = 5.0f + fOffsetArray[3];
//
//	pOffset[2].x = -1.0f - fOffsetArray[4];
//	pOffset[2].y = -1.0f - fOffsetArray[4];
//
//	pOffset[2].z = -3.0f - fOffsetArray[5];
//	pOffset[2].w = -3.0f - fOffsetArray[5];
//
//	pOffset[3].x = -5.0f - fOffsetArray[6];
//	pOffset[3].y = -5.0f - fOffsetArray[6];
//
//	const MY_FLOAT fDiv = 1.0f / nTexSize;
//
//	for (MY_UINT i = 0; i < 4; i++) {
//		CMath::VectorScale(
//			pOffset[i],
//			pOffset[i],
//			fDiv);
//
//		if (bIsVertical) {
//			pOffset[i].x = pOffset[i].z = 0.0f;
//		}
//		else {
//			pOffset[i].y = pOffset[i].w = 0.0f;
//		}
//	}
//}

// 縦
PRIVATE(float4 g_vBloomBlurSampleOffsetV_Linear[] : TEX_OFFSET) = {
	{
		0.00000000f,  0.00000000f, 
		0.00000000f,  1.4073334f,  
	},	
	{	
		0.00000000f,  3.2942150f,  
		0.00000000f,  5.2018132f,  
	},	
	{	
		0.00000000f, -1.4073334f,  
		0.00000000f, -3.2942150f,  
	},	
	{	
		0.00000000f, -5.2018132f,  
		0.00000000f,  0.00000000f,	// dummy
	},
};

// 横
PRIVATE(float4 g_vBloomBlurSampleOffsetH_Linear[] : TEX_OFFSET) = {
	{
		 0.00000000f, 0.00000000f,
		 1.4073334f,  0.00000000f,
	},
	{
		 3.2942150f,  0.00000000f,
		 5.2018132f,  0.00000000f,
	},
	{
		-1.4073334f,  0.00000000f,
		-3.2942150f,  0.00000000f,
	},
	{
		-5.2018132f,  0.00000000f,
		 0.00000000f, 0.00000000f,	// dummy
	},
};

///////////////////////////////////////////////////
// ブルームブラーサンプリングウエイト
// リニアフィルタ

// NOTE
// +---+---+---+---+---+---+---+---+---+---+---+---+---+
// | x | x | x | x | x | x | @ | x | x | x | x | x | x |
// +---+---+---+---+---+---+---+---+---+---+---+---+---+
// |<----->|<----->|<----->|   |<----->|<----->|<----->|
// 上のように２テクセルずつまとめる

// NOTE
// ↓C++ソースコード
//void _Get1DirectionGaussBlurWeight_Linear(
//	MY_FLOAT pWeight[],
//	MY_FLOAT fMultiplier,
//	MY_FLOAT fDeviation)
//{
//	// まずはポイントフィルタの場合のウエイトを計算する
//	MY_FLOAT fWeightArray[13];
//	_Get1DirectionGaussBlurWeight_Point(
//		fWeightArray,
//		fMultiplier,
//		fDeviation);
//
//	pWeight[0] = fWeightArray[0];
//
//	for (MY_UINT i = 0; i < 7; i++) {
//		pWeight[i + 1] = fWeightArray[i * 2 + 1] + fWeightArray[i * 2 + 2];
//	}
//}

static const float g_vBloomBlurWeight_Linear[] = {
	0.59841347f,
	0.89105409f,
	0.27526283f,
	0.032940224f,
	0.89105409f,
	0.27526283f,
	0.032940224f,
};

///////////////////////////////////////////////////
// ９点サンプリング
// リニアフィルタ

// NOTE
// +---+---+---+   +---+---+---+
// |   |   @   |   |   |   2   |
// +-@-+---+---+   +-0-+---+---+
// |   | x |   |   |   | x |   |
// +---+---+-@-+   +---+---+-1-+
// |   @   |   |   |   3   |   |
// +---+---+---+   +---+---+---+

PRIVATE(float4 g_v9PointSampleOffset_Linear[] : TEX_OFFSET) = {
	{
		-1.0f, -0.5f,	// 0
		 1.0f,  0.5f,	// 1
	},
	{
		 0.5f, -1.0f,	// 2
		-0.5f,  1.0f,	// 3
	},
};

///////////////////////////////////////////////////
// ９点サンプリング
// ポイントフィルタ

// NOTE
// +---+---+---+  +---+---+---+
// | @ | @ | @ |  | 0 | 1 | 2 |
// +---+---+---+  +---+---+---+
// | @ | x | @ |  | 3 | 4 | 5 |
// +---+---+---+  +---+---+---+
// | @ | @ | @ |  | 6 | 7 | 8 |
// +---+---+---+  +---+---+---+

PRIVATE(float4 g_v9PointSampleOffset_Point[] : TEX_OFFSET) = {
	{
		-1.0f, -1.0f,	// 0
		 0.0f, -1.0f,	// 1
	},
	{
		 1.0f, -1.0f,	// 2
		-1.0f,  0.0f,	// 3
	},
	{
		 0.0f,  0.0f,	// 4
		 1.0f,  0.0f,	// 5
	},
	{
		-1.0f,  1.0f,	// 6
		 0.0f,  1.0f,	// 7
	},
	{
		 1.0f,  1.0f,	// 8
		 0.0f,  0.0f,	// dummy
	},
};

#endif	// #if !defined(__IZANAGI_POSTEFFECT_SAMPLING_PARAM_FXH__)
