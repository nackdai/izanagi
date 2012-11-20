#include "StateHDR.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "EnvBox.h"
#include "MySystem.h"
#include "PostEffectSample.h"

IZ_BOOL CStateHDR::Init()
{
	return IZ_TRUE;
}

IZ_BOOL CStateHDR::Render(izanagi::graph::CGraphicsDevice* device)
{
	Render3D();
	Render2D();

	return IZ_TRUE;
}

void CStateHDR::Render2D()
{
	static const D3DCOLOR color = D3DCOLOR_RGBA(255, 255, 255, 255);

	/*
	if (CMySystem::GetInstance().GetGraphicsDevice()->Begin2D()) {
		CMySystem::GetInstance().GetDebugFont()->Begin();

		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			0, 0,
			color,
			"Monotone\n");
		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			color,
			"Back : Return Menu\n");
		CMySystem::GetInstance().GetDebugFont()->DBPrint(
			color,
			"Space : On/Off\n");

		CMySystem::GetInstance().GetDebugFont()->End();
		CMySystem::GetInstance().GetGraphicsDevice()->End2D();
	}
	*/
}

void CStateHDR::Render3D()
{
	static const D3DCOLOR color = D3DCOLOR_ARGB(255, 0, 128, 255);

	izanagi::graph::CGraphicsDevice* pDevice = CMySystem::GetInstance().GetGraphicsDevice();

	CPostEffectSample::GetInstance().BeginScene();

	// クリアシーン
	pDevice->Clear(
		D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
		color, 1.0f, 0);

	CEnvBox::GetInstance().Render(pDevice);
	CSceneRenderer::GetInstance().Render(pDevice);

	pDevice->SetRenderState(
		izanagi::graph::E_GRAPH_RS_CULLMODE,
		izanagi::graph::E_GRAPH_CULL_NONE);

	CPostEffectSample::GetInstance().Apply();
}

IZ_BOOL CStateHDR::Update()
{
	if (m_bBack) {
		CStateManager::GetInstance().ChangePrevState();
		return IZ_TRUE;
	}

	return IZ_TRUE;
}

IZ_BOOL CStateHDR::Destroy()
{
	return IZ_TRUE;
}

IZ_BOOL CStateHDR::Enter(izanagi::IMemoryAllocator* allocator, void* val)
{
	m_bBack = IZ_FALSE;

	IZ_BOOL ret = CPostEffectSample::GetInstance().Read("data/HDR2.pes");

	return ret;
}

IZ_BOOL CStateHDR::Leave()
{
	return IZ_TRUE;
}

IZ_BOOL CStateHDR::OnKeyDown(UINT nChar)
{
	if (nChar == VK_BACK) {
		m_bBack = IZ_TRUE;
	}

	return IZ_TRUE;
}
