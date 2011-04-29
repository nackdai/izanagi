#if !defined(__IZANAGI_SHADER_CAMERA_DEFS_FXH__)
#define __IZANAGI_SHADER_CAMERA_DEFS_FXH__

float4 g_vEye;
float4 g_vAt;

float4x4 g_mL2W;
float4x4 g_mW2C;
float4x4 g_mL2C;

float4 GetEye() { return g_vEye; }
float4 GetAt()  { return g_vAt; }

float4x4 GetL2W() { return g_mL2W; }
float4x4 GetW2C() { return g_mW2C; }
float4x4 GetL2C() { return g_mL2C; }

float4 ApplyL2W(float4 v) { return mul(v, g_mL2W); }
float4 ApplyW2C(float4 v) { return mul(v, g_mW2C); }
float4 ApplyL2C(float4 v) { return mul(v, g_mL2C); }

float3 ApplyL2W(float3 v) { return mul(v, (float3x3)g_mL2W); }
float3 ApplyW2C(float3 v) { return mul(v, (float3x3)g_mW2C); }
float3 ApplyL2C(float3 v) { return mul(v, (float3x3)g_mL2C); }

float3 ComputeDirFromEye(float4 v)
{
	return normalize(v.xyz - g_vEye.xyz);
}

#endif	// #if !defined(__IZANAGI_SHADER_CAMERA_DEFS_FXH__)
