#include "izSampleKit.h"
#include "StateManager.h"
#include "SceneRenderer.h"

static const IZ_UINT BUF_SIZE = 8 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CPostEffectApp : public izanagi::sample::CSampleApp {
public:
	CPostEffectApp();
	virtual ~CPostEffectApp();

public:
	// �L�[�{�[�h����
	virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);

protected:
	// ������.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera);

	// ���.
	virtual void ReleaseInternal();

	// �X�V.
	virtual void UpdateInternal(
		izanagi::CCamera& camera,
		izanagi::CGraphicsDevice* device);

	// �`��.
	virtual void RenderInternal(izanagi::CGraphicsDevice* device);

private:
	izanagi::IMemoryAllocator* m_Allocator;
	izanagi::CGraphicsDevice* m_Device;
};

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
	return CStateManager::GetInstance().OnKeyDown(nChar);
}

// ������.
IZ_BOOL CPostEffectApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::CGraphicsDevice* device,
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
	SAFE_RELEASE(m_Device);
}

// �X�V.
void CPostEffectApp::UpdateInternal(
	izanagi::CCamera& camera,
	izanagi::CGraphicsDevice* device)
{
	CStateManager::GetInstance().Update(m_Allocator, m_Device);
}

// �`��.
void CPostEffectApp::RenderInternal(izanagi::CGraphicsDevice* device)
{
	CStateManager::GetInstance().Render(device);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CPostEffectApp app;

	int ret = SampleMain(
		hInstance,
		&app,
		"PostEffect",
		SCREEN_WIDTH, SCREEN_HEIGHT,
		BUF, BUF_SIZE,
		GFX_BUF, GFX_BUF_SIZE);

	return ret;
}
