#if !defined(__IZANAGI_SHADER_LIGHT_UTIL_FXH__)
#define __IZANAGI_SHADER_LIGHT_UTIL_FXH__

#include "CameraDefs.fxh"
#include "LightDefs.fxh"
#include "MaterialDefs.fxh"

/**
* Half�x�N�g���v�Z
*/
float3 ComputeHalfVectorEx(int idxParallelLight, float4 vPos_L2W)
{
	// ���_����̕���
	float3 vV = ComputeDirFromEye(vPos_L2W);
	return normalize(GetParallelLightDir(idxParallelLight) + vV);
}

/**
* Half�x�N�g���v�Z
*/
float3 ComputeHalfVector(int idxParallelLight, float4 vPos)
{
	return ComputeHalfVectorEx(idxParallelLight, ApplyL2W(vPos));
}

/**
* �����v�Z
*/
float4 ComputeAmbientLight()
{
	return GetMtrlAmbient() * GetAmbientLightColor();
}

/**
* Phong���C�g�v�Z
*/
float3 ComputePhongLight(
	int idxParallelLight,
	float3 vNormal,
	float3 vHalf)
{
	float4 sParallelLightClr = GetParallelLightColor(idxParallelLight);
	float3 sParallelLightDir = GetParallelLightDir(idxParallelLight);
	
	// Diffuse = Md * ��(C * max(N�EL, 0))
	float3 ret = GetMtrlDiffuse().rgb * sParallelLightClr.rgb * max(0.0f, dot(vNormal, sParallelLightDir));

	// Specular = Ms * ��(C * pow(max(N�EH, 0), m))
	ret += GetMtrlSpecular().rgb * sParallelLightClr.rgb * pow(max(0.0f, dot(vNormal, vHalf)), GetMtrlSpecular().w);

	return ret;
}

#endif	// #if !defined(__IZANAGI_SHADER_LIGHT_UTIL_FXH__)
