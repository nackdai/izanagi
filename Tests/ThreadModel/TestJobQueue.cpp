#include "izSampleKit.h"
#include "izSystem.h"
#include "izThreadModel.h"

class CTestJob : public izanagi::threadmodel::CJob
{
public:
    CTestJob(IZ_UINT id) : m_ID(id) {}
    virtual ~CTestJob() {}

public:
    virtual void OnExecute()
    {
        izanagi::ThreadId id = izanagi::CThread::GetCurrentThreadId();

        for (IZ_UINT i = 0; i < 100; i++)
        {
            IZ_PRINTF("%d: [%d] {%d}\n", id, m_ID, i);

            izanagi::CThread::Sleep(5);
        }
    }

    virtual void OnFinished()
    {
        IZ_PRINTF("[%d] Finish ******\n", m_ID);
    }

private:
    IZ_UINT m_ID;
};

int TestJobQueue(izanagi::IMemoryAllocator* allocator)
{
    CTestJob job_0(0);
    CTestJob job_1(1);
    CTestJob job_2(2);
    CTestJob job_3(3);

    izanagi::threadmodel::CJobQueue jobQueue;
    jobQueue.Init(allocator, 4);

    jobQueue.Enqueue(&job_0);
    jobQueue.Enqueue(&job_1);
    jobQueue.Enqueue(&job_2);
    jobQueue.Enqueue(&job_3);

    //jobQueue.Join();
    jobQueue.WaitForFinishJobQueue();

    jobQueue.Update();

    jobQueue.Join();

	return 0;
}
