#include <windows.h>

#include "test0.h"
#include "NumericalAnalysisTest.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	test0();
	test1();

	NumericalAnalysisTest();

	return 0;
}
