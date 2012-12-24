#include "izSampleKit.h"
#include "izSystem.h"

extern int TestJobQueue(izanagi::IMemoryAllocator* allocator);
extern int TestMessageQueue(izanagi::IMemoryAllocator* allocator);

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    izanagi::CStandardMemoryAllocator allocator(BUF_SIZE, BUF);

    //TestJobQueue(&allocator);
    TestMessageQueue(&allocator);

    return 0;
}
