#if !defined(__IZANAGI_POSTEFFECT_VS_DEFAULT_FXH__)
#define __IZANAGI_POSTEFFECT_VS_DEFAULT_FXH__

#include "VSCommon.fxh"

// デフォルト頂点シェーダ
S_PE_VS_OUT VSDefault(S_PE_VS_IN sIn)
{
	S_PE_VS_OUT sOut;

	// 頂点位置計算
	sOut.vPos = _ComputeVSPosition(sIn.vPos);

	// テクスチャ座標
	//	out = in * scale + offset
	sOut.vUV.xy = sIn.vUV.xy * g_vTexParam.xy + g_vTexParam.zw;

	return sOut;
}

#endif	// #if !defined(__IZANAGI_POSTEFFECT_VS_DEFAULT_FXH__)
