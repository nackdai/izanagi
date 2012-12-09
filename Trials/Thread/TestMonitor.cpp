#include "izSampleKit.h"
#include "izSystem.h"

static izanagi::sys::CMonitor g_Monitor;
static IZ_BOOL g_IsLocked[3] = { IZ_FALSE, IZ_FALSE, IZ_FALSE };

class CTestRunnableMonitor_1 : public izanagi::sys::IRunnable
{
public:
    CTestRunnableMonitor_1() {}
    virtual ~CTestRunnableMonitor_1() {}

private:
    virtual void Run(void* userData)
    {
        g_Monitor.Lock();
        g_IsLocked[0] = IZ_TRUE;
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("1 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
        g_Monitor.Notify();
        g_Monitor.Unlock();
    }
};

class CTestRunnableMonitor_2 : public izanagi::sys::IRunnable
{
public:
    CTestRunnableMonitor_2() {}
    virtual ~CTestRunnableMonitor_2() {}

private:
    virtual void Run(void* userData)
    {
        g_Monitor.Lock();
        g_IsLocked[1] = IZ_TRUE;
        g_Monitor.Wait();
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("2 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
        g_Monitor.Unlock();
    }
};

class CTestRunnableMonitor_3 : public izanagi::sys::IRunnable
{
public:
    CTestRunnableMonitor_3() {}
    virtual ~CTestRunnableMonitor_3() {}

private:
    virtual void Run(void* userData)
    {
        g_Monitor.Lock();
        g_IsLocked[2] = IZ_TRUE;
        g_Monitor.Wait();
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("3 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
        g_Monitor.Unlock();
    }
};

int TestMonitor()
{
    g_Monitor.Init();

    CTestRunnableMonitor_1 run_1;
    CTestRunnableMonitor_2 run_2;
    CTestRunnableMonitor_3 run_3;

    izanagi::sys::CThread thread_1(&run_1, IZ_NULL);
    izanagi::sys::CThread thread_2(&run_2, IZ_NULL);
    izanagi::sys::CThread thread_3(&run_3, IZ_NULL);

    thread_1.Start();
    thread_2.Start();
    thread_3.Start();

    for (;;)
    {
        if (g_IsLocked[0] && g_IsLocked[1] && g_IsLocked[2])
        {
            g_Monitor.NotifyAll();
            break;
        }
        izanagi::sys::CThread::YieldThread();
    }

    thread_1.Join();
    thread_2.Join();
    thread_3.Join();

    g_Monitor.Destroy();

	return 0;
}
