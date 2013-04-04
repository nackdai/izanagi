#if !defined(__IZANAGI_SHADER_SHADER_UTIL_H__)
#define __IZANAGI_SHADER_SHADER_UTIL_H__

#include "izGraph.h"
#include "ShaderParamUtil.h"

namespace izanagi {
	class CShaderUtil {
	private:
		CShaderUtil();
		~CShaderUtil();

		CShaderUtil(const CShaderUtil& rhs);
		const CShaderUtil& operator=(const CShaderUtil& rhs);

	public:
		// パラメータをセットする
		template <class _T>
		static IZ_BOOL SetValue(
			_T* pShader,
			SHADER_PARAM_HANDLE handle,
			const void* pValue,
			E_SHADER_PARAMETER_TYPE type,
			IZ_UINT nElements);

	public:
		// サンプラのリソースインデックスをハンドルから引いてくる
		static inline IZ_UINT GetSamplerResourceIndexByHandle(
			graph::CPixelShader* pShader,
			SHADER_PARAM_HANDLE handle);

		static inline IZ_BOOL BeginScene(
			graph::CGraphicsDevice* pDevice,
			IZ_INT nColorBufferNum,
            graph::CRenderTarget** pColorBuffer,
			IZ_BOOL bIsResetViewport,
			IZ_INT nTarget,
			IZ_DWORD nClearColor,
			IZ_FLOAT fClearZ,
			IZ_INT nClearStencil);
	};

	// パラメータをセットする
	template <class _T>
	IZ_BOOL CShaderUtil::SetValue(
		_T* pShader,
		SHADER_PARAM_HANDLE handle,
		const void* pValue,
		E_SHADER_PARAMETER_TYPE type,
		IZ_UINT nElements)
	{
		IZ_BOOL ret = IZ_TRUE;

		IZ_UINT nRow = CPostEffectParamUtil::GetParamRowFromParamType(type);
		IZ_UINT nColumn = CPostEffectParamUtil::GetParamColumnFromParamType(type);

		if (CPostEffectParamUtil::IsBoolType(type)) {
			// BOOL
			pShader->SetBoolArray(
				handle,
				(const IZ_BOOL*)pValue,
				nRow * nColumn * nElements);
		}
		else if (CPostEffectParamUtil::IsIntType(type)) {
			// INT
			pShader->SetIntArray(
				handle,
				(const IZ_INT*)pValue,
				nRow * nColumn * nElements);
		}
		else if (CPostEffectParamUtil::IsMatrixType(type)) {
			// FLOAT4x4 -> MATRIX
			pShader->SetMatrixArray(
				handle,
				(const SMatrix*)pValue,
				nElements);
		}
		else if (CPostEffectParamUtil::IsFloatType(type)) {
			if (nColumn < 4) {
				// FLOATn x M
				pShader->SetFloatArray(
					handle,
					(const IZ_FLOAT*)pValue,
					nRow * nColumn * nElements);
			}
			else {
				// FLOAT4xN -> VECTOR4 x N
				pShader->SetVectorArray(
					handle,
					(const SVector*)pValue,
					nRow * nElements);
			}
		}
		else {
			ret = IZ_FALSE;
		}

		IZ_ASSERT(ret);
		return ret;
	}

	// inline *******************************

	// サンプラのリソースインデックスをハンドルから引いてくる
	IZ_UINT CShaderUtil::GetSamplerResourceIndexByHandle(
		graph::CPixelShader* pShader,
		SHADER_PARAM_HANDLE handle)
	{
		IZ_UINT ret = pShader->GetSamplerIndex(handle);
		return ret;
	}

	IZ_BOOL CShaderUtil::BeginScene(
		graph::CGraphicsDevice* pDevice,
		IZ_INT nColorBufferNum,
		graph::CRenderTarget** pColorBuffer,
		IZ_BOOL bIsResetViewport,
		IZ_INT nTarget,
		IZ_DWORD nClearColor,
		IZ_FLOAT fClearZ,
		IZ_INT nClearStencil)
	{
		IZ_BOOL ret = pDevice->BeginScene(
						pColorBuffer,
						nColorBufferNum,
						//bIsResetViewport,	// TODO
						nTarget,
						nClearColor,
						fClearZ,
						nClearStencil);
		return ret;
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_UTIL_H__)
