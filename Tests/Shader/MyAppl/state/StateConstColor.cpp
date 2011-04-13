#include <stdafx.h>
#include "StateConstColor.h"
#include "StateManager.h"
#include "MySystem.h"
#include "MyCamera.h"

IZ_BOOL CStateConstColor::Create()
{
	m_pMesh = IZ_NULL;
	m_pShader = IZ_NULL;

	return IZ_TRUE;
}

IZ_BOOL CStateConstColor::Render()
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

void CStateConstColor::Render2D()
{
	static const D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);
}

void CStateConstColor::Render3D()
{
	static const D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 128, 255);

	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	// クリアシーン
	pDevice->Clear(
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		color, 1.0f, 0);

	IZ_UINT nPassNum = m_pShader->Begin(0, IZ_TRUE);
	for (IZ_UINT i = 0; i < nPassNum; i++) {
		IZ_ASSERT(m_pShader->BeginPass(i));
		{
			izanagi::SMatrix mtxL2W;
			izanagi::SetUnitMatrix(mtxL2W);
			m_pShader->SetParamValue(m_hL2W, &mtxL2W, sizeof(mtxL2W));

			const izanagi::SMatrix& mtxW2C = CMyCamera::GetInstance().GetRawInterface().mtxW2C;
			m_pShader->SetParamValue(m_hW2C, &mtxW2C, sizeof(mtxW2C));
		}

		m_pShader->CommitChanges();

		m_pMesh->Draw();

		IZ_ASSERT(m_pShader->EndPass());
	}
	m_pShader->End();
}

IZ_BOOL CStateConstColor::Update()
{
	return IZ_TRUE;
}

IZ_BOOL CStateConstColor::Destroy()
{
	Leave();
	return IZ_TRUE;
}

IZ_BOOL CStateConstColor::Enter()
{
	IZ_BOOL ret = IZ_TRUE;

	izanagi::IMemoryAllocator* pAllocator = CMySystem::GetInstance().GetMemoryAllocator();
	izanagi::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	// 球
	m_pMesh = izanagi::CDebugMeshSphere::CreateDebugMeshSphere(
				pAllocator,
				pDevice,
				izanagi::E_DEBUG_MESH_VTX_FORM_POS | izanagi::E_DEBUG_MESH_VTX_FORM_COLOR,
				IZ_COLOR_RGBA(0xff, 0, 0, 0xff),
				5.0f,
				50, 50);
	IZ_ASSERT(m_pMesh != IZ_NULL);

	// シェーダ
	izanagi::CFileInputStream in;
	IZ_ASSERT(in.Open("data/ConstantColorShader.shd"));
	m_pShader = izanagi::CShaderBasic::CreateShader<izanagi::CShaderBasic>(
					pAllocator,
					pDevice,
					&in);
	IZ_ASSERT(m_pShader != IZ_NULL);
	in.Finalize();

	m_hL2W = m_pShader->GetParameterByName("g_mL2W");
	m_hW2C = m_pShader->GetParameterByName("g_mW2C");

	return ret;
}

IZ_BOOL CStateConstColor::Leave()
{
	SAFE_RELEASE(m_pMesh);
	SAFE_RELEASE(m_pShader);
	return IZ_TRUE;
}
