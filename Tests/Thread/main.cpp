#include "izSampleKit.h"
#include "izSystem.h"

extern int TestThread();
extern int TestMutex();
extern int TestSemaphore();
extern int TestEvent();
extern int TestCondVar();
extern int TestMonitor();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if 0
    IZ_PRINTF("TestThread =========\n");
    TestThread();
#endif

#if 0
    IZ_PRINTF("TestMutex =========\n");
    TestMutex();
#endif

#if 0
    IZ_PRINTF("TestSemaphore =========\n");
    TestSemaphore();
#endif

#if 0
    IZ_PRINTF("TestEvent =========\n");
    TestEvent();
#endif

#if 0
    IZ_PRINTF("TestCondVar =========\n");
    TestCondVar();
#endif

#if 1
    IZ_PRINTF("TestMonitor =========\n");
    TestMonitor();
#endif

	return 0;
}
