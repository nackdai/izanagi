#include "posteffect/vs/PostEffectVS.h"
#include "posteffect/vs/PostEffectVSSampling.h"
#include "posteffect/vs/PostEffectVSManager.h"

using namespace izanagi;

///////////////////////////////////////////////////
// 頂点シェーダ基本

// コンストラクタ
CPostEffectVS::CPostEffectVS()
{
	m_Allocator = IZ_NULL;
	m_pDevice = IZ_NULL;

	m_pShader = IZ_NULL;
	m_pVB = IZ_NULL;
	m_pVtxDecl = IZ_NULL;

	FILL_ZERO(m_hCommonHandle, sizeof(m_hCommonHandle));

	SVector::SetZero(m_vecPosOffset);
	SVector::SetZero(m_vecTexParam);
}

// デストラクタ
CPostEffectVS::~CPostEffectVS()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pVtxDecl);
	SAFE_RELEASE(m_pDevice);
}

// 解放
void CPostEffectVS::InternalRelease()
{
	delete this;

	if (m_Allocator != IZ_NULL) {
		m_Allocator->Free(this);
	}
}

// シェーダ作成
IZ_BOOL CPostEffectVS::CreateShader(
	graph::CGraphicsDevice* pDevice,
	IZ_UINT8* pProgram)
{
	IZ_BOOL result = IZ_FALSE;

	// 頂点シェーダ作成
	m_pShader = pDevice->CreateVertexShader(pProgram);
	result = (m_pShader != IZ_NULL);

	if (result) {
		SAFE_REPLACE(m_pDevice, pDevice);

		// 内部初期化
		result = InternalInit();

		if (result) {
			// 共通シェーダ定数ハンドルを取得
			static IZ_PCSTR name[COMMON_HANDLE_NUM] = {
				"g_vPosOffset",
				"g_vTexParam",
			};

			result = GetHandleByName(
						COMMON_HANDLE_NUM,
						name,
						m_hCommonHandle);
		}
	}

	return result;
}

// シェーダ定数ハンドル取得
IZ_BOOL CPostEffectVS::GetHandleByName(
	IZ_UINT num,
	IZ_PCSTR pNameList[],
	SHADER_PARAM_HANDLE* pHandleList)
{
	for (IZ_UINT i = 0; i < num; ++i) {
		pHandleList[i] = m_pShader->GetHandleByName(pNameList[i]);
		if (pHandleList[i] == IZ_NULL) {
			IZ_ASSERT(IZ_FALSE);
			return IZ_FALSE;
		}
	}

	return IZ_TRUE;
}

// 描画
void CPostEffectVS::Render(
	IZ_FLOAT fPosOffsetX,
	IZ_FLOAT fPosOffsetY,
	const SFloatRect* pTexCoord)
{
	IZ_ASSERT(m_pVB != IZ_NULL);
	IZ_ASSERT(m_pVtxDecl != IZ_NULL);
	IZ_ASSERT(m_pShader != IZ_NULL);
	IZ_ASSERT(m_pDevice != IZ_NULL);

	// 頂点シェーダ
	m_pDevice->SetVertexShader(m_pShader);

	// 共通パラメータセット
	SetCommonShaderParameter(
		fPosOffsetX,
		fPosOffsetY,
		pTexCoord);

	// パラメータセット
	SetShaderParameter();

	// 頂点宣言
	m_pDevice->SetVertexDeclaration(m_pVtxDecl);

	// 頂点バッファ
	m_pDevice->SetVertexBuffer(
		0, 
		0,
		sizeof(CPostEffectVSManager::CUSTOM_FVF),
		m_pVB);

	// NOTE
	// 頂点数を指定する
	m_pDevice->DrawPrimitive(
		graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP, 
		0, 
		CPostEffectVSManager::PRIM_NUM);
}

// パラメータセット
void CPostEffectVS::SetParameter(
	const SVector* pVector,
	IZ_UINT num)
{
	// 何もしない
	UNUSED_ALWAYS(pVector);
	UNUSED_ALWAYS(num);
}

// 共通パラメータセット
void CPostEffectVS::SetCommonShaderParameter(
	IZ_FLOAT fPosOffsetX,
	IZ_FLOAT fPosOffsetY,
	const SFloatRect* pTexCoord)
{
	// 位置オフセット
	{
		graph::CSurface* pSurface = m_pDevice->GetRenderTarget(0);
		IZ_ASSERT(pSurface != IZ_NULL);

		IZ_FLOAT fWidth = (IZ_FLOAT)pSurface->GetWidth();
		IZ_FLOAT fHeight = (IZ_FLOAT)pSurface->GetHeight();

		// テクセル中心となるようなオフセット位置
		// ポリゴンの描画位置を-0.5fずらすことで、テクセル位置が0.5fずれるようにするらしい
		IZ_FLOAT fOffsetX = -0.5f / fWidth;
		IZ_FLOAT fOffsetY = -0.5f / fHeight;

		fOffsetX += fPosOffsetX / fWidth;
		fOffsetY += fPosOffsetY / fHeight;

		m_vecPosOffset.Set(
			fOffsetX, fOffsetY,
			0.0f, 0.0f);

		m_pShader->SetVector(
            m_pDevice,
			m_hCommonHandle[COMMON_HANDLE_PosOffset],
			m_vecPosOffset);
	}

	// テクスチャ座標
	{
		IZ_FLOAT fTexScaleX = 1.0f;
		IZ_FLOAT fTexScaleY  = 1.0f;
		IZ_FLOAT fTexOffsetX = 0.0f;
		IZ_FLOAT fTexOffsetY = 0.0f;

		if (pTexCoord != IZ_NULL) {
			// テクスチャ座標(UV)を計算するためのパラメータ
			// 全て 0.0f〜1.0f の間にクランプ済み・・・のはず
			fTexScaleX = pTexCoord->right - pTexCoord->left;
			fTexScaleY = pTexCoord->bottom - pTexCoord->top;
			fTexOffsetX = pTexCoord->left;
			fTexOffsetY = pTexCoord->top;
		}

		m_vecTexParam.Set(
			fTexScaleX, fTexScaleY,
			fTexOffsetX, fTexOffsetY);

		m_pShader->SetVector(
            m_pDevice,
			m_hCommonHandle[COMMON_HANDLE_TexParam],
			m_vecTexParam);
	}
}

// パラメータセット
void CPostEffectVS::SetShaderParameter()
{
	// 何もしない
}

// 初期化
IZ_BOOL CPostEffectVS::InternalInit()
{
	// 何もしない
	return IZ_TRUE;
}

///////////////////////////////////////////////////
// デフォルト頂点シェーダ

///////////////////////////////////////////////////
// 周囲Ｎ点をサンプリングする頂点シェーダ
