#include "StateHemiLight.h"
#include "MySystem.h"
#include "MyCamera.h"

CStateHemiLight::CStateHemiLight()
{
	m_pShader = IZ_NULL;

	m_pTex = IZ_NULL;

	m_pGrid = IZ_NULL;
	m_pSphere = IZ_NULL;
}

CStateHemiLight::~CStateHemiLight()
{
}

IZ_BOOL CStateHemiLight::Create()
{
	return IZ_TRUE;
}

IZ_BOOL CStateHemiLight::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

IZ_BOOL CStateHemiLight::Render2D()
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

IZ_BOOL CStateHemiLight::Render3D()
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

IZ_BOOL CStateHemiLight::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateHemiLight::Destroy()
{
	Leave();
	return IZ_TRUE;
}

IZ_BOOL CStateHemiLight::Enter()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();

	// シェーダ
	m_pShader = CShaderBase::CreateShader<CHemiLightShader>(
					pAllocator,
					pDevice,
					"./data/HemiLightShader.bin");
	IZ_ASSERT(m_pShader != IZ_NULL);

	// テストなので、ここで各種パラメータ設定
	if (m_pShader != IZ_NULL) {
		// ライト
		izanagi::SHemisphereLightParam sParam;
		{
			sParam.vAxis.Set(1.0f, 1.0f, 1.0f, 0.0f);
			izanagi::SVector::Normalize(sParam.vAxis, sParam.vAxis);

			sParam.upColor.Set(1.0f, 0.0f, 0.0f);
			sParam.downColor.Set(0.0f, 0.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::SVector::SetZero(sAmbient.color);

		// マテリアル
		izanagi::SMaterialParam sMtrl;
		{
			sMtrl.vDiffuse.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		m_pShader->SetLight(sParam);
		m_pShader->SetMaterial(sMtrl);
	}

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
					izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL,
					0xffffffff,
					5.0f,
					50, 50);
	IZ_ASSERT(m_pSphere != IZ_NULL);

	return IZ_TRUE;
}

IZ_BOOL CStateHemiLight::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTex);
	SAFE_RELEASE(m_pGrid);
	SAFE_RELEASE(m_pSphere);

	return IZ_TRUE;
}

IZ_BOOL CStateHemiLight::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CStateHemiLight::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateHemiLight::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}