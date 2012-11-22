#if !defined(__IZANAGI_SHADER_LIGHT_UTIL_FXH__)
#define __IZANAGI_SHADER_LIGHT_UTIL_FXH__

#include "CameraDefs.fxh"
#include "LightDefs.fxh"
#include "MaterialDefs.fxh"

/**
* Halfベクトル計算
*/
float3 ComputeHalfVectorEx(int idxParallelLight, float4 vPos_L2W)
{
	// 視点からの方向
	float3 vV = ComputeDirFromEye(vPos_L2W);
	return normalize(GetParallelLightDir(idxParallelLight) + vV);
}

/**
* Halfベクトル計算
*/
float3 ComputeHalfVector(int idxParallelLight, float4 vPos)
{
	return ComputeHalfVectorEx(idxParallelLight, ApplyL2W(vPos));
}

/**
* 環境光計算
*/
float4 ComputeAmbientLight()
{
	return GetMtrlAmbient() * GetAmbientLightColor();
}

/**
* Phongライト計算
*/
float3 ComputePhongLight(
	int idxParallelLight,
	float3 vNormal,
	float3 vHalf)
{
	float4 sParallelLightClr = GetParallelLightColor(idxParallelLight);
	float3 sParallelLightDir = GetParallelLightDir(idxParallelLight);
	
	// Diffuse = Md * ∑(C * max(N・L, 0))
	float3 ret = GetMtrlDiffuse().rgb * sParallelLightClr.rgb * max(0.0f, dot(vNormal, sParallelLightDir));

	// Specular = Ms * ∑(C * pow(max(N・H, 0), m))
	ret += GetMtrlSpecular().rgb * sParallelLightClr.rgb * pow(max(0.0f, dot(vNormal, vHalf)), GetMtrlSpecular().w);

	return ret;
}

#endif	// #if !defined(__IZANAGI_SHADER_LIGHT_UTIL_FXH__)
