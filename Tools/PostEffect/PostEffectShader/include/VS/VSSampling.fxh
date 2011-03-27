#if !defined(__IZANAGI_POSTEFFECT_VS_POINT_SAMPLING_FXH__)
#define __IZANAGI_POSTEFFECT_VS_POINT_SAMPLING_FXH__

#include "VSCommon.fxh"

// サンプリングオフセット
float4 g_vUVOffsetSampling[16];

//////////////////////////////////////////////
// 頂点シェーダ
// 1ポイントサンプリング（デフォルト）

S_PE_VS_OUT VSSampling_1(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	// out = in * scale + offset
	sOut.vUV.xy = sIn.vUV.xy * g_vTexParam.xy + g_vTexParam.zw;

	return sOut;
}

//////////////////////////////////////////////
// 頂点シェーダ
// 偶数ポイントサンプリング

// 偶数ポイントサンプリング　テクスチャ座標計算
// NOTE : out = in * scale + offset
#define _COMPUTE_VS_EVEN_SAMPLING_TEXCOORD(NUM)\
	for (int i = 0; i < NUM; i++) {\
		sOut.vUV[i].xyzw = sIn.vUV.xyxy + g_vUVOffsetSampling[i].xyzw;\
		sOut.vUV[i].xyzw = sOut.vUV[i].xyzw * g_vTexParam.xyxy + g_vTexParam.zwzw;\
	}

// 4ポイントサンプリング
S_PE_VS_OUT_SAMPLING_4 VSSampling_4(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT_SAMPLING_4 sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	_COMPUTE_VS_EVEN_SAMPLING_TEXCOORD(NUM_SAMPLING_4);

	return sOut;
}

// 8ポイントサンプリング
S_PE_VS_OUT_SAMPLING_8 VSSampling_8(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT_SAMPLING_8 sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	_COMPUTE_VS_EVEN_SAMPLING_TEXCOORD(NUM_SAMPLING_8);

	return sOut;
}

// 16ポイントサンプリング
S_PE_VS_OUT_SAMPLING_16 VSSampling_16(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT_SAMPLING_16 sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	_COMPUTE_VS_EVEN_SAMPLING_TEXCOORD(NUM_SAMPLING_16);

	return sOut;
}

//////////////////////////////////////////////
// 頂点シェーダ
// 奇数ポイントサンプリング

// 奇数ポイントサンプリング　テクスチャ座標計算
// NOTE : out = in * scale + offset
#define _COMPUTE_VS_ODD_SAMPLING_TEXCOORD(NUM)\
	{\
		for (int i = 0; i < NUM; i++) {\
			sOut.vUV[i].xyzw = sIn.vUV.xyxy + g_vUVOffsetSampling[i].xyzw;\
			sOut.vUV[i].xyzw = sOut.vUV[i].xyzw * g_vTexParam.xyxy + g_vTexParam.zwzw;\
		}\
		sOut.vUV_0.xy = sIn.vUV.xy + g_vUVOffsetSampling[NUM].xy;\
		sOut.vUV_0.xy = sOut.vUV_0.xy * g_vTexParam.xy + g_vTexParam.zw;\
	}

// 5ポイントサンプリング
S_PE_VS_OUT_SAMPLING_5 VSSampling_5(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT_SAMPLING_5 sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	_COMPUTE_VS_ODD_SAMPLING_TEXCOORD(NUM_SAMPLING_5);

	return sOut;
}

// 7ポイントサンプリング
S_PE_VS_OUT_SAMPLING_7 VSSampling_7(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT_SAMPLING_7 sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	_COMPUTE_VS_ODD_SAMPLING_TEXCOORD(NUM_SAMPLING_7);

	return sOut;
}

// 9ポイントサンプリング
S_PE_VS_OUT_SAMPLING_9 VSSampling_9(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT_SAMPLING_9 sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	_COMPUTE_VS_ODD_SAMPLING_TEXCOORD(NUM_SAMPLING_9);

	return sOut;
}

// 13ポイントサンプリング
S_PE_VS_OUT_SAMPLING_13 VSSampling_13(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT_SAMPLING_13 sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	_COMPUTE_VS_ODD_SAMPLING_TEXCOORD(NUM_SAMPLING_13);

	return sOut;
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_VS_POINT_SAMPLING_FXH__)
