#include <windows.h>

#include "izStd.h"

static void OnEvent(IZ_UINT x)
{
	IZ_PRINTF("static OnEvent [%d]\n", x);
}

static IZ_BOOL OnEventEx(IZ_UINT x)
{
	IZ_PRINTF("static OnEventEx [%d]\n", x);
	return IZ_TRUE;
}

class Test
{
public:
	void OnEvent(IZ_UINT x)
	{
		IZ_PRINTF("Test::OnEvent [%d]\n", x);
	}

	IZ_BOOL OnEventEx(IZ_UINT x)
	{
		IZ_PRINTF("Test::OnEventEx [%d]\n", x);
		return IZ_TRUE;
	}
};

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	izanagi::CSimpleMemoryAllocator allocator;

	Test test;

	izanagi::CStdEvent<void, IZ_UINT> event1;
	event1.Init(&allocator);
	event1 += izanagi::EventHandler(&allocator, OnEvent);
	event1 += izanagi::EventHandler(&allocator, &test, &Test::OnEvent);
	event1(1);

	izanagi::CStdEvent<IZ_BOOL, IZ_UINT> event2;
	event2.Init(&allocator);
	event2 += izanagi::EventHandler(&allocator, OnEventEx);
	event2 += izanagi::EventHandler(&allocator, &test, &Test::OnEventEx);
	event2(1);

	return 0;
}
