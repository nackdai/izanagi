#include "izSampleKit.h"
#include "izThreadModel.h"

class CTaskDelay;
class CTaskInterval;

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

    izanagi::threadmodel::CTimerThread m_Thread;

    CTaskDelay* m_DelayTask;
    CTaskInterval* m_IntervalTask;
};

class CTaskDelay : public izanagi::threadmodel::CTimerTask {
    friend class izanagi::threadmodel::CTask;

protected:
    CTaskDelay() {}
    virtual ~CTaskDelay() {}

    virtual void OnRun(IZ_FLOAT time)
    {
        IZ_PRINTF("Run Task Delay.[%f]\n", time);
    }
};

class CTaskInterval : public izanagi::threadmodel::CTimerTask {
    friend class izanagi::threadmodel::CTask;

protected:
    CTaskInterval() {}
    virtual ~CTaskInterval() {}

    virtual void OnRun(IZ_FLOAT time)
    {
        IZ_PRINTF("Run Task Interval.[%f]\n", time);
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

    m_Thread.Start();

    m_IntervalTask = izanagi::threadmodel::CTask::CreateTask<CTaskInterval>(m_Allocator);
    m_Thread.PostIntervalTask(m_IntervalTask, 100.0f, IZ_TRUE);

    return IZ_TRUE;
}

// 解放.
void CTimerThreadApp::ReleaseInternal()
{
    m_Thread.Join();
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
        m_DelayTask = izanagi::threadmodel::CTask::CreateTask<CTaskDelay>(m_Allocator);
        m_Thread.PostDelayedTask(m_DelayTask, 1000.0f, IZ_TRUE);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_BACK) {
        m_DelayTask->Cancel();
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DELETE) {
        m_IntervalTask->Cancel();
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
