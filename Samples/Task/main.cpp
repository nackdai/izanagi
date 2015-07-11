#include "izSampleKit.h"
#include "izThreadModel.h"

static IZ_INT Data[100];

class CTask : public izanagi::threadmodel::CTask{
public:
    CTask() {}
    virtual ~CTask() {}

private:
    virtual void OnRun()
    {
        for (IZ_UINT i = m_From; i < m_To; i++) {
            IZ_PRINTF("Task %d\n", i);
            Data[i] = i - 1;
        }

        izanagi::sys::CThread::Sleep(100);
    }

public:
    void SetRange(IZ_UINT from, IZ_UINT to)
    {
        m_From = from;
        m_To = to;
    }

private:
    IZ_UINT m_From;
    IZ_UINT m_To;
};

IZ_UINT8 buffer[1 * 1024 * 1024];

IzMain(0, 0)
{
    izanagi::CStandardMemoryAllocator allocator;
    allocator.Init(sizeof(buffer), buffer);

    CTask tasks[4];
    tasks[0].SetRange( 0,  25);
    tasks[1].SetRange(25,  50);
    tasks[2].SetRange(50,  75);
    tasks[3].SetRange(75, 100);

#if 1
    // Sample 1
    // Simple Sample
    {
		IZ_PRINTF("Sample 1 *****\n");

		izanagi::threadmodel::CThreadPool threadPool;
		threadPool.Init(&allocator, 4);

        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            Data[i] = i;
        }

        for (IZ_UINT i = 0; i < COUNTOF(tasks); i++) {
			threadPool.EneueueTask(&tasks[i]);
        }

        for (IZ_UINT i = 0; i < COUNTOF(tasks); i++) {
            tasks[i].Wait();
        }
    
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            if (Data[i] != i - 1) {
                IZ_PRINTF("Failed Sample1 !!!\n");
                IZ_ASSERT(IZ_FALSE);
            }
        }

		threadPool.Terminate();
    }
#endif

    // Sample 2
    // Only one thread.
    {
		IZ_PRINTF("Sample 2 *****\n");

		izanagi::threadmodel::CThreadPool threadPool;
		threadPool.Init(&allocator, 1);

        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            Data[i] = i;
        }

        for (IZ_UINT i = 0; i < COUNTOF(tasks); i++) {
			threadPool.EneueueTask(&tasks[i]);
        }

        for (IZ_UINT i = 0; i < COUNTOF(tasks); i++) {
            tasks[i].Wait();
        }
    
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            if (Data[i] != i - 1) {
                IZ_PRINTF("Failed Sample2 !!!\n");
                IZ_ASSERT(IZ_FALSE);
            }
        }

		threadPool.Terminate();
    }

    // Sample 3
    // Cancel task.
    {
		izanagi::threadmodel::CThreadPool threadPool;
		threadPool.Init(&allocator, 4);

        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            Data[i] = i;
        }

        for (IZ_UINT i = 0; i < COUNTOF(tasks); i++) {
			threadPool.EneueueTask(&tasks[i]);
        }

		tasks[COUNTOF(tasks) - 1].Cancel();

        for (IZ_UINT i = 0; i < COUNTOF(tasks); i++) {
            tasks[i].Wait();
        }
    
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            if (75 <= i && i < 100) {
                if (Data[i] != i) {
                    IZ_PRINTF("Failed Sample3 !!!\n");
                    IZ_ASSERT(IZ_FALSE);
                }
            }
            else if (Data[i] != i - 1) {
                IZ_PRINTF("Failed Sample3 !!!\n");
                IZ_ASSERT(IZ_FALSE);
            }
        }

		threadPool.Terminate();
    }

    allocator.Dump();

    return 0;
}
