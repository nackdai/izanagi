#include "izSampleKit.h"
#include "izThreadModel.h"

class CTimerThreadApp : public izanagi::sample::CSampleApp {
public:
    CTimerThreadApp();
    virtual ~CTimerThreadApp();

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

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

private:
    izanagi::IMemoryAllocator* m_Allocator;
};

class CTask : public izanagi::threadmodel::CTimerTask {
    friend class izanagi::threadmodel::CTask;

protected:
    CTask() {}
    virtual ~CTask() {}

    virtual void OnRun()
    {
        IZ_PRINTF("Run Task.\n");
    }
};

CTimerThreadApp::CTimerThreadApp()
{
}

CTimerThreadApp::~CTimerThreadApp()
{
}

// 初期化.
IZ_BOOL CTimerThreadApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_Allocator = allocator;

    return IZ_TRUE;
}

// 解放.
void CTimerThreadApp::ReleaseInternal()
{
    izanagi::threadmodel::CTimerThread::Terminate();
}

// 更新.
void CTimerThreadApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
}

// 描画.
void CTimerThreadApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    // Nothing is done..
}

IZ_BOOL CTimerThreadApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_SPACE) {
        CTask* task = izanagi::threadmodel::CTask::CreateTask<CTask>(m_Allocator);
        izanagi::threadmodel::CTimerThread::PostDelayedTask(task, 1000.0f, IZ_TRUE);
    }
    return IZ_TRUE;
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CTimerThreadApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "TimerThread",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
