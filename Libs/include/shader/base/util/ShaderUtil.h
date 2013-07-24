#if !defined(__IZANAGI_SHADER_SHADER_UTIL_H__)
#define __IZANAGI_SHADER_SHADER_UTIL_H__

#include "izGraph.h"
#include "izMath.h"
#include "ShaderParamUtil.h"

namespace izanagi {
    struct S_SHD_PARAMETER;

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
			const S_SHD_PARAMETER& desc);

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
