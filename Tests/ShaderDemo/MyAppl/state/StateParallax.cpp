#include <stdafx.h>
#include "StateParallax.h"
#include "MySystem.h"
#include "MyCamera.h"
#include "MyAppl.h"
#include "DebugInfoRenderer.h"
#include "TextureManager.h"

CStateParallax::CStateParallax()
{
	m_pShader = IZ_NULL;

	m_pGrid = IZ_NULL;
	m_pAxis = IZ_NULL;
	m_pMesh = IZ_NULL;
}

CStateParallax::~CStateParallax()
{
}

IZ_BOOL CStateParallax::Create()
{
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Render2D()
{
	CDebugInfoRenderer::Render("Parallax");
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::Render3D()
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
			izanagi::SVector::Normalize(sParallel.vDir, sParallel.vDir);

			// ローカル座標に変換する
			izanagi::ApplyMatrixXYZ(sParallel.vDir, sParallel.vDir, mtxW2L);

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

	// グリッド描画
	m_pShader->BeginPass(1);
	m_pGrid->Draw();
	m_pAxis->Draw();
	m_pShader->EndPass();

	// テクスチャセット
	izanagi::CTexture* pTex = izanagi::CTextureManager::GetInstance().Get(DEMO_TEX_WOOD);
	izanagi::CTexture* pNormaMap = izanagi::CTextureManager::GetInstance().Get(DEMO_TEX_NMH_FOUR);
	pDevice->SetTexture(0, pTex);
	pDevice->SetTexture(1, pNormaMap);

	IZ_BOOL bIsOnShader = CMyAppl::GetInstance().IsOnShader();

	// メッシュ描画
	m_pShader->BeginPass(bIsOnShader ? 0 : 2);
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

IZ_BOOL CStateParallax::Enter()
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
	SAFE_RELEASE(m_pGrid);
	SAFE_RELEASE(m_pAxis);
	SAFE_RELEASE(m_pMesh);

	return IZ_TRUE;
}

IZ_BOOL CStateParallax::OnKeyDown(IZ_UINT nChar, IZ_UINT nRepCnt, IZ_UINT nFlags)
{
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::OnMouseMove(IZ_UINT nFlags, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateParallax::OnMouseWheel(IZ_UINT nFlags, IZ_SHORT zDelta, IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}