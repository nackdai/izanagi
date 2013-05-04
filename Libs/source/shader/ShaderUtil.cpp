#include "shader/base/util/ShaderUtil.h"

namespace izanagi {
    // パラメータをセットする
	IZ_BOOL CShaderUtil::SetValue(
        graph::CGraphicsDevice* device,
		graph::CShaderProgram* pShader,
		SHADER_PARAM_HANDLE handle,
		const void* pValue,
		E_SHADER_PARAMETER_TYPE type,
		IZ_UINT nElements)
	{
		IZ_BOOL ret = IZ_TRUE;

		IZ_UINT nRow = CShaderParamUtil::GetParamRowFromParamType(type);
		IZ_UINT nColumn = CShaderParamUtil::GetParamColumnFromParamType(type);

		if (CShaderParamUtil::IsBoolType(type)) {
			// BOOL
			pShader->SetBoolArray(
                device,
				handle,
				(const IZ_BOOL*)pValue,
				nRow * nColumn * nElements);
		}
		else if (CShaderParamUtil::IsIntType(type)) {
			// INT
			pShader->SetIntArray(
                device,
				handle,
				(const IZ_INT*)pValue,
				nRow * nColumn * nElements);
		}
		else if (CShaderParamUtil::IsMatrixType(type)) {
			// FLOAT4x4 -> MATRIX
			pShader->SetMatrixArray(
                device,
				handle,
				(const math::SMatrix*)pValue,
				nElements);
		}
		else if (CShaderParamUtil::IsFloatType(type)) {
			if (nColumn < 4) {
				// FLOATn x M
				pShader->SetFloatArray(
                    device,
					handle,
					(const IZ_FLOAT*)pValue,
					nRow * nColumn * nElements);
			}
			else {
				// FLOAT4xN -> VECTOR4 x N
				pShader->SetVectorArray(
                    device,
					handle,
					(const math::SVector*)pValue,
					nRow * nElements);
			}
		}
		else {
			ret = IZ_FALSE;
		}

		IZ_ASSERT(ret);
		return ret;
	}

	// サンプラのリソースインデックスをハンドルから引いてくる
	IZ_UINT CShaderUtil::GetSamplerResourceIndexByHandle(
        graph::CGraphicsDevice* device,
		graph::CShaderProgram* pShader,
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
