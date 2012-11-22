#include "ShaderConverterUtil.h"
#include "izToolKit.h"

namespace {
	izanagi::E_SHADER_PARAMETER_CLASS ClassTable[] = {
		izanagi::E_SHADER_PARAMETER_CLASS_UNKNOWN,		// その他
		izanagi::E_SHADER_PARAMETER_CLASS_SCALAR,		// スカラー
		izanagi::E_SHADER_PARAMETER_CLASS_VECTOR,		// ベクトル
		izanagi::E_SHADER_PARAMETER_CLASS_MATRIX,		// マトリクス
		izanagi::E_SHADER_PARAMETER_CLASS_STRUCT,		// 構造体
		izanagi::E_SHADER_PARAMETER_CLASS_ARRAY,		// 配列
		izanagi::E_SHADER_PARAMETER_CLASS_SAMPLER,		// サンプラ
		izanagi::E_SHADER_PARAMETER_CLASS_OBJECT		// テクスチャ、シェーダ、または文字列
	};
}	// namespace

/**
* CGparameterclass -> E_SHADER_PARAMETER_CLASS
*/
izanagi::E_SHADER_PARAMETER_CLASS CShaderConvUtil::CgParamClassToIzanagiShaderParamClass(CGparameterclass nClass)
{
	IZ_ASSERT(nClass < COUNTOF(ClassTable));
	
	izanagi::E_SHADER_PARAMETER_CLASS ret = ClassTable[nClass];

	return ret;
}

