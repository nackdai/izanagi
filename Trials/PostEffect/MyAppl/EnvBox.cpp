#include "izStd.h"
#include "EnvBox.h"

#include "MyCamera.h"

CEnvBox CEnvBox::s_cInstance;

// コンストラクタ
CEnvBox::CEnvBox()
{
	m_pVB = IZ_NULL;
	m_pIB = IZ_NULL;
	m_pVD = IZ_NULL;
	m_pTex = IZ_NULL;
	m_pShader = IZ_NULL;
}

// デストラクタ
CEnvBox::~CEnvBox()
{
}

// 初期化
IZ_BOOL CEnvBox::Init(
	izanagi::IMemoryAllocator* pAllocator,
	izanagi::graph::CGraphicsDevice* pDevice,
	LPCSTR lpszCubeTex,
	LPCSTR lpszShader)
{
	// 頂点
	static const SVertex sVtxList[VTX_NUM] = {
		{{-1.0f,-1.0f,-1.0f}}, {{-1.0f,-1.0f, 1.0f}}, {{ 1.0f,-1.0f,-1.0f}}, {{ 1.0f,-1.0f, 1.0f}},
		{{ 1.0f, 1.0f,-1.0f}}, {{ 1.0f, 1.0f, 1.0f}}, {{-1.0f, 1.0f,-1.0f}}, {{-1.0f, 1.0f, 1.0f}},
	};

	// インデックス
	const IZ_USHORT nIdxList[IDX_NUM] = {
		0, 1, 2, 3, 4, 5, 6, 7, 0, 1,
		1, 7, 3, 5, 5,
		6, 6, 0, 4, 2,
	};

	// 頂点宣言
	static const izanagi::graph::SVertexElement VERTEX_ELEMENT[] = {
		{0, 0, izanagi::graph::E_GRAPH_VTX_DECL_TYPE_FLOAT3, izanagi::graph::E_GRAPH_VTX_DECL_USAGE_POSITION , 0},	// 座標
	};

	IZ_BOOL ret = IZ_TRUE;

	// 頂点バッファ
	if (ret) {
		m_pVB = pDevice->CreateVertexBuffer(
					sizeof(SVertex),
					VTX_NUM,
					izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);
		ret = (m_pVB != IZ_NULL);
		IZ_ASSERT(ret);

		if (m_pVB != IZ_NULL) {
			SVertex* data;
			m_pVB->Lock(0, 0, (void**)&data, IZ_FALSE);
			memcpy(data, sVtxList, sizeof(sVtxList));
			m_pVB->Unlock();
		}
	}

	// インデックスバッファ
	if (ret) {
		m_pIB = pDevice->CreateIndexBuffer(
					IDX_NUM,
					izanagi::graph::E_GRAPH_INDEX_BUFFER_FMT_INDEX16,
					izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);
		ret = (m_pIB != IZ_NULL);
		IZ_ASSERT(ret);

		if (m_pIB != IZ_NULL) {
			IZ_UINT16* data;
			m_pIB->Lock(0, 0, (void**)&data, IZ_FALSE);
			memcpy(data, nIdxList, sizeof(nIdxList));
			m_pIB->Unlock();
		}
	}

	// 頂点宣言
	if (ret) {
		m_pVD = pDevice->CreateVertexDeclaration(
					VERTEX_ELEMENT,
					COUNTOF(VERTEX_ELEMENT));
		ret = (m_pVD != IZ_NULL);
		IZ_ASSERT(ret);
	}

	// シェーダ
	if (ret) {
		ret = InitShader(
				pAllocator,
				pDevice,
				lpszShader);
	}

	// キューブテクスチャ
	if (ret) {
		m_pTex = pDevice->CreateCubeTextureFromFile(lpszCubeTex);
		ret = (m_pTex != IZ_NULL);
		IZ_ASSERT(ret);
	}

	if (!ret) {
		Release();
	}

	return ret;
}

IZ_BOOL CEnvBox::InitShader(
	izanagi::IMemoryAllocator* pAllocator,
	izanagi::graph::CGraphicsDevice* pDevice,
	LPCSTR lpszShader)
{
	IZ_BOOL ret = IZ_FALSE;

	// シェーダ
	if (m_pShader == IZ_NULL) {
		m_pShader = CShaderBase::CreateShader<CEnvBoxShader>(
						pAllocator,
						pDevice,
						lpszShader);
		ret = (m_pShader != IZ_NULL);
		IZ_ASSERT(ret);
	}
	else {
		ret = m_pShader->Reset(pDevice, lpszShader);
		IZ_ASSERT(ret);
	}
	
	return ret;
}

// 開放
void CEnvBox::Release()
{
	SAFE_RELEASE(m_pVB);
	SAFE_RELEASE(m_pIB);
	SAFE_RELEASE(m_pVD);
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pShader);
}

// 描画
void CEnvBox::Render(izanagi::graph::CGraphicsDevice* pDevice)
{
	pDevice->SetRenderState(
		izanagi::graph::E_GRAPH_RS_CULLMODE,
		izanagi::graph::E_GRAPH_CULL_NONE);
	{
		pDevice->SetVertexBuffer(0, 0, sizeof(SVertex), m_pVB);
		pDevice->SetIndexBuffer(m_pIB);
		pDevice->SetVertexDeclaration(m_pVD);

		pDevice->SetTexture(0, m_pTex);
	}

	izanagi::math::SMatrix mL2W;
	izanagi::math::SMatrix::SetUnit(mL2W);
	izanagi::math::SMatrix::Scale(mL2W, mL2W, izanagi::math::CVector(100.0f, 100.0f, 100.0f, 100.0f));
#if 1
	izanagi::math::SVector::CopyXYZ(
		mL2W.v[3],
		CMyCamera::GetInstance().GetRawInterface().GetParam().pos);
#endif
	m_pShader->SetL2W(mL2W);

	m_pShader->SetW2C(CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C);

	m_pShader->Begin(0);
	IZ_UINT nPassCnt = m_pShader->GetPassCount();

	for (IZ_UINT i = 0; i < nPassCnt; i++) {
		if (m_pShader->BeginPass(i)) {
			pDevice->DrawIndexedPrimitive(
				izanagi::graph::E_GRAPH_PRIM_TYPE_TRIANGLESTRIP,
				0, 0,
				VTX_NUM, 0,
				IDX_NUM - 2);

			m_pShader->EndPass();
		}
	}

	m_pShader->End();
}
