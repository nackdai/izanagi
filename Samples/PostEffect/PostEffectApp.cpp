#include "PostEffectApp.h"
#include "StateManager.h"

CPostEffectApp::CPostEffectApp()
{
	m_Allocator = IZ_NULL;
	m_Device = IZ_NULL;
}

CPostEffectApp::~CPostEffectApp()
{
}

// �L�[�{�[�h����
IZ_BOOL CPostEffectApp::OnKeyDown(IZ_UINT nChar)
{
    if (nChar == VK_SPACE)
    {
        CPostEffectSample::GetInstance()->ToggleEnablePostEffect();
        return IZ_TRUE;
    }

    return CStateManager::GetInstance().OnKeyDown(nChar);
}

// ������.
IZ_BOOL CPostEffectApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::graph::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	m_Allocator = allocator;
	SAFE_REPLACE(m_Device, device);

	// �J����
	camera.Init(
		izanagi::CVector(0.0f, 0.0f, 30.0f, 1.0f),
		izanagi::CVector(0.0f, 0.0f, 0.0f, 1.0f),
		izanagi::CVector(0.0f, 1.0f, 0.0f, 1.0f),
		1.0f,
		500.0f,
		izanagi::CMath::Deg2Rad(60.0f),
		(IZ_FLOAT)SCREEN_WIDTH / SCREEN_HEIGHT);
	camera.Update();

    VRETURN(
        CSceneRenderer::GetInstance()->Init(
            allocator,
            device));

    VRETURN(
        CPostEffectSample::GetInstance()->Init(
            allocator,
            device));

    CStateManager::GetInstance().Create(
		this,
		const_cast<izanagi::SCameraParam&>(camera.GetParam()));

    return CStateManager::GetInstance().Init();
}

// ���.
void CPostEffectApp::ReleaseInternal()
{
	CStateManager::GetInstance().Destroy();
    CSceneRenderer::GetInstance()->Release();
    CPostEffectSample::GetInstance()->Release();
	SAFE_RELEASE(m_Device);
}

// �X�V.
void CPostEffectApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    CStateManager::GetInstance().Update(m_Allocator, device);
}

// �`��.
void CPostEffectApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    CStateManager::GetInstance().Render(device);
}
