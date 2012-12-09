#include "izSampleKit.h"
#include "izSystem.h"

static IZ_UINT g_Count = 0;

class CTestRunnableSema : public izanagi::sys::IRunnable
{
public:
    CTestRunnableSema()
    {
        m_Sema.Init(2);
    }
    virtual ~CTestRunnableSema()
    {
        m_Sema.Close();
    }

private:
    virtual void Run(void* userData)
    {
        IZ_UINT id = *(IZ_UINT*)userData;

        m_Sema.Wait();
        g_Count++;
        for (IZ_UINT i = 0; i < 100; i++)
        {
            IZ_PRINTF("%d : (%d) [%d]\n", g_Count, id, i);
            izanagi::sys::CThread::YieldThread();
        }
        m_Sema.Release();
    }

private:
    izanagi::sys::CSemaphore m_Sema;
};


int TestSemaphore()
{
    CTestRunnableSema run;

    IZ_UINT id[3] = {0, 1, 2};

    izanagi::sys::CThread thread_1(&run, (void*)&id[0]);
    izanagi::sys::CThread thread_2(&run, (void*)&id[1]);
    izanagi::sys::CThread thread_3(&run, (void*)&id[2]);

    thread_1.Start();
    thread_2.Start();
    thread_3.Start();

    for (int i = 0; i < 100; i++)
    {
        izanagi::sys::CThread::Sleep(10);
    }

    thread_1.Join();
    thread_2.Join();
    thread_3.Join();

	return 0;
}
