#include "izStd.h"

IZ_UINT8 buffer[4 * 1024 * 1024];

static const IZ_UINT size[] = {
    16,
    32,
    64,
    96,
    128,
    160,
    192,
    224,
    256,
    288,
    320,
    352,
    384,
    416,
    448,
    480,
    512,
};

void Alloc(izanagi::IMemoryAllocator* allocator)
{
    for (IZ_UINT i = 0; i < COUNTOF(size); i++) {
        void* p = ALLOC(allocator, size[i] - 1);

        IZ_UINT allocedSize = allocator->GetAllocatedSize();
        IZ_PRINTF("Allloced Size %d[bytes]\n", allocedSize);

        FREE(allocator, p);

        IZ_UINT freedSize = allocator->GetFreedSize();
        IZ_PRINTF("Freed Size %d[bytes]\n", freedSize);
    }

    allocator->Dump();
}

IzMain(0, 0)
{
    // Standard Allocator
    {
        izanagi::CStandardMemoryAllocator allocator;
        allocator.Init(sizeof(buffer), buffer);
        Alloc(&allocator);
    }

    // Chunked Allocator
    {
        izanagi::CChunkedMemoryAllocator allocator;
        allocator.Init(sizeof(buffer), buffer, 32);
        Alloc(&allocator);
    }

    // Block Allocator
    {
        izanagi::CBlockMemoryAllocator allocator;
        allocator.Init(sizeof(buffer), buffer);
        Alloc(&allocator);
    }

    return 0;
}
