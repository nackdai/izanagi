#include "izSampleKit.h"
#include "StateManager.h"
#include "StateBase.h"

static const IZ_UINT BUF_SIZE = 8 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class CShaderApp : public izanagi::sample::CSampleApp {
public:
	CShaderApp();
	virtual ~CShaderApp();

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
	virtual void UpdateInternal(izanagi::CGraphicsDevice* device);

	// �`��.
	virtual void RenderInternal(izanagi::CGraphicsDevice* device);

    // �w�i�F�擾.
	virtual IZ_COLOR GetBgColor() const;

private:
	izanagi::IMemoryAllocator* m_Allocator;
	izanagi::CGraphicsDevice* m_Device;
};

CShaderApp::CShaderApp()
{
	m_Allocator = IZ_NULL;
	m_Device = IZ_NULL;
}

CShaderApp::~CShaderApp()
{
}

// �L�[�{�[�h����
IZ_BOOL CShaderApp::OnKeyDown(IZ_UINT nChar)
{
	return CStateManager::GetInstance().OnKeyDown(nChar);
}

// ������.
IZ_BOOL CShaderApp::InitInternal(
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

	CStateManager::GetInstance().Create(
		this,
		const_cast<izanagi::SCameraParam&>(camera.GetParam()));

	return CStateManager::GetInstance().Init();
}

// ���.
void CShaderApp::ReleaseInternal()
{
	CStateManager::GetInstance().Destroy();
	SAFE_RELEASE(m_Device);
}

// �X�V.
void CShaderApp::UpdateInternal(izanagi::CGraphicsDevice* device)
{
	CStateManager::GetInstance().Update(m_Allocator, m_Device);
}

// �`��.
void CShaderApp::RenderInternal(izanagi::CGraphicsDevice* device)
{
	CStateManager::GetInstance().Render(device);
}

// �w�i�F�擾.
IZ_COLOR CShaderApp::GetBgColor() const
{
    izanagi::CSceneStateBase* state = CStateManager::GetInstance().GetState(
        CStateManager::GetInstance().GetCurrentState());

    IZ_BOOL enableBgColor = (reinterpret_cast<CStateBase*>(state))->EnableBgColor();
    if (enableBgColor)
    {
        return (reinterpret_cast<CStateBase*>(state))->GetBgColor();
    }

    return CSampleApp::GetBgColor();
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CShaderApp app;

	int ret = SampleMain(
		hInstance,
		&app,
		"Shader",
		SCREEN_WIDTH, SCREEN_HEIGHT,
		BUF, BUF_SIZE,
		GFX_BUF, GFX_BUF_SIZE);

	return ret;
}
