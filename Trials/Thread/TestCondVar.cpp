#include "izSampleKit.h"
#include "izSystem.h"

static izanagi::sys::CCondVar g_Cond;
static IZ_BOOL g_IsLocked[3] = { IZ_FALSE, IZ_FALSE, IZ_FALSE };

class CTestRunnableCond : public izanagi::sys::IRunnable
{
protected:
    CTestRunnableCond()
    {
        m_Mutex.Open();
    }
    virtual ~CTestRunnableCond()
    {
        m_Mutex.Close();
    }

protected:
    izanagi::sys::CMutex m_Mutex;
};

class CTestRunnableCond_1 : public CTestRunnableCond
{
public:
    CTestRunnableCond_1() {}
    virtual ~CTestRunnableCond_1() {}

private:
    virtual void Run(void* userData)
    {
        m_Mutex.Lock();
        g_IsLocked[0] = IZ_TRUE;
        g_Cond.Wait(m_Mutex);
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("1 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
        m_Mutex.Unlock();
    }
};

class CTestRunnableCond_2 : public CTestRunnableCond
{
public:
    CTestRunnableCond_2() {}
    virtual ~CTestRunnableCond_2() {}

private:
    virtual void Run(void* userData)
    {
        m_Mutex.Lock();
        g_IsLocked[1] = IZ_TRUE;
        g_Cond.Wait(m_Mutex);
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("2 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
        m_Mutex.Unlock();
    }
};

class CTestRunnableCond_3 : public CTestRunnableCond
{
public:
    CTestRunnableCond_3() {}
    virtual ~CTestRunnableCond_3() {}

private:
    virtual void Run(void* userData)
    {
        m_Mutex.Lock();
        g_IsLocked[2] = IZ_TRUE;
        g_Cond.Wait(m_Mutex);
        for (int i = 0; i < 100; i++)
        {
            IZ_PRINTF("3 [%d]\n", i);
            izanagi::sys::CThread::YieldThread();
        }
        m_Mutex.Unlock();
    }
};

int TestCondVar()
{
    g_Cond.Init();

    CTestRunnableCond_1 run_1;
    CTestRunnableCond_2 run_2;
    CTestRunnableCond_3 run_3;

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
            break;
        }
        izanagi::sys::CThread::YieldThread();
    }

    for (int i = 0; i < 100; i++)
    {
        if (i == 40)
        {
            IZ_PRINTF("Signal ********\n");
            g_Cond.Signal();
        }
        else if (i == 90)
        {
            IZ_PRINTF("Broadcast ********\n");
            g_Cond.Broadcast();
        }

        izanagi::sys::CThread::Sleep(100);
    }

    thread_1.Join();
    thread_2.Join();
    thread_3.Join();

    g_Cond.Destroy();

    return 0;
}
