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

class CSample
{
public:
    void OnEvent(IZ_UINT x)
    {
        IZ_PRINTF("CSample::OnEvent [%d]\n", x);
    }

    IZ_BOOL OnEventEx(IZ_UINT x)
    {
        IZ_PRINTF("CSample::OnEventEx [%d]\n", x);
        return IZ_TRUE;
    }
};

void DoEvent(izanagi::IMemoryAllocator* allocator)
{
    CSample sample;

    izanagi::CStdEvent<void, IZ_UINT> event1;
    event1.Init(allocator, IZ_NULL);
    event1 += izanagi::EventHandler(allocator, OnEvent);
    event1 += izanagi::EventHandler(allocator, &sample, &CSample::OnEvent);
    event1(1);

    izanagi::CStdEvent<IZ_BOOL, IZ_UINT> event2;
    event2.Init(allocator, IZ_NULL);
    event2 += izanagi::EventHandler(allocator, OnEventEx);
    event2 += izanagi::EventHandler(allocator, &sample, &CSample::OnEventEx);
    event2(1);
}

IZ_UINT8 buffer[1 * 1024 * 1024];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    izanagi::CStandardMemoryAllocator allocator;
    allocator.Init(sizeof(buffer), buffer);

    DoEvent(&allocator);

    allocator.Dump();

    return 0;
}
