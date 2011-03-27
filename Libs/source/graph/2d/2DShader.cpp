#include "graph/2d/2DShader.h"
#include "izGraph.h"

#include "./shader/2DVS.h"
#include "./shader/2DPS_OpVtx.h"
#include "./shader/2DPS_OpTex.h"
#include "./shader/2DPS_OpModulate.h"
#include "./shader/2DPS_OpModulate2x.h"
#include "./shader/2DPS_OpModulate4x.h"
#include "./shader/2DPS_Add.h"
#include "./shader/2DPS_OpModulateAlpha.h"
#include "./shader/2DPS_OpNoTexAlpha.h"

static const IZ_BYTE* PS_Programs[] = {
	(const IZ_BYTE*)g_ps20_main_OpVtx,
	(const IZ_BYTE*)g_ps20_main_OpTex,
	(const IZ_BYTE*)g_ps20_main_OpModulate,
	(const IZ_BYTE*)g_ps20_main_OpModulate2x,
	(const IZ_BYTE*)g_ps20_main_OpModulate4x,
	(const IZ_BYTE*)g_ps20_main_OpAdd,
	(const IZ_BYTE*)g_ps20_main_OpModulateAlpha,
	(const IZ_BYTE*)g_ps20_main_OpNoTexAlpha,
};

C_ASSERT(COUNTOF(PS_Programs) == izanagi::E_GRAPH_2D_RENDER_OP_NUM);

using namespace izanagi;

// インスタンス作成
C2DShader* C2DShader::Create2DShader(
	IMemoryAllocator* pAllocator,
	CGraphicsDevice* pDevice)
{
	IZ_ASSERT(pAllocator != IZ_NULL);
	IZ_ASSERT(pDevice != IZ_NULL);

	C2DShader* pInstance = IZ_NULL;

	// メモリ確保
	IZ_BYTE* pBuf = (IZ_BYTE*)ALLOC_ZERO(pAllocator, sizeof(C2DShader));
	IZ_BOOL result = (pBuf != IZ_NULL);
	if (!result) {
		IZ_ASSERT(IZ_FALSE);
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
			IZ_ASSERT(IZ_FALSE);
			goto __EXIT__;
		}
	}

__EXIT__:
	if (!result) {
		if (pInstance != IZ_NULL) {
			SAFE_RELEASE(pInstance);
		}
		else if (pBuf != IZ_NULL) {
			pAllocator->Free(pBuf);
		}
	}
	return pInstance;
}

// コンストラクタ
C2DShader::C2DShader()
{
	m_pVS = IZ_NULL;
	FILL_ZERO(m_pPS, sizeof(m_pPS));

	m_nOp = E_GRAPH_2D_RENDER_OP_MODULATE;

	FILL_ZERO(m_hVtxParam, sizeof(m_hVtxParam));
}

// デストラクタ
C2DShader::~C2DShader()
{
	SAFE_RELEASE(m_pVS);

	for (IZ_UINT i = 0; i < COUNTOF(m_pPS); ++i) {
		SAFE_RELEASE(m_pPS[i]);
	}
}

// シェーダ作成
IZ_BOOL C2DShader::CreateShader(CGraphicsDevice* pDevice)
{
	IZ_ASSERT(pDevice != NULL);

	// 頂点シェーダ
	m_pVS = pDevice->CreateVertexShader(g_vs20_main);
	VRETURN(m_pVS != IZ_NULL);

	// ピクセルシェーダ
	for (IZ_UINT i = 0; i < E_GRAPH_2D_RENDER_OP_NUM; ++i) {
		m_pPS[i] = pDevice->CreatePixelShader(PS_Programs[i]);
		VRETURN(m_pPS[i] != IZ_NULL);
	}

	// シェーダパラメータ
	static IZ_PCSTR ParamName[] = {
		"g_vInvScreen",
	};
	C_ASSERT(COUNTOF(ParamName) == VTX_PARAM_NUM);

	for (IZ_UINT i = 0; i < VTX_PARAM_NUM; ++i) {
		m_hVtxParam[i] = m_pVS->GetHandleByName(ParamName[i]);
		VRETURN(m_hVtxParam[i] != IZ_NULL);
	}

	return IZ_TRUE;
}

// シェーダセット
IZ_BOOL C2DShader::SetShader(CGraphicsDevice* pDevice)
{
	IZ_ASSERT(pDevice != NULL);

	// 頂点シェーダ
	VRETURN(pDevice->SetVertexShader(m_pVS));

	// ピクセルシェーダ
	VRETURN(pDevice->SetPixelShader(m_pPS[m_nOp]));

	return IZ_TRUE;
}

// シェーダパラメータセット
IZ_BOOL C2DShader::SetShaderParams(CGraphicsDevice* pDevice)
{
	IZ_ASSERT(pDevice != NULL);
	IZ_ASSERT(m_pVS != NULL);

	// スクリーンサイズの逆数
	{
		SVector vTmp;
		vTmp.x = 1.0f / pDevice->GetViewport().width;
		vTmp.y = 1.0f / pDevice->GetViewport().height;

		IZ_ASSERT(m_hVtxParam[VTX_PARAM_INV_SCREEN] != IZ_NULL);
		VRETURN(m_pVS->SetVector(m_hVtxParam[VTX_PARAM_INV_SCREEN], vTmp));
	}

	return IZ_TRUE;
}
