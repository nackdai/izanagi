#if !defined(__SHADER_CONVERTER_PARAM_UTIL_H__)
#define __SHADER_CONVERTER_PARAM_UTIL_H__

#include <Cg/cg.h>
#include <Cg/cgD3D9.h>
#include "izShader.h"
#include "StringChunk.h"

class CParamUtil {
public:
	static CGtype GetCgType(CGparameter param)
	{
		CGtype type = ::cgGetParameterType(param);
		if (type == CG_ARRAY) {
			type = ::cgGetArrayType(param);
		}

		return type;
	}
	
	static BOOL IsTexture(CGparameter param)
	{
		CGtype type = GetCgType(param);
		return (type == CG_TEXTURE);
	}

	static BOOL IsSampler(CGparameter param)
	{
		CGtype type = GetCgType(param);
		return ((type == CG_SAMPLER1D)
				|| (type == CG_SAMPLER2D)
				|| (type == CG_SAMPLER3D));
	}

	static BOOL IsParameter(CGparameter param)
	{
		// Texture でも Sampler でもない
		BOOL ret = (IsTexture(param) || IsSampler(param));
		return !ret;
	}

	// タイプがfixedかどうか
	static BOOL IsTypeFixed(CGparameter param)
	{
		CGtype type = GetCgType(param);
		BOOL b1 = ((type >= CG_FIXED) && (type <= CG_FIXED4x4));
		BOOL b2 = (type == CG_FIXED1);
		return (b1 || b2);
	}

	static BOOL IsValidParameter(CGparameter param)
	{
		CGparameterclass paramclass = cgGetParameterClass(param);

		// 構造体
		BOOL b1 = (paramclass == CG_PARAMETERCLASS_STRUCT);

		// オブジェクトだがテクスチャorサンプラでない
		BOOL b2 = ((paramclass == CG_PARAMETERCLASS_OBJECT)
					&& (!IsTexture(param) && !IsSampler(param)));

		// fixed
		BOOL b3 = IsTypeFixed(param);

		return (!b1 && !b2 && !b3);
	}

	template <typename _T>
	static void SetNameAndSemantic(
		_T& sDesc,
		CGparameter param)
	{
		// 名前
		IZ_PCSTR name = ::cgGetParameterName(param);
		IZ_UINT pos = CStringChunk::GetInstance().Register(name);

		//sDesc.name = *(IZ_PCSTR*)(&pos);
		sDesc.posName = pos;
		sDesc.keyName = izanagi::CKey::GenerateValue(name);

		// セマンティック
		IZ_PCSTR semantic = ::cgGetParameterSemantic(param);
		pos = CStringChunk::GetInstance().Register(semantic);

		//sDesc.semantic = *(IZ_PCSTR*)(&pos);
		sDesc.posSemantic = pos;
		sDesc.keySemantic = izanagi::CKey::GenerateValue(semantic);
	}

public:
	static BOOL SetAnnValue(
		izanagi::S_SHD_PARAM_ANN& sAnn,
		CGparameter param);

	static BOOL HasAnn(CGparameter param)
	{
		return (::cgGetFirstParameterAnnotation(param) != NULL);
	}

	static BOOL DoNotStrip(CGparameter param);

	static BOOL SetDescValue(
		izanagi::S_SHD_PARAMETER& sDesc,
		CGparameter param);

	static BOOL GetInitValue(
		izanagi::S_SHD_PARAMETER& sDesc,
		CGparameter param);
};

#endif	// #if !defined(__SHADER_CONVERTER_PARAM_UTIL_H__)
