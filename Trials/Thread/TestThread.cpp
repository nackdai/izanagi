#include "izSampleKit.h"
#include "izSystem.h"

class CTestRunnable_1 : public izanagi::sys::IRunnable
{
public:
    CTestRunnable_1() {}
    virtual ~CTestRunnable_1() {}

private:
    virtual void Run(void* userData)
    {
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("1 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
    }
};

class CTestRunnable_2 : public izanagi::sys::IRunnable
{
public:
    CTestRunnable_2() {}
    virtual ~CTestRunnable_2() {}

private:
    virtual void Run(void* userData)
    {
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("2 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
    }
};

class CTestRunnable_3 : public izanagi::sys::IRunnable
{
public:
    CTestRunnable_3() {}
    virtual ~CTestRunnable_3() {}

private:
    virtual void Run(void* userData)
    {
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("3 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
    }
};

int TestThread()
{
    CTestRunnable_1 run_1;
    CTestRunnable_2 run_2;
    CTestRunnable_3 run_3;

    izanagi::sys::CThread thread_1(&run_1, IZ_NULL);
    izanagi::sys::CThread thread_2(&run_2, IZ_NULL);
    izanagi::sys::CThread thread_3(&run_3, IZ_NULL);

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
