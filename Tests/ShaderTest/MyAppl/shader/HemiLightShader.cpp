#include "HemiLightShader.h"

// シェーダごとの初期化
BOOL CHemiLightShader::InitInternal(izanagi::graph::CGraphicsDevice* pDevice)
{
	UNUSED_ALWAYS(pDevice);
	IZ_ASSERT(m_pEffect != IZ_NULL);

	if (!GetCommonParameterHandles()) {
		return IZ_FALSE;
	}

	static const char* PARAM_NAME[] = {
		"g_vEye",
		"g_vMtrlDiffuse",
		"g_vAxis",
		"g_vUpColor",
		"g_vDownColor",
	};
	C_ASSERT(COUNTOF(PARAM_NAME) == HANDLE_NUM);

	IZ_BOOL result = GetParameterByName(
						HANDLE_NUM,
						m_hHandles,
						PARAM_NAME);
	IZ_ASSERT(result);

	return result;
}
