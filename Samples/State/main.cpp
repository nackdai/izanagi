#include "izSampleKit.h"
#include "StateManager.h"

class CSStateApp : public izanagi::sample::CSampleApp {
public:
	CSStateApp();
	virtual ~CSStateApp();

public:
	// キーボード押下
	virtual IZ_BOOL OnKeyDown(IZ_UINT nChar);

protected:
	// 初期化.
	virtual IZ_BOOL InitInternal(
		izanagi::IMemoryAllocator* allocator,
		izanagi::CGraphicsDevice* device,
		izanagi::sample::CSampleCamera& camera);

	// 解放.
	virtual void ReleaseInternal();

	// 更新.
	virtual void UpdateInternal(izanagi::CCamera& camera);

	// 描画.
	virtual void RenderInternal(izanagi::CGraphicsDevice* device);

private:
	izanagi::IMemoryAllocator* m_Allocator;
	izanagi::CGraphicsDevice* m_Device;
};

CSStateApp::CSStateApp()
{
	m_Allocator = IZ_NULL;
	m_Device = IZ_NULL;
}

CSStateApp::~CSStateApp()
{
}

// キーボード押下
IZ_BOOL CSStateApp::OnKeyDown(IZ_UINT nChar)
{
	return CStateManager::GetInstance().OnKeyDown(nChar);
}

// 初期化.
IZ_BOOL CSStateApp::InitInternal(
	izanagi::IMemoryAllocator* allocator,
	izanagi::CGraphicsDevice* device,
	izanagi::sample::CSampleCamera& camera)
{
	m_Allocator = allocator;
	SAFE_REPLACE(m_Device, device);

	return CStateManager::GetInstance().Init();
}

// 解放.
void CSStateApp::ReleaseInternal()
{
	CStateManager::GetInstance().Destroy();
	SAFE_RELEASE(m_Device);
}

// 更新.
void CSStateApp::UpdateInternal(izanagi::CCamera& camera)
{
	CStateManager::GetInstance().Update(m_Allocator, m_Device);
}

// 描画.
void CSStateApp::RenderInternal(izanagi::CGraphicsDevice* device)
{
	static const IZ_COLOR bgColor = IZ_COLOR_RGBA(0, 128, 255, 255);

	device->BeginRender(
		izanagi::E_GRAPH_CLEAR_FLAG_ALL,
		bgColor, 1.0f, 0);
	{
		CStateManager::GetInstance().Render(device);
	}
	device->EndRender();
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	CSStateApp app;

	izanagi::CStandardMemoryAllocator allocator(BUF_SIZE, BUF);
	izanagi::CStandardMemoryAllocator allocatorForGraph(GFX_BUF_SIZE, GFX_BUF);

	izanagi::sample::SSampleParam sampleParam = {
		&app,
		&allocator,
		&allocatorForGraph,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		IZ_TRUE,
		"State",
		hInstance,
	};

	IZ_BOOL result = SampleMainLoop(sampleParam);

	int ret = (result ? 0 : 1);

	return ret;
}
