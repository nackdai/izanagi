#if !defined(__IZANAGI_SHADER_MATERIAL_DEFS_FXH__)
#define __IZANAGI_SHADER_MATERIAL_DEFS_FXH__

float4 g_vMtrlDiffuse;
float4 g_vMtrlAmbient;
float4 g_vMtrlSpecular;

float4 GetMtrlDiffuse()  { return g_vMtrlDiffuse; }
float4 GetMtrlAmbient()  { return g_vMtrlAmbient; }
float4 GetMtrlSpecular() { return g_vMtrlSpecular; }

#endif	// #if !defined(__IZANAGI_SHADER_MATERIAL_DEFS_FXH__)
