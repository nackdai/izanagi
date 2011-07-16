#include <stdafx.h>
#include "StatePhong.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "MyAppl.h"
#include "DebugInfoRenderer.h"
#include "TextureManager.h"

CStatePhong::CStatePhong()
{
	m_pShader = IZ_NULL;

	m_pGrid = IZ_NULL;
	m_pAxis = IZ_NULL;
	m_pMesh = IZ_NULL;
}

CStatePhong::~CStatePhong()
{
}

IZ_BOOL CStatePhong::Create()
{
	return IZ_TRUE;
}

IZ_BOOL CStatePhong::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

IZ_BOOL CStatePhong::Render2D()
{
	CDebugInfoRenderer::Render("Phong");
	return IZ_TRUE;
}

IZ_BOOL CStatePhong::Render3D()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	// 描画開始
	m_pShader->Begin(0);

	// 念のため確認
	IZ_UINT nPassCnt = m_pShader->GetPassCount();
	IZ_ASSERT(nPassCnt >= 2);

	// シェーダパラメータセット
	{
		// Local -> World
		izanagi::SMatrix mtxL2W;
		izanagi::SetUnitMatrix(mtxL2W);
		m_pShader->SetL2W(mtxL2W);

		// ローカル座標変換行列
		izanagi::SMatrix mtxW2L;
		izanagi::InverseMatrix(mtxW2L, mtxL2W);

		m_pShader->SetW2C(CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2C);

		// View -> Local
		izanagi::SMatrix mtxV2L;
		izanagi::MulMatrix(
			mtxV2L, 
			mtxL2W, 
			CMyCamera::GetInstance().GetRawInterface().GetParam().mtxW2V);
		izanagi::InverseMatrix(mtxV2L, mtxV2L);

		// ビュー座標系における視点は常に原点
		izanagi::SVector vEyePos;
		vEyePos.Set(0.0f, 0.0f, 0.0f);

		// 視点の位置をローカル座標に変換する
		izanagi::SVector vLocalEye;
		izanagi::ApplyMatrix(
			vLocalEye,
			vEyePos,
			mtxV2L);

		m_pShader->SetCameraPos(vLocalEye);

		// ライト
		izanagi::SParallelLightParam sParallel;
		{
			sParallel.vDir.Set(-1.0f, -1.0f, -1.0f, 0.0f);
			izanagi::NormalizeVector(sParallel.vDir, sParallel.vDir);

			// ローカル座標に変換する
			izanagi::ApplyMatrixXYZ(sParallel.vDir, sParallel.vDir, mtxW2L);

			sParallel.color.Set(1.0f, 1.0f, 1.0f, 1.0f);
		}

		izanagi::SAmbientLightParam sAmbient;
		izanagi::SetZeroVector(sAmbient.color);

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

	// グリッド描画
	m_pShader->BeginPass(1);
	m_pGrid->Draw();
	m_pAxis->Draw();
	m_pShader->EndPass();

	// テクスチャセット
	izanagi::CTexture* pTex = izanagi::CTextureManager::GetInstance().Get(DEMO_TEX_EARTH);
	pDevice->SetTexture(0, pTex);

	IZ_BOOL bIsOnShader = CMyAppl::GetInstance().IsOnShader();

	// メッシュ描画
	m_pShader->BeginPass(bIsOnShader ? 0 : 2);
	m_pMesh->Draw();
	m_pShader->EndPass();

	// 描画終了
	m_pShader->End();

	return IZ_TRUE;
}

IZ_BOOL CStatePhong::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStatePhong::Destroy()
{
	Leave();
	return IZ_TRUE;
}

IZ_BOOL CStatePhong::Enter()
{
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();
	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();

	// シェーダ
	m_pShader = CShaderBase::CreateShader<CPhongShader>(
					pAllocator,
					pDevice,
					"./data/PhongShader.bin");
	IZ_ASSERT(m_pShader != IZ_NULL);

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

	// 球
	m_pMesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
					pAllocator,
					pDevice,
					izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL,
					0xffffffff,
					5.0f,
					50, 50);
	IZ_ASSERT(m_pMesh != IZ_NULL);

	return IZ_TRUE;
}

IZ_BOOL CStatePhong::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pGrid);
	SAFE_RELEASE(m_pAxis);
	SAFE_RELEASE(m_pMesh);

	return IZ_TRUE;
}

IZ_BOOL CStatePhong::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CStatePhong::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStatePhong::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}