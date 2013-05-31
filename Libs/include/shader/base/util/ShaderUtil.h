#if !defined(__IZANAGI_SHADER_SHADER_UTIL_H__)
#define __IZANAGI_SHADER_SHADER_UTIL_H__

#include "izGraph.h"
#include "izMath.h"
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
		static IZ_BOOL SetValue(
            graph::CGraphicsDevice* device,
            graph::CShaderProgram* pShader,
			SHADER_PARAM_HANDLE handle,
			const void* pValue,
			E_SHADER_PARAMETER_TYPE type,
			IZ_UINT nElements);

		static IZ_BOOL BeginScene(
			graph::CGraphicsDevice* pDevice,
			IZ_INT nColorBufferNum,
            graph::CRenderTarget** pColorBuffer,
			IZ_BOOL bIsResetViewport,
			IZ_INT nTarget,
			IZ_DWORD nClearColor,
			IZ_FLOAT fClearZ,
			IZ_INT nClearStencil);
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_SHADER_SHADER_UTIL_H__)
