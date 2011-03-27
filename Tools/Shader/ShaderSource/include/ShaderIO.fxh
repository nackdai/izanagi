#if !defined(__IZANAGI_SHADER_SHADER_IO_FXH__)
#define __IZANAGI_SHADER_SHADER_IO_FXH__

///////////////////////////////////////////////

#ifndef TEXCOORD_TYPE
	#define TEXCOORD_TYPE	float2
#endif

#ifndef TEXCOORD_NUM
	#define TEXCOORD_NUM	1
#endif

#ifdef TEXCOORD_TYPE != float2 && TEXCOORD_TYPE != float4
	// TODO
#endif

#ifdef TEXCOORD_NUM == 0
	// TODO
#endif

///////////////////////////////////////////////
// For VS Input

struct SIZVSInput {
	float4 vPos			: POSITION;

#ifdef HAS_NORMAL
	float3 vNormal		: NORMAL;
#endif

#ifdef HAS_COLOR
	float4 vColor		: COLOR;
#endif

#ifdef HAS_TEXCOORD
	TEXCOORD_TYPE vTexCoord[TEXCOORD_NUM]	: TEXCOORD;
#endif

#ifdef HAS_TANGENT
	float3 vTangent		: TANGENT;
#endif

#ifdef HAS_BINORMAL
	float3 vBiNormal	: BINORMAL;
#endif

#ifdef HAS_BLEND
	float4 vBlendWeight	: BLENDWEIGHT;
	float4 vBlendIndex	: BLENDINDICES;
#endif
};

///////////////////////////////////////////////
// For VS Output / PS Input

struct SIZPSInput {
	float4 vPos			: POSITION;

#ifdef HAS_COLOR
	float4 vColor		: COLOR;
#endif
};

#define SIZVSOutpur SIZPSInput;

///////////////////////////////////////////////

float3 GetVSInputNormal(SIZVSInput sIn)
{
#ifdef HAS_NORMAL
	return sIn.vNormal;
#else
	return 0;
#endif
}

float4 GetVSInputColor(SIZVSInput sIn)
{
#ifdef HAS_COLOR
	return sIn.vColor;
#else
	return 0;
#endif
}

float3 GetVSInputTangent(SIZVSInput sIn)
{
#ifdef HAS_TANGENT
	return sIn.vTangent;
#else
	return 0;
#endif
}

float3 GetVSInputBiNormal(SIZVSInput sIn)
{
#ifdef HAS_BINORMAL
	return sIn.vBiNormal;
#else
	return 0;
#endif
}

float4 GetVSInputBlendWeight(SIZVSInput sIn)
{
#ifdef HAS_BLEND
	return sIn.vBlendWeight;
#else
	return 0;
#endif
}

float4 GetVSInputBlendIndex(SIZVSInput sIn)
{
#ifdef HAS_BLEND
	return sIn.vBlendIndex;
#else
	return 0;
#endif
}

TEXCOORD_TYPE GetVSInputTexCoord(SIZVSInput sIn, int idx)
{
#ifdef HAS_TEXCOORD
	return sIn.vTexCoord[idx];
#else
	return 0;
#endif
}

///////////////////////////////////////////////

#define IS_TEXCOORD_TYPE_FLOAT2		(TEXCOORD_TYPE == float2)
#define IS_TEXCOORD_TYPE_FLOAT4		(TEXCOORD_TYPE == float4)

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_IO_FXH__)
