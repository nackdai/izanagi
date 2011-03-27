#include "posteffect/util/PostEffectUtilityParam.h"

using namespace izanagi;

namespace {
	struct SParamTypeInfo{
		IZ_UINT8 row;
		IZ_UINT8 column;
		IZ_BOOL is_int;
		IZ_BOOL is_float;
	};

#undef __PE_PARAM_TYPE__
#define __PE_PARAM_TYPE__(type, row, column, is_int, is_float) { row, column, is_int, is_float }
	SParamTypeInfo ParamTypeInfoTbl[] = {
	#include "posteffect/util/PostEffectParameterTypes.h"
	};
#undef __PE_PARAM_TYPE__
}	// namespace

/**
* パラメータタイプから行数を取得
*
* 例えば
*    E_POSTEFFECT_PARAMETER_TYPE_FLOAT3x4 -> 3
*    E_POSTEFFECT_PARAMETER_TYPE_FLOAT    -> 1
*    E_POSTEFFECT_PARAMETER_TYPE_FLOAT4   -> 1
* が返ってくる
*/
IZ_UINT CPostEffectParamUtil::GetParamRowFromParamType(E_POSTEFFECT_PARAMETER_TYPE type)
{
	IZ_ASSERT(type < E_POSTEFFECT_PARAMETER_TYPE_NUM);
	return ParamTypeInfoTbl[type].row;
}

/**
* パラメータタイプから列数を取得
*
* 例えば
*    E_POSTEFFECT_PARAMETER_TYPE_FLOAT3x4 -> 4
*    E_POSTEFFECT_PARAMETER_TYPE_FLOAT    -> 1
*    E_POSTEFFECT_PARAMETER_TYPE_FLOAT4   -> 4
* が返ってくる
*/
IZ_UINT CPostEffectParamUtil::GetParamColumnFromParamType(E_POSTEFFECT_PARAMETER_TYPE type)
{
	IZ_ASSERT(type < E_POSTEFFECT_PARAMETER_TYPE_NUM);
	return ParamTypeInfoTbl[type].column;
}

/**
* パラメータタイプがintかどうか
*/
IZ_BOOL CPostEffectParamUtil::IsIntType(E_POSTEFFECT_PARAMETER_TYPE type)
{
	IZ_ASSERT(type < E_POSTEFFECT_PARAMETER_TYPE_NUM);
	return ParamTypeInfoTbl[type].is_int;
}

/**
* パラメータタイプがfalseかどうか
*/
IZ_BOOL CPostEffectParamUtil::IsFloatType(E_POSTEFFECT_PARAMETER_TYPE type)
{
	IZ_ASSERT(type < E_POSTEFFECT_PARAMETER_TYPE_NUM);
	return ParamTypeInfoTbl[type].is_float;
}
