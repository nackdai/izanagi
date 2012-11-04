#include <windows.h>

#include "test0.h"
#include "NumericalAnalysisTest.h"

extern void HalfFloatTest();

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#if 0
	test0();
	test1();

	NumericalAnalysisTest();
#endif

    HalfFloatTest();

	return 0;
}
