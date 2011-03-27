#if !defined(__IZANAGI_POSTEFFECT_PARAM_UTILITY_H__)
#define __IZANAGI_POSTEFFECT_PARAM_UTILITY_H__

#include "izStd.h"
#include "posteffect/PostEffectDefs.h"

namespace izanagi {
	/**
	* ポストエフェクトのパラメータに関するユーティリティ
	*/
	class CPostEffectParamUtil {
	private:
		CPostEffectParamUtil();
		~CPostEffectParamUtil();

	public:
		// パラメータタイプから行数を取得
		static IZ_UINT GetParamRowFromParamType(E_POSTEFFECT_PARAMETER_TYPE type);

		// パラメータタイプから列数を取得
		static IZ_UINT GetParamColumnFromParamType(E_POSTEFFECT_PARAMETER_TYPE type);

		// パラメータタイプがintかどうか
		static IZ_BOOL IsIntType(E_POSTEFFECT_PARAMETER_TYPE type);

		// パラメータタイプがfloatかどうか
		static IZ_BOOL IsFloatType(E_POSTEFFECT_PARAMETER_TYPE type);

	public:
		// パラメータタイプがboolかどうか
		static inline IZ_BOOL IsBoolType(E_POSTEFFECT_PARAMETER_TYPE type);

		// パラメータタイプがmatrix(=float4x4)かどうか
		static inline IZ_BOOL IsMatrixType(E_POSTEFFECT_PARAMETER_TYPE type);

	public:
		// パラメータタイプからバイトサイズを取得する
		static inline IZ_UINT GetBytesFromParamType(E_POSTEFFECT_PARAMETER_TYPE type);

		// パラメータタイプがテクスチャかどうか
		static inline IZ_BOOL IsTextureType(E_POSTEFFECT_PARAMETER_TYPE type);
	};

	// inline ******************************

	/**
	* パラメータタイプがboolかどうか
	*/
	IZ_BOOL CPostEffectParamUtil::IsBoolType(E_POSTEFFECT_PARAMETER_TYPE type)
	{
		IZ_BOOL ret = ((E_POSTEFFECT_PARAMETER_TYPE_BOOL <= type)
						&& (type <= E_POSTEFFECT_PARAMETER_TYPE_BOOL4x4));
		return ret;
	}

	/**
	* パラメータタイプがmatrix(=float4x4)かどうか
	*/
	IZ_BOOL CPostEffectParamUtil::IsMatrixType(E_POSTEFFECT_PARAMETER_TYPE type)
	{
		IZ_BOOL ret = (type == E_POSTEFFECT_PARAMETER_TYPE_FLOAT4x4);
		return ret;
	}

	/**
	* パラメータタイプからバイトサイズを取得する
	*/
	IZ_UINT CPostEffectParamUtil::GetBytesFromParamType(E_POSTEFFECT_PARAMETER_TYPE type)
	{
		IZ_UINT nRow = GetParamRowFromParamType(type);			// 行数
		IZ_UINT nColumn = GetParamColumnFromParamType(type);	// 列数

		IZ_UINT nBase = 0;
		if (IsIntType(type)) {
			nBase = sizeof(IZ_UINT);
		}
		else if (IsFloatType(type)) {
			nBase = sizeof(IZ_FLOAT);
		}

		IZ_UINT ret = nBase * nRow * nColumn;
		return ret;
	}

	/**
	* パラメータタイプがテクスチャかどうか
	*/
	IZ_BOOL CPostEffectParamUtil::IsTextureType(E_POSTEFFECT_PARAMETER_TYPE type)
	{
		return (type == E_POSTEFFECT_PARAMETER_TYPE_TEXTURE);
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_POSTEFFECT_PARAM_UTILITY_H__)
