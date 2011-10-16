#include "StateBasic.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "DebugInfoRenderer.h"
#include "TextureManager.h"

CStateBasic::CStateBasic()
{
	m_pShader = IZ_NULL;

	m_pGrid = IZ_NULL;
	m_pSphere = IZ_NULL;
}

CStateBasic::~CStateBasic()
{
}

IZ_BOOL CStateBasic::Create()
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Render2D()
{
	CDebugInfoRenderer::Render("Basic");
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Render3D()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	// 描画開始
	m_pShader->Begin(0);

	// 念のため確認
	IZ_UINT nPassCnt = m_pShader->GetPassCount();
	IZ_ASSERT(nPassCnt == 2);

	// シェーダパラメータセット
	{
		izanagi::SMatrix mL2W;
		izanagi::SMatrix::SetUnit(mL2W);
		m_pShader->SetL2W(mL2W);

		m_pShader->SetW2C(CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C);
	}

	// グリッド描画
	m_pShader->BeginPass(1);
	m_pGrid->Draw();
	m_pShader->EndPass();

	// テクスチャセット
	izanagi::CTexture* pTex = izanagi::CTextureManager::GetInstance().Get(DEMO_TEX_EARTH);
	pDevice->SetTexture(0, pTex);

	// メッシュ描画
	m_pShader->BeginPass(0);
	m_pSphere->Draw();
	m_pShader->EndPass();

	// 描画終了
	m_pShader->End();

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Destroy()
{
	Leave();
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Enter()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();

	// シェーダ
	m_pShader = CShaderBase::CreateShader<CBasicShader>(
					pAllocator,
					pDevice,
					"./data/BasicShader.bin");
	IZ_ASSERT(m_pShader != IZ_NULL);

	// グリッド
	m_pGrid = izanagi::CDebugMeshGrid::CreateDebugMeshGrid(
				pAllocator,
				pDevice,
				0xffffffff,
				20, 20,
				10.0f);
	IZ_ASSERT(m_pGrid != IZ_NULL);

	// 球
	m_pSphere = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
					pAllocator,
					pDevice,
					izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV,
					0xffffffff,
					5.0f,
					50, 50);
	IZ_ASSERT(m_pSphere != IZ_NULL);

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pGrid);
	SAFE_RELEASE(m_pSphere);

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}