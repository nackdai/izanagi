#include "StateParallax.h"
#include "MySystem.h"
#include "MyCamera.h"

CStateParallax::CStateParallax()
{
	m_pShader = IZ_NULL;

	m_pTex = IZ_NULL;
	m_pParallax = IZ_NULL;

	m_pGrid = IZ_NULL;
	m_pAxis = IZ_NULL;
	m_pMesh = IZ_NULL;
}

CStateParallax::~CStateParallax()
{
}

IZ_BOOL CStateParallax::Init()
{
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Render(izanagi::CGraphicsDevice* device)
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Render2D()
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

IZ_BOOL CStateParallax::Render3D()
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
		m_pShader->SetCameraPos(CMyCamera::GetInstance().GetRawInterface().GetParam().pos);
	}

	// グリッド描画
	m_pShader->BeginPass(1);
	m_pGrid->Draw();
	m_pAxis->Draw();
	m_pShader->EndPass();

	// テクスチャセット
	pDevice->SetTexture(0, m_pTex);
	pDevice->SetTexture(1, m_pParallax);

	// メッシュ描画
	m_pShader->BeginPass(0);
	m_pMesh->Draw();
	m_pShader->EndPass();

	// 描画終了
	m_pShader->End();

	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Destroy()
{
	Leave();
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Enter(izanagi::IMemoryAllocator* allocator, void* val)
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();

	// シェーダ
	m_pShader = CShaderBase::CreateShader<CParallaxShader>(
					pAllocator,
					pDevice,
					"./data/ParallaxShader.bin");
					//"./data/PhongeShader.bin");
	IZ_ASSERT(m_pShader != IZ_NULL);

	// テストなので、ここで各種パラメータ設定
	if (m_pShader != IZ_NULL) {
		// ライト
		izanagi::SParallelLightParam sParallel;
		{
			sParallel.vDir.Set(-1.0f, -1.0f, -1.0f, 0.0f);
			izanagi::SVector::Normalize(sParallel.vDir, sParallel.vDir);

			sParallel.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::SVector::SetZero(sAmbient.color);

		// マテリアル
		izanagi::SMaterialParam sMtrl;
		{
			sMtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vAmbient.Set(1.0f, 1.0f, 1.0f, 1.0f);
			sMtrl.vSpecular.Set(1.0f, 1.0f, 1.0f, 20.0f);
		}

		m_pShader->SetLight(sParallel, sAmbient);
		m_pShader->SetMaterial(sMtrl);
	}

	// テクスチャ
	m_pTex = pDevice->CreateTextureFromFile(
				//"./data/earth.bmp",
				"./data/tile.bmp",
				izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
	IZ_ASSERT(m_pTex != IZ_NULL);

	// 法線マップ
	m_pParallax = pDevice->CreateTextureFromFile(
					//"./data/normal.bmp",
					"./data/four_NM_height.tga",
					izanagi::E_GRAPH_PIXEL_FMT_RGBA8);
	IZ_ASSERT(m_pParallax != IZ_NULL);

	// グリッド
	m_pGrid = izanagi::CDebugMeshGrid::CreateDebugMeshGrid(
				pAllocator,
				pDevice,
				0xffffffff,
				20, 20,
				10.0f);
	IZ_ASSERT(m_pGrid != IZ_NULL);

	// XYZ軸
	m_pAxis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
				pAllocator,
				pDevice);
	IZ_ASSERT(m_pAxis != IZ_NULL);

#if 0
	// 球
	m_pMesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
				pAllocator,
				pDevice,
				//izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL,
				izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT,
				0xffffffff,
				5.0f,
				50, 50);
#else
	// 矩形
	m_pMesh = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
				pAllocator,
				pDevice,
				izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT,
				0xffffffff,
				10, 10,
				20.0f, 20.0f);
#endif
	IZ_ASSERT(m_pMesh != IZ_NULL);

	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pParallax);
	SAFE_RELEASE(m_pGrid);
	SAFE_RELEASE(m_pAxis);
	SAFE_RELEASE(m_pMesh);

	return IZ_TRUE;
}

IZ_BOOL CStateParallax::OnKeyDown(IZ_UINT nChar)
{
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::OnMouseMove(IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::OnMouseWheel(IZ_SHORT zDelta)
{
	return IZ_TRUE;
}