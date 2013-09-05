#include "izSampleKit.h"

class CJobQueueApp : public izanagi::sample::CSampleApp {
public:
    CJobQueueApp();
    virtual ~CJobQueueApp();

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
};

CJobQueueApp::CJobQueueApp()
{
}

CJobQueueApp::~CJobQueueApp()
{
}

// 初期化.
IZ_BOOL CJobQueueApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    return IZ_TRUE;
}

// 解放.
void CJobQueueApp::ReleaseInternal()
{
}

// 更新.
void CJobQueueApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    // TODO
}

// 描画.
void CJobQueueApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    // Nothing is done..
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    CJobQueueApp app;

    int ret = SampleMain(
        hInstance,
        &app,
        "JobQueueApp",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
