#if !defined(__IZANAGI_POSTEFFECT_VS_COMMON_FXH__)
#define __IZANAGI_POSTEFFECT_VS_COMMON_FXH__

#include "../PostEffectShaderIO.fxh"

/********************************************
   頂点シェーダ共通処理
********************************************/

// 頂点シェーダ入力
struct S_PE_VS_IN {
	float4 vPos	: POSITION;
	float2 vUV	: TEXCOORD0;
};

///////////////////////////////////////////
// 頂点位置計算

// 頂点位置オフセット
float4 g_vPosOffset = (float4)0.0f;

// 頂点位置計算
float4 _ComputeVSPosition(in float4 vPos)
{
	float4 vRet = vPos;

	// [0.0f, 0.0f] - [1.0f, 1.0f] -> [-1.0f, -1.0f] - [1.0f, 1.0f]
	vRet.xy = (vRet.xy + g_vPosOffset.xy) * 2.0f - 1.0f;

	// さらにY座標は反転させる
	vRet.y *= -1.0f;

	return vRet;
}

///////////////////////////////////////////
// テクスチャ座標計算

// xy : scale
// zw : offset
float4 g_vTexParam = {
	1.0f, 1.0f, 0.0f, 0.0f
};

// スクリーンサイズ
float2 g_vQuadScreenSize : VIEWPORTPIXELSIZE;

///////////////////////////////////////////
// 頂点シェーダ出力

#define S_PE_VS_OUT				S_PE_PS_IN
#define S_PE_VS_OUT_SAMPLING_4	S_PE_PS_IN_SAMPLING_4
#define S_PE_VS_OUT_SAMPLING_8	S_PE_PS_IN_SAMPLING_8
#define S_PE_VS_OUT_SAMPLING_16	S_PE_PS_IN_SAMPLING_16
#define S_PE_VS_OUT_SAMPLING_5	S_PE_PS_IN_SAMPLING_5
#define S_PE_VS_OUT_SAMPLING_7	S_PE_PS_IN_SAMPLING_7
#define S_PE_VS_OUT_SAMPLING_9	S_PE_PS_IN_SAMPLING_9
#define S_PE_VS_OUT_SAMPLING_13	S_PE_PS_IN_SAMPLING_13
#define S_PE_VS_OUT_SAMPLING_15	S_PE_PS_IN_SAMPLING_15

#endif	// #if !defined(__IZANAGI_POSTEFFECT_VS_COMMON_FXH__)
