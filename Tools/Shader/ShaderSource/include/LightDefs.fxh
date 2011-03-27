#if !defined(__IZANAGI_SHADER_LIGHT_DEFS_FXH__)
#define __IZANAGI_SHADER_LIGHT_DEFS_FXH__

#define SVector	float4

////////////////////////////////////////////////////////////

/**
* ���s����
*/
struct SParallelLightParam {
	SVector vDir;
	SVector color;
};

/**
* ����
*/
struct SAmbientLightParam {
	SVector color;
};

/**
* �_����
*/
struct SPointLightParam {
	SVector vPos;
	SVector color;
	SVector attn;	// 0 - 2 : attn
};

/**
* �X�|�b�g���C�g
*/
struct SSpotLightParam {
	SVector vPos;
	SVector vDir;
	SVector color;
	SVector param;	// 0 : power / 1 - 3 : attn
};

/**
* �������C�g
*/
struct SHemisphereLightParam {
	SVector vAxis;
	SVector upColor;
	SVector downColor;
};

////////////////////////////////////////////////////////////

// ���s����
#ifdef IZ_SHADER_PARALLEL_LIGHT_NUM && IZ_SHADER_PARALLEL_LIGHT_NUM > 0
	float3 g_ParallelLightDir[IZ_SHADER_PARALLEL_LIGHT_NUM];
	float4 g_ParallelLightClr[IZ_SHADER_PARALLEL_LIGHT_NUM];

	#define GetParallelLightDir(idx)	g_ParallelLightDir[idx]
	#define GetParallelLightColor(idx)	g_ParallelLightClr[idx]
#else
	#define GetParallelLightDir(idx)	((float3)0)
	#define GetParallelLightColor(idx)	((float4)0)
#endif

// ����
#ifdef IZ_SHADER_ENABLE_AMBIENT_LIGHT
	float4 g_AmbientLightColor;
	#define GetAmbientLightColor()	g_AmbientLightColor
#else	// #ifdef IZ_SHADER_ENABLE_AMBIENT_LIGHT
	#define GetAmbientLightColor()	((float4)0)
#endif	// #ifdef IZ_SHADER_ENABLE_AMBIENT_LIGHT

#define GetAmbientColor()	GetAmbientLight().color

// �_����
#ifdef IZ_SHADER_POINT_LIGHT_NUM && IZ_SHADER_POINT_LIGHT_NUM > 0
	float4 g_PointLightPos[IZ_SHADER_POINT_LIGHT_NUM];
	float4 g_PointLightClr[IZ_SHADER_POINT_LIGHT_NUM];
	float4 g_PointLightAttn[IZ_SHADER_POINT_LIGHT_NUM];

	#define GetPointLightDir(idx)	g_PointLightPos[idx]
	#define GetPointLightColor(idx)	g_PointLightClr[idx]
	#define GetPointLightAttn(idx)	g_PointLightAttn[idx]
#else
	#define GetPointLightDir(idx)	((float4)0)
	#define GetPointLightColor(idx)	((float4)0)
	#define GetPointLightAttn(idx)	((float4)0)
#endif

// �X�|�b�g���C�g
#ifdef IZ_SHADER_SPOT_LIGHT_NUM && IZ_SHADER_SPOT_LIGHT_NUM > 0
	float4 g_SpotLightPos[IZ_SHADER_SPOT_LIGHT_NUM];
	float3 g_SpotLightDir[IZ_SHADER_SPOT_LIGHT_NUM];
	float4 g_SpotLightClr[IZ_SHADER_SPOT_LIGHT_NUM];
	float4 g_SpotLightParam[IZ_SHADER_SPOT_LIGHT_NUM];

	#define GetSporLightPos(idx)	g_SpotLightPos[idx]
	#define GetSporLightDir(idx)	g_SpotLightDir[idx]
	#define GetSporLightClr(idx)	g_SpotLightClr[idx]
	#define GetSporLightParam(idx)	g_SpotLightParam[idx]
#else
	#define GetSporLightPos(idx)	((float4)0)
	#define GetSporLightDir(idx)	((float3)0)
	#define GetSporLightClr(idx)	((float4)0)
	#define GetSporLightParam(idx)	((float4)0)
#endif

// �������C�g
#ifdef IZ_SHADER_HEMISPHERE_LIGHT_NUM && IZ_SHADER_HEMISPHERE_LIGHT_NUM > 0
	float3 g_HemisphereLightAxis[IZ_SHADER_HEMISPHERE_LIGHT_NUM];
	float4 g_HemisphereLightUpClr[IZ_SHADER_HEMISPHERE_LIGHT_NUM];
	float4 g_HemisphereLightDownClr[IZ_SHADER_HEMISPHERE_LIGHT_NUM];

	#define GetHemisphereLightAxis(idx)		g_HemisphereLightAxis[idx]
	#define GetHemisphereLightUpClr(idx)	g_HemisphereLightUpClr[idx]
	#define GetHemisphereLightDownClr(idx)	g_HemisphereLightDownClr[idx]
#else
	#define GetHemisphereLightAxis(idx)		((float3)0)
	#define GetHemisphereLightUpClr(idx)	((float4)0)
	#define GetHemisphereLightDownClr(idx)	((float4)0)
#endif

#endif	// #if !defined(__IZANAGI_SHADER_LIGHT_DEFS_FXH__)
