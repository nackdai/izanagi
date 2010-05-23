#include "graph/2d/2DShader.h"
#include "unGraph.h"

#include "./shader/2DVS.h"
#include "./shader/2DPS_OpVtx.h"
#include "./shader/2DPS_OpTex.h"
#include "./shader/2DPS_OpModulate.h"
#include "./shader/2DPS_OpModulate2x.h"
#include "./shader/2DPS_OpModulate4x.h"
#include "./shader/2DPS_Add.h"
#include "./shader/2DPS_OpModulateAlpha.h"

static const UN_BYTE* PS_Programs[] = {
	(const UN_BYTE*)g_ps20_main_OpVtx,
	(const UN_BYTE*)g_ps20_main_OpTex,
	(const UN_BYTE*)g_ps20_main_OpModulate,
	(const UN_BYTE*)g_ps20_main_OpModulate2x,
	(const UN_BYTE*)g_ps20_main_OpModulate4x,
	(const UN_BYTE*)g_ps20_main_OpAdd,
	(const UN_BYTE*)g_ps20_main_OpModulateAlpha,
};

C_ASSERT(COUNTOF(PS_Programs) == uranus::E_GRAPH_2D_RENDER_OP_NUM);

using namespace uranus;

// インスタンス作成
C2DShader* C2DShader::Create2DShader(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice)
{
	UN_ASSERT(pAllocator != UN_NULL);
	UN_ASSERT(pDevice != UN_NULL);

	C2DShader* pInstance = UN_NULL;

	// メモリ確保
	UN_BYTE* pBuf = (UN_BYTE*)ALLOC_ZERO(pAllocator, sizeof(C2DShader));
	UN_BOOL result = (pBuf != UN_NULL);
	if (!result) {
		UN_ASSERT(UN_FALSE);
		goto __EXIT__;
	}

	// インスタンス作成
	pInstance = new (pBuf) C2DShader();
	{
		pInstance->AddRef();
		pInstance->m_pAllocator = pAllocator;
		
		// シェーダ作成
		result = pInstance->CreateShader(pDevice);
		if (!result) {
			UN_ASSERT(UN_FALSE);
			goto __EXIT__;
		}
	}

__EXIT__:
	if (!result) {
		if (pInstance != UN_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != UN_NULL) {
			pAllocator->Free(pBuf);
		}
	}
	return pInstance;
}

// コンストラクタ
C2DShader::C2DShader()
{
	m_pVS = UN_NULL;
	FILL_ZERO(m_pPS, sizeof(m_pPS));

	m_nOp = E_GRAPH_2D_RENDER_OP_MODULATE;

	FILL_ZERO(m_hVtxParam, sizeof(m_hVtxParam));
}

// デストラクタ
C2DShader::~C2DShader()
{
	SAFE_RELEASE(m_pVS);

	for (UN_UINT i = 0; i < COUNTOF(m_pPS); i++) {
		SAFE_RELEASE(m_pPS[i]);
	}
}

// シェーダ作成
UN_BOOL C2DShader::CreateShader(CGraphicsDevice* pDevice)
{
	UN_ASSERT(pDevice != NULL);

	// 頂点シェーダ
	m_pVS = pDevice->CreateVertexShader(g_vs20_main);
	VRETURN(m_pVS != UN_NULL);

	// ピクセルシェーダ
	for (UN_UINT i = 0; i < E_GRAPH_2D_RENDER_OP_NUM; i++) {
		m_pPS[i] = pDevice->CreatePixelShader(PS_Programs[i]);
		VRETURN(m_pPS[i] != UN_NULL);
	}

	// シェーダパラメータ
	static UN_PCSTR ParamName[] = {
		"g_vInvScreen",
	};
	C_ASSERT(COUNTOF(ParamName) == VTX_PARAM_NUM);

	for (UN_UINT i = 0; i < VTX_PARAM_NUM; i++) {
		m_hVtxParam[i] = m_pVS->GetHandleByName(ParamName[i]);
		VRETURN(m_hVtxParam[i] != UN_NULL);
	}

	return UN_TRUE;
}

// シェーダセット
UN_BOOL C2DShader::SetShader(CGraphicsDevice* pDevice)
{
	UN_ASSERT(pDevice != NULL);

	// 頂点シェーダ
	VRETURN(pDevice->SetVertexShader(m_pVS));

	// ピクセルシェーダ
	VRETURN(pDevice->SetPixelShader(m_pPS[m_nOp]));

	return UN_TRUE;
}

// シェーダパラメータセット
UN_BOOL C2DShader::SetShaderParams(CGraphicsDevice* pDevice)
{
	UN_ASSERT(pDevice != NULL);
	UN_ASSERT(m_pVS != NULL);

	// スクリーンサイズの逆数
	{
		SVector vTmp;
		vTmp.x = 1.0f / pDevice->GetViewport().width;
		vTmp.y = 1.0f / pDevice->GetViewport().height;

		UN_ASSERT(m_hVtxParam[VTX_PARAM_INV_SCREEN] != UN_NULL);
		VRETURN(m_pVS->SetVector(m_hVtxParam[VTX_PARAM_INV_SCREEN], vTmp));
	}

	return UN_TRUE;
}
