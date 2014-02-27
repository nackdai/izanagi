#include "izSampleKit.h"
#include "izThreadModel.h"

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

private:
    izanagi::threadmodel::CJobQueue m_JobQueue;

    izanagi::IMemoryAllocator* m_Allocator;
};

class CJobSample : public izanagi::threadmodel::CJob {
public:
    CJobSample() {}
    virtual ~CJobSample() {}

    virtual IZ_BOOL OnRun()
    {
        IZ_PRINTF("Run!!\n");
        return IZ_TRUE;
    }

    virtual void OnFinish(IZ_BOOL runResult)
    {
        if (IsCanceled()) {
            IZ_PRINTF("Finish Canceled\n");
        }
        else {
            IZ_PRINTF("Finish %s\n", runResult ? "Succeeded" : "Failed");
        }
    }

    virtual void OnCancel()
    {
        IZ_PRINTF("Cancel\n");
    }
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
    VRETURN(
        m_JobQueue.Init(
            allocator,
            1));

    m_JobQueue.Start();

    m_Allocator = allocator;

    return IZ_TRUE;
}

// 解放.
void CJobQueueApp::ReleaseInternal()
{
    m_JobQueue.Terminate();

    izanagi::threadmodel::CJobQueue::TerminateJobQueue();
}

// 更新.
void CJobQueueApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::threadmodel::CJob* job = izanagi::threadmodel::CJob::CreateJob<CJobSample>(m_Allocator);

    m_JobQueue.Enqueue(job, IZ_TRUE);

    izanagi::threadmodel::CJobQueue::UpdateQueues();
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

IzMain()
{
    CJobQueueApp app;

    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "JobQueue",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
