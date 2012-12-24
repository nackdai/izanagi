#include "izSampleKit.h"
#include "izSystem.h"

static IZ_UINT g_Value = 0;
static izanagi::sys::CEvent g_Event;

class CTestRunnableEvent : public izanagi::sys::IRunnable
{
public:
    CTestRunnableEvent()
    {
    }
    virtual ~CTestRunnableEvent()
    {
    }

private:
    virtual void Run(void* userData)
    {
        IZ_UINT i = 0;
        while (g_Event.Wait())
        {
            IZ_PRINTF("[%d]\n", i++);
            if (i > 200)
            {
                break;
            }

            izanagi::sys::CThread::Sleep(10);
        }
    }
};

int TestEvent()
{
    g_Event.Open();
    g_Event.Set();

    CTestRunnableEvent run;

    izanagi::sys::CThread thread_1(&run, IZ_NULL);

    thread_1.Start();

    for (int i = 0; i < 100; i++)
    {
        if (i == 20
            || i == 40
            || i == 60
            || i == 80)
        {
            IZ_PRINTF("Event Set\n");
            g_Event.Set();
            izanagi::sys::CThread::Sleep(100);
        }
        else if (i == 10
            || i == 30
            || i == 50
            || i == 70
            || i == 90)
        {
            IZ_PRINTF("Event Reset\n");
            g_Event.Reset();
            izanagi::sys::CThread::Sleep(100);
        }

        izanagi::sys::CThread::Sleep(10);
    }

    IZ_PRINTF("Event Set\n");
    g_Event.Set();

    thread_1.Join();
    g_Event.Close();

    return 0;
}
