#include "izSampleKit.h"
#include "izThreadModel.h"

class CMessageQueueApp : public izanagi::sample::CSampleApp {
public:
    CMessageQueueApp();
    virtual ~CMessageQueueApp();

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

public:
    void AddMessage();
    void TerminateMessage();
    IZ_BOOL IsTerminated();

private:
    izanagi::sys::CThread m_Thread;
    IZ_BOOL m_IsTerminated;

    izanagi::threadmodel::CMessageQueue m_MsgQueue;

    izanagi::IMemoryAllocator* m_Allocator;
};

class CRunnable : public izanagi::sys::IRunnable {
public:
    CRunnable() {}
    virtual ~CRunnable() {}

    virtual void Run(void* userData)
    {
        CMessageQueueApp* app = (CMessageQueueApp*)userData;

        for (;;) {
            app->AddMessage();

            if (app->IsTerminated()) {
                break;
            }

            izanagi::sys::CThread::Sleep(10);
        }
    }
};

static CRunnable runnable;

class CMessageSample : public izanagi::threadmodel::CMessage {
public:
    CMessageSample() {}
    virtual ~CMessageSample() {}
};

CMessageQueueApp::CMessageQueueApp()
{
    m_Thread.Init(&runnable, this);
    m_IsTerminated = IZ_FALSE;
}

CMessageQueueApp::~CMessageQueueApp()
{
}

void CMessageQueueApp::AddMessage()
{
    if (m_MsgQueue.GetPostedMessageNum() < 1) {
        CMessageSample* msg = izanagi::threadmodel::CMessage::CreateMessage<CMessageSample>(m_Allocator);
        m_MsgQueue.Post(msg);
    }
}

void CMessageQueueApp::TerminateMessage()
{
    m_IsTerminated = IZ_TRUE;
}

IZ_BOOL CMessageQueueApp::IsTerminated()
{
    return m_IsTerminated;
}

// 初期化.
IZ_BOOL CMessageQueueApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    m_Allocator = allocator;

    m_Thread.Start();

    return IZ_TRUE;
}

// 解放.
void CMessageQueueApp::ReleaseInternal()
{
    TerminateMessage();
    m_Thread.Join();

    m_MsgQueue.WaitEmpty(IZ_TRUE);
}

// 更新.
void CMessageQueueApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::threadmodel::CMessage* msg = m_MsgQueue.Peek();
    if (msg != IZ_NULL) {
        IZ_PRINTF("Get Message\n");
        izanagi::threadmodel::CMessage::DeleteMessage(msg);
    }
}

// 描画.
void CMessageQueueApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    // Nothing is done..
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CMessageQueueApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "MessageQueue",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
