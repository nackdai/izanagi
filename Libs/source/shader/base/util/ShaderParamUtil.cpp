#include "shader/base/util/ShaderParamUtil.h"

using namespace izanagi;

namespace {
	struct SParamTypeInfo{
		IZ_UINT8 row;
		IZ_UINT8 column;
		IZ_BOOL is_int;
		IZ_BOOL is_float;
	};

#undef __SHD_PARAM_TYPE__
#define __SHD_PARAM_TYPE__(type, row, column, is_int, is_float) { row, column, is_int, is_float }
	SParamTypeInfo ParamTypeInfoTbl[] = {
	#include "shader/base/util/ShaderParameterTypes.h"
	};
#undef __SHD_PARAM_TYPE__
}	// namespace

/**
* パラメータタイプから行数を取得
*
* 例えば
*    E_SHADER_PARAMETER_TYPE_FLOAT3x4 -> 3
*    E_SHADER_PARAMETER_TYPE_FLOAT    -> 1
*    E_SHADER_PARAMETER_TYPE_FLOAT4   -> 1
* が返ってくる
*/
IZ_UINT CShaderParamUtil::GetParamRowFromParamType(E_SHADER_PARAMETER_TYPE type)
{
	IZ_ASSERT(type < E_SHADER_PARAMETER_TYPE_NUM);
	return ParamTypeInfoTbl[type].row;
}

/**
* パラメータタイプから列数を取得
*
* 例えば
*    E_SHADER_PARAMETER_TYPE_FLOAT3x4 -> 4
*    E_SHADER_PARAMETER_TYPE_FLOAT    -> 1
*    E_SHADER_PARAMETER_TYPE_FLOAT4   -> 4
* が返ってくる
*/
IZ_UINT CShaderParamUtil::GetParamColumnFromParamType(E_SHADER_PARAMETER_TYPE type)
{
	IZ_ASSERT(type < E_SHADER_PARAMETER_TYPE_NUM);
	return ParamTypeInfoTbl[type].column;
}

/**
* パラメータタイプがintかどうか
*/
IZ_BOOL CShaderParamUtil::IsIntType(E_SHADER_PARAMETER_TYPE type)
{
	IZ_ASSERT(type < E_SHADER_PARAMETER_TYPE_NUM);
	return ParamTypeInfoTbl[type].is_int;
}

/**
* パラメータタイプがfalseかどうか
*/
IZ_BOOL CShaderParamUtil::IsFloatType(E_SHADER_PARAMETER_TYPE type)
{
	IZ_ASSERT(type < E_SHADER_PARAMETER_TYPE_NUM);
	return ParamTypeInfoTbl[type].is_float;
}
