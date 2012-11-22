#if !defined(__POSTEFFECT_CONVERTER_POSTEFFECT_CONVERTER_UTIL_H__)
#define __POSTEFFECT_CONVERTER_POSTEFFECT_CONVERTER_UTIL_H__

#include "Cg/cg.h"
#include "Cg/cgD3D9.h"

#include "izPostEffect.h"

class CPostEffectConvUtil {
public:
	/**
	* パラメータタイプがfloatか
	*/
	static BOOL IsFloatType(CGtype type)
	{
		BOOL ret = FALSE;

		// うーん・・・
		switch (type) {
		// float
		case CG_HALF:	case CG_HALF1:
		case CG_HALF2:	case CG_HALF3:
		case CG_HALF4:
		case CG_HALF1x1:	case CG_HALF1x2:
		case CG_HALF1x3:	case CG_HALF1x4:
		case CG_HALF2x1:	case CG_HALF2x2:
		case CG_HALF2x3:	case CG_HALF2x4:
		case CG_HALF3x1:	case CG_HALF3x2:
		case CG_HALF3x3:	case CG_HALF3x4:
		case CG_HALF4x1:	case CG_HALF4x2:
		case CG_HALF4x3:	case CG_HALF4x4:
		case CG_FLOAT:	case CG_FLOAT1:
		case CG_FLOAT2:	case CG_FLOAT3:
		case CG_FLOAT4:
		case CG_FLOAT1x1:	case CG_FLOAT1x2:
		case CG_FLOAT1x3:	case CG_FLOAT1x4:
		case CG_FLOAT2x1:	case CG_FLOAT2x2:
		case CG_FLOAT2x3:	case CG_FLOAT2x4:
		case CG_FLOAT3x1:	case CG_FLOAT3x2:
		case CG_FLOAT3x3:	case CG_FLOAT3x4:
		case CG_FLOAT4x1:	case CG_FLOAT4x2:
		case CG_FLOAT4x3:	case CG_FLOAT4x4:
			ret = TRUE;
			break;
		default:
			break;
		}

		return ret;
	}

	/**
	* パラメータタイプがintか
	*/
	static BOOL IsIntType(CGtype type)
	{
		BOOL ret = FALSE;

		switch (type) {
		// int
		case CG_INT:	case CG_INT1:
		case CG_INT2:	case CG_INT3:
		case CG_INT4:
		case CG_INT1x1:	case CG_INT1x2:
		case CG_INT1x3:	case CG_INT1x4:
		case CG_INT2x1:	case CG_INT2x2:
		case CG_INT2x3:	case CG_INT2x4:
		case CG_INT3x1:	case CG_INT3x2:
		case CG_INT3x3:	case CG_INT3x4:
		case CG_INT4x1:	case CG_INT4x2:
		case CG_INT4x3:	case CG_INT4x4:
		case CG_BOOL:	case CG_BOOL1:
		case CG_BOOL2:	case CG_BOOL3:
		case CG_BOOL4:
		case CG_BOOL1x1:	case CG_BOOL1x2:
		case CG_BOOL1x3:	case CG_BOOL1x4:
		case CG_BOOL2x1:	case CG_BOOL2x2:
		case CG_BOOL2x3:	case CG_BOOL2x4:
		case CG_BOOL3x1:	case CG_BOOL3x2:
		case CG_BOOL3x3:	case CG_BOOL3x4:
		case CG_BOOL4x1:	case CG_BOOL4x2:
		case CG_BOOL4x3:	case CG_BOOL4x4:
			ret = TRUE;
			break;
		default:
			break;
		}

		return ret;
	}

	/**
	* CGtypeからサイズを取得
	*/
	static UINT GetSizeFromCgType(CGtype type)
	{
		UINT ret = 0;

		if (IsIntType(type)) {
			ret = sizeof(int);
		}
		else if (IsFloatType(type)) {
			ret = sizeof(float);
		}
		else {
			IZ_ASSERT(IZ_FALSE);
		}

		return ret;
	}

	///////////////////////////////////////////

	// 頂点プログラムのタイプを決める
	static izanagi::E_POSTEFFECT_VTX_SHADER GetVtxShaderType(LPCSTR lpszStr);

	// CGparameterclass -> E_POSTEFFECT_PARAMETER_CLASS
	static izanagi::E_POSTEFFECT_PARAMETER_CLASS CgParamClassToIzanagiPostEffectParamClass(CGparameterclass nClass);

	// CGtype -> E_POSTEFFECT_PARAMETER_TYPE
	static izanagi::E_POSTEFFECT_PARAMETER_TYPE CgParamTypeToIzanagiPostEffectParamType(CGtype type);
};

#endif	// #if !defined(__POSTEFFECT_CONVERTER_POSTEFFECT_CONVERTER_UTIL_H__)
