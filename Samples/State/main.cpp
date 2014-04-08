#include "izSampleKit.h"
#include "StateManager.h"

class CSStateApp : public izanagi::sample::CSampleApp {
public:
    CSStateApp();
    virtual ~CSStateApp();

public:
    // キーボード押下
    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::IMemoryAllocator* m_Allocator;
    izanagi::graph::CGraphicsDevice* m_Device;
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
IZ_BOOL CSStateApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return CStateManager::GetInstance().OnKeyDown(key);
}

// 初期化.
IZ_BOOL CSStateApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
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
void CSStateApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    CStateManager::GetInstance().Update(m_Allocator, m_Device);
}

// 描画.
void CSStateApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    CStateManager::GetInstance().Render(device);
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CSStateApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "State",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
