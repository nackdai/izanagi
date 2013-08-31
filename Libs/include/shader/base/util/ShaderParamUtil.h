#if !defined(__IZANAGI_SHADER_SHADER_PARAM_UTIL_H__)
#define __IZANAGI_SHADER_SHADER_PARAM_UTIL_H__

#include "izStd.h"
#include "shader/base/ShaderDefs.h"

namespace izanagi
{
namespace shader
{
	/**
	* ポストエフェクトのパラメータに関するユーティリティ
	*/
	class CShaderParamUtil {
	private:
		CShaderParamUtil();
		~CShaderParamUtil();

	public:
		/**
		 * パラメータタイプから行数を取得
		 *
		 * 例えば
		 *    E_SHADER_PARAMETER_TYPE_FLOAT3x4 -> 3
		 *    E_SHADER_PARAMETER_TYPE_FLOAT    -> 1
		 *    E_SHADER_PARAMETER_TYPE_FLOAT4   -> 1
		 * が返ってくる
		 */
		static inline IZ_UINT GetParamRowFromParamType(E_SHADER_PARAMETER_TYPE type);

		/**
		 * パラメータタイプから列数を取得
		 *
		 * 例えば
		 *    E_SHADER_PARAMETER_TYPE_FLOAT3x4 -> 4
		 *    E_SHADER_PARAMETER_TYPE_FLOAT    -> 1
		 *    E_SHADER_PARAMETER_TYPE_FLOAT4   -> 4
		 * が返ってくる
		 */
		static inline IZ_UINT GetParamColumnFromParamType(E_SHADER_PARAMETER_TYPE type);

		/**
		 * パラメータタイプがintかどうか
		 */
		static inline IZ_BOOL IsIntType(E_SHADER_PARAMETER_TYPE type);

		/**
		 * パラメータタイプがfloatかどうか
		 */
		static inline IZ_BOOL IsFloatType(E_SHADER_PARAMETER_TYPE type);

	public:
		/**
		 * パラメータタイプがboolかどうか
		 */
		static inline IZ_BOOL IsBoolType(E_SHADER_PARAMETER_TYPE type);

		/**
		 * パラメータタイプがmatrix(=float4x4)かどうか
		 */
		static inline IZ_BOOL IsMatrixType(E_SHADER_PARAMETER_TYPE type);

	public:
		/**
		 * パラメータタイプからバイトサイズを取得する
		 */
		static inline IZ_UINT GetBytesFromParamType(E_SHADER_PARAMETER_TYPE type);

		/**
		 * パラメータタイプがテクスチャかどうか
		 */
		static inline IZ_BOOL IsTextureType(E_SHADER_PARAMETER_TYPE type);
	};

	// inline ******************************

	// パラメータタイプから行数を取得
	IZ_UINT CShaderParamUtil::GetParamRowFromParamType(E_SHADER_PARAMETER_TYPE type)
	{
		IZ_UINT ret = IZ_GET_ROW_SHADER_PARAM_TYPE(type);
		return ret;
	}

	// パラメータタイプから列数を取得
	IZ_UINT CShaderParamUtil::GetParamColumnFromParamType(E_SHADER_PARAMETER_TYPE type)
	{
		IZ_UINT ret = IZ_GET_COLUMN_SHADER_PARAM_TYPE(type);
		return ret;
	}

	// パラメータタイプがintかどうか
	IZ_BOOL CShaderParamUtil::IsIntType(E_SHADER_PARAMETER_TYPE type)
	{
		IZ_UINT ret = IZ_IS_INT_SHADER_PARAM_TYPE(type);
		return (ret == 1);
	}

	// パラメータタイプがfloatかどうか
	IZ_BOOL CShaderParamUtil::IsFloatType(E_SHADER_PARAMETER_TYPE type)
	{
		IZ_UINT ret = IZ_IS_FLOAT_SHADER_PARAM_TYPE(type);
		return (ret == 1);
	}

	// パラメータタイプがboolかどうか
	IZ_BOOL CShaderParamUtil::IsBoolType(E_SHADER_PARAMETER_TYPE type)
	{
		IZ_BOOL ret = ((E_SHADER_PARAMETER_TYPE_BOOL <= type)
						&& (type <= E_SHADER_PARAMETER_TYPE_BOOL4x4));
		return ret;
	}

	// パラメータタイプがmatrix(=float4x4)かどうか
	IZ_BOOL CShaderParamUtil::IsMatrixType(E_SHADER_PARAMETER_TYPE type)
	{
		IZ_BOOL ret = (type == E_SHADER_PARAMETER_TYPE_FLOAT4x4);
		return ret;
	}

	// パラメータタイプからバイトサイズを取得する
	IZ_UINT CShaderParamUtil::GetBytesFromParamType(E_SHADER_PARAMETER_TYPE type)
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

	// パラメータタイプがテクスチャかどうか
	IZ_BOOL CShaderParamUtil::IsTextureType(E_SHADER_PARAMETER_TYPE type)
	{
		return (type == E_SHADER_PARAMETER_TYPE_TEXTURE);
	}
}   // namespace shader
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_PARAM_UTIL_H__)
