#include "izSampleKit.h"
#include "izSystem.h"

static IZ_UINT g_Value = 0;

class CTestRunnableMutex : public izanagi::IRunnable
{
public:
    CTestRunnableMutex()
    {
        m_Mutex.Open();
    }
    virtual ~CTestRunnableMutex()
    {
        m_Mutex.Close();
    }

private:
    virtual void Run(void* userData)
    {
        for (;;)
        {
            m_Mutex.Lock();
            {
                g_Value++;
                IZ_PRINTF("(%d): [%d]\n", m_Mutex.GetOwnerThreadId(), g_Value);
                if (g_Value > 1000)
                {
                    m_Mutex.Unlock();
                    return;
                }
            }
            m_Mutex.Unlock();

            izanagi::CThread::YieldThread();
        }
    }

private:
    izanagi::CMutex m_Mutex;
};

int TestMutex()
{
    CTestRunnableMutex run;

    izanagi::CThread thread_1(&run, IZ_NULL);
    izanagi::CThread thread_2(&run, IZ_NULL);
    izanagi::CThread thread_3(&run, IZ_NULL);

    thread_1.Start();
    thread_2.Start();
    thread_3.Start();

    for (int i = 0; i < 100; i++)
    {
        izanagi::CThread::Sleep(10);
    }

    thread_1.Join();
    thread_2.Join();
    thread_3.Join();

	return 0;
}