namespace {
	izanagi::E_SHADER_PARAMETER_TYPE TypeTable[] = {
		// HALF
		izanagi::E_SHADER_PARAMETER_TYPE_HALF,  izanagi::E_SHADER_PARAMETER_TYPE_HALF2,
		izanagi::E_SHADER_PARAMETER_TYPE_HALF3, izanagi::E_SHADER_PARAMETER_TYPE_HALF4,

		izanagi::E_SHADER_PARAMETER_TYPE_HALF1x1, izanagi::E_SHADER_PARAMETER_TYPE_HALF1x2,
		izanagi::E_SHADER_PARAMETER_TYPE_HALF1x3, izanagi::E_SHADER_PARAMETER_TYPE_HALF1x4,

		izanagi::E_SHADER_PARAMETER_TYPE_HALF2x1, izanagi::E_SHADER_PARAMETER_TYPE_HALF2x2,
		izanagi::E_SHADER_PARAMETER_TYPE_HALF2x3, izanagi::E_SHADER_PARAMETER_TYPE_HALF2x4,

		izanagi::E_SHADER_PARAMETER_TYPE_HALF3x1, izanagi::E_SHADER_PARAMETER_TYPE_HALF3x2,
		izanagi::E_SHADER_PARAMETER_TYPE_HALF3x3, izanagi::E_SHADER_PARAMETER_TYPE_HALF3x4,

		izanagi::E_SHADER_PARAMETER_TYPE_HALF4x1, izanagi::E_SHADER_PARAMETER_TYPE_HALF4x2,
		izanagi::E_SHADER_PARAMETER_TYPE_HALF4x3, izanagi::E_SHADER_PARAMETER_TYPE_HALF4x4,

		// FLOAT
		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT,  izanagi::E_SHADER_PARAMETER_TYPE_FLOAT2,
		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT3, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT4,

		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT1x1, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT1x2,
		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT1x3, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT1x4,

		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT2x1, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT2x2,
		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT2x3, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT2x4,

		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT3x1, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT3x2,
		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT3x3, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT3x4,

		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT4x1, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT4x2,
		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT4x3, izanagi::E_SHADER_PARAMETER_TYPE_FLOAT4x4,

		// SAMPLER
		izanagi::E_SHADER_PARAMETER_TYPE_SAMPLER1D, izanagi::E_SHADER_PARAMETER_TYPE_SAMPLER2D,
		izanagi::E_SHADER_PARAMETER_TYPE_SAMPLER3D, izanagi::E_SHADER_PARAMETER_TYPE_SAMPLERRECT,
		izanagi::E_SHADER_PARAMETER_TYPE_SAMPLERCUBE,

		// FIXED
		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,
		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,

		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,
		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,

		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,
		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,

		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,
		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,

		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,
		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32, izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,

		// HALF1,FLOAT1,FIXED1
		izanagi::E_SHADER_PARAMETER_TYPE_HALF1,		// HALF1
		izanagi::E_SHADER_PARAMETER_TYPE_FLOAT1,		// FLOAT1
		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,	// FIXED1

		// INT
		izanagi::E_SHADER_PARAMETER_TYPE_INT,
		izanagi::E_SHADER_PARAMETER_TYPE_INT1, izanagi::E_SHADER_PARAMETER_TYPE_INT2,
		izanagi::E_SHADER_PARAMETER_TYPE_INT3, izanagi::E_SHADER_PARAMETER_TYPE_INT4,

		izanagi::E_SHADER_PARAMETER_TYPE_INT1x1, izanagi::E_SHADER_PARAMETER_TYPE_INT1x2,
		izanagi::E_SHADER_PARAMETER_TYPE_INT1x3, izanagi::E_SHADER_PARAMETER_TYPE_INT1x4,

		izanagi::E_SHADER_PARAMETER_TYPE_INT2x1, izanagi::E_SHADER_PARAMETER_TYPE_INT2x2,
		izanagi::E_SHADER_PARAMETER_TYPE_INT2x3, izanagi::E_SHADER_PARAMETER_TYPE_INT2x4,

		izanagi::E_SHADER_PARAMETER_TYPE_INT3x1, izanagi::E_SHADER_PARAMETER_TYPE_INT3x2,
		izanagi::E_SHADER_PARAMETER_TYPE_INT3x3, izanagi::E_SHADER_PARAMETER_TYPE_INT3x4,

		izanagi::E_SHADER_PARAMETER_TYPE_INT4x1, izanagi::E_SHADER_PARAMETER_TYPE_INT4x2,
		izanagi::E_SHADER_PARAMETER_TYPE_INT4x3, izanagi::E_SHADER_PARAMETER_TYPE_INT4x4,

		// BOOL
		izanagi::E_SHADER_PARAMETER_TYPE_BOOL,
		izanagi::E_SHADER_PARAMETER_TYPE_BOOL1, izanagi::E_SHADER_PARAMETER_TYPE_BOOL2,
		izanagi::E_SHADER_PARAMETER_TYPE_BOOL3, izanagi::E_SHADER_PARAMETER_TYPE_BOOL4,

		izanagi::E_SHADER_PARAMETER_TYPE_BOOL1x1, izanagi::E_SHADER_PARAMETER_TYPE_BOOL1x2,
		izanagi::E_SHADER_PARAMETER_TYPE_BOOL1x3, izanagi::E_SHADER_PARAMETER_TYPE_BOOL1x4,

		izanagi::E_SHADER_PARAMETER_TYPE_BOOL2x1, izanagi::E_SHADER_PARAMETER_TYPE_BOOL2x2,
		izanagi::E_SHADER_PARAMETER_TYPE_BOOL2x3, izanagi::E_SHADER_PARAMETER_TYPE_BOOL2x4,

		izanagi::E_SHADER_PARAMETER_TYPE_BOOL3x1, izanagi::E_SHADER_PARAMETER_TYPE_BOOL3x2,
		izanagi::E_SHADER_PARAMETER_TYPE_BOOL3x3, izanagi::E_SHADER_PARAMETER_TYPE_BOOL3x4,

		izanagi::E_SHADER_PARAMETER_TYPE_BOOL4x1, izanagi::E_SHADER_PARAMETER_TYPE_BOOL4x2,
		izanagi::E_SHADER_PARAMETER_TYPE_BOOL4x3, izanagi::E_SHADER_PARAMETER_TYPE_BOOL4x4,

		// その他
		izanagi::E_SHADER_PARAMETER_TYPE_STRING,		// 文字列
		izanagi::E_SHADER_PARAMETER_TYPE_FORCE_INT32,	// プログラム
		izanagi::E_SHADER_PARAMETER_TYPE_TEXTURE,		// テクスチャ
	};
}	// namespace

/**
* CGtype -> E_SHADER_PARAMETER_TYPE
*/
izanagi::E_SHADER_PARAMETER_TYPE CShaderConvUtil::CgParamTypeToIzanagiShaderParamType(CGtype type)
{
	INT idx = type - CG_TYPE_START_ENUM - 1;
	IZ_ASSERT(idx >= 0);

	izanagi::E_SHADER_PARAMETER_TYPE ret = TypeTable[idx];

	return ret;
}
