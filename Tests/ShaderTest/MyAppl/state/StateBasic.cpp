#include "StateBasic.h"
#include "MySystem.h"
#include "MyCamera.h"

CStateBasic::CStateBasic()
{
	m_pShader = IZ_NULL;

	m_pTex = IZ_NULL;

	m_pGrid = IZ_NULL;
	m_pSphere = IZ_NULL;
}

CStateBasic::~CStateBasic()
{
}

IZ_BOOL CStateBasic::Init()
{
	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Render(izanagi::CGraphicsDevice* device)
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

IZ_BOOL CStateBasic::Render2D()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::CDebugFont* pFont = CMySystem::GetInstance().GetDebugFont();

	if (pDevice->Begin2D()) {
		pFont->Begin();

#if 1
		{
			IZ_FLOAT time = CMySystem::GetInstance().GetTimer(0).GetTime();
			IZ_FLOAT fps = 1000.0f / time;

			pFont->DBPrint(
				"%.2f[ms] %.2f[fps]\n",
				time, fps);
		}
#endif

#if 1
		{
			IZ_FLOAT time = CMySystem::GetInstance().GetTimer(1).GetTime();
			IZ_FLOAT fps = 1000.0f / time;

			pFont->DBPrint(
				"%.2f[ms] %.2f[fps]\n",
				time, fps);
		}
#endif

		pFont->DBPrint(
			0xffffffff,
			"test\n");

		pFont->End();

		pDevice->End2D();
	}

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
	pDevice->SetTexture(0, m_pTex);

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

IZ_BOOL CStateBasic::Enter(izanagi::IMemoryAllocator* allocator, void* val)
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();

	// シェーダ
	m_pShader = CShaderBase::CreateShader<CBasicShader>(
					pAllocator,
					pDevice,
					"./data/BasicShader.bin");
	IZ_ASSERT(m_pShader != IZ_NULL);

	// テクスチャ
	m_pTex = pDevice->CreateTextureFromFile(
				"./data/earth.bmp",
				//"./data/color.bmp",
				izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
	IZ_ASSERT(m_pTex != IZ_NULL);

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
	SAFE_RELEASE(m_pTex);
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