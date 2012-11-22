#include "RimLightShader.h"

// シェーダごとの初期化
BOOL CRimLightShader::InitInternal(izanagi::graph::CGraphicsDevice* pDevice)
{
	BOOL ret = CPhongShader::InitInternal(pDevice);

	if (ret) {
		static const char* PARAM_NAME[] = {
			"g_vMtrlRim",
		};

		IZ_BOOL ret = GetParameterByName(
						1,
						&m_hRimLightParamHandle,
						PARAM_NAME);
		IZ_ASSERT(ret);
	}

	return ret;
}
