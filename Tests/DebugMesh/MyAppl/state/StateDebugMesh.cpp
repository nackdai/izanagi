#include "StateDebugMesh.h"
#include "MySystem.h"
#include "MyCamera.h"

CStateDebugMesh::CStateDebugMesh()
{
	m_pShader = IZ_NULL;

	m_pTex = IZ_NULL;

	m_pGrid = IZ_NULL;
	m_pSphere = IZ_NULL;
	m_pCylinder = IZ_NULL;
	m_pBox = IZ_NULL;
	m_pTorus = IZ_NULL;
	m_pRect = IZ_NULL;
	m_pAxis = IZ_NULL;
	m_pFrustum = IZ_NULL;

	m_CurMesh = 0;
	izanagi::CStdUtil::FillZero(m_MeshList, COUNTOF(m_MeshList));

	m_Flags.val = 0;
}

CStateDebugMesh::~CStateDebugMesh()
{
}

IZ_BOOL CStateDebugMesh::Init()
{
	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::Render(izanagi::CGraphicsDevice* device)
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::Render2D()
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

IZ_BOOL CStateDebugMesh::Render3D()
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
	m_pAxis->Draw();
	m_pShader->EndPass();

	izanagi::CDebugMesh* pMesh = m_MeshList[m_CurMesh];

	IZ_UINT nVtxFormFlag = pMesh->GetVtxFormFlag();

	if (nVtxFormFlag & izanagi::E_DEBUG_MESH_VTX_FORM_UV) {
		// テクスチャセット
		pDevice->SetTexture(0, m_pTex);

		m_pShader->BeginPass(0);
	}
	else {
		m_pShader->BeginPass(1);
	}

	// メッシュ描画
	pMesh->Draw(m_Flags.isDrawDebugAxis);

	m_pShader->EndPass();

	// 描画終了
	m_pShader->End();

	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::Destroy()
{
	Leave();
	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::Enter(izanagi::IMemoryAllocator* allocator, void* val)
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
					//izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV,
					//izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL,
					izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT,
					0xffffffff,
					5.0f,
					50, 50);
	IZ_ASSERT(m_pSphere != IZ_NULL);

	// 円柱
	m_pCylinder = izanagi::CDebugMeshCylinder::CreateDebugMeshCylinder(
					pAllocator,
					pDevice,
					//izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV,
					//izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL,
					izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT,
					0xffffffff,
					5.0f,
					10.0f,
					50, 10);
	IZ_ASSERT(m_pCylinder != IZ_NULL);

	// ボックス
	m_pBox = izanagi::CDebugMeshBox::CreateDebugMeshBox(
				pAllocator,
				pDevice,
				//izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV,
				//izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL,
				izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT,
				0xffffffff,
				10.0f, 10.0f, 10.0f);
	IZ_ASSERT(m_pBox != IZ_NULL);

	// トーラス
	m_pTorus = izanagi::CDebugMeshTorus::CreateDebugMeshTorus(
				pAllocator,
				pDevice,
				//izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV,
				izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT,
				0xffffffff,
				5.0f,
				10.0f,
				30, 30);
	IZ_ASSERT(m_pTorus != IZ_NULL);

	// 矩形
	m_pRect = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
				pAllocator,
				pDevice,
				izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR | izanagi::E_DEBUG_MESH_VTX_FORM_UV | izanagi::E_DEBUG_MESH_VTX_FORM_NORMAL | izanagi::E_DEBUG_MESH_VTX_FORM_TANGENT,
				0xffffffff,
				10, 10,
				20.0f, 20.0f);
	IZ_ASSERT(m_pRect != IZ_NULL);

	izanagi::CCamera& cCamera = CMyCamera::GetInstance().GetRawInterface();

	// 視錐台
	m_pFrustum = izanagi::CDebugMeshFrustum::CreateDebugMeshFrustum(
					pAllocator,
					pDevice,
					IZ_COLOR_RGBA(0xff, 0, 0, 0x80),
					cCamera.GetParam().aspect,
					cCamera.GetParam().fov,
					1.0f, 100.0f);
					//cCamera.cameraNear,
					//cCamera.cameraFar);
	IZ_ASSERT(m_pFrustum != IZ_NULL);

	// 軸
	m_pAxis = izanagi::CDebugMeshAxis::CreateDebugMeshAxisDefault(
				pAllocator,
				pDevice);
	IZ_ASSERT(m_pAxis != IZ_NULL);

	m_MeshList[0] = m_pSphere;
	m_MeshList[1] = m_pTorus;
	m_MeshList[2] = m_pCylinder;
	m_MeshList[3] = m_pBox;
	m_MeshList[4] = m_pRect;
	m_MeshList[5] = m_pFrustum;

	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::Leave()
{
	SAFE_RELEASE(m_pShader);
	SAFE_RELEASE(m_pTex);

	SAFE_RELEASE(m_pGrid);
	SAFE_RELEASE(m_pSphere);
	SAFE_RELEASE(m_pCylinder);
	SAFE_RELEASE(m_pBox);
	SAFE_RELEASE(m_pTorus);
	SAFE_RELEASE(m_pRect);
	SAFE_RELEASE(m_pAxis);
	SAFE_RELEASE(m_pFrustum);

	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::OnKeyDown(IZ_UINT nChar)
{
	switch (nChar) {
	case VK_UP:
		m_CurMesh--;
		m_CurMesh = (m_CurMesh < 0 ? COUNTOF(m_MeshList) - 1 : m_CurMesh);
		break;
	case VK_DOWN:
		m_CurMesh++;
		m_CurMesh = (m_CurMesh > COUNTOF(m_MeshList) - 1 ? 0 : m_CurMesh);
		break;
	case 'A':
		m_Flags.isDrawDebugAxis = !m_Flags.isDrawDebugAxis;
		break;
	}

	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::OnMouseMove(IZ_INT x, IZ_INT y)
{
	return IZ_TRUE;
}

IZ_BOOL CStateDebugMesh::OnMouseWheel(IZ_SHORT zDelta)
{
	return IZ_TRUE;
}