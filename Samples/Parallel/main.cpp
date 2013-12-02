#include "izSampleKit.h"
#include "izThreadModel.h"

static IZ_INT Data[100];

static void ParallelForFunc(IZ_INT idx)
{
    IZ_PRINTF("For Func : %d\n", idx);
    Data[idx] -= 1;
}

static void ParallelForEachFunc(void* data)
{
    IZ_UINT value = *(IZ_UINT*)data;
    IZ_PRINTF("ForEach Func : %d\n", value);
    Data[value] -= 1;
}

class CForFunctor : public izanagi::threadmodel::CParallel::CFuncFor
{
public:
    virtual void operator()(IZ_INT idx)
    {
        IZ_PRINTF("For Functor : %d\n", idx);
        Data[idx] -= 1;
    }
};

class CForEachFunctor : public izanagi::threadmodel::CParallel::CFuncForEach
{
public:
    virtual void operator()(void* data)
    {
        IZ_UINT value = *(IZ_UINT*)data;
        IZ_PRINTF("ForEach Functor : %d\n", value);
        Data[value] -= 1;
    }
};

IZ_UINT8 buffer[1 * 1024 * 1024];

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    izanagi::CStandardMemoryAllocator allocator;
    allocator.Init(sizeof(buffer), buffer);

    izanagi::threadmodel::CThreadPool::Init(&allocator, 4);

    {
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            Data[i] = i;
        }

        izanagi::threadmodel::CParallel::For(
            &allocator,
            0, 100,
            ParallelForFunc);
    
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            if (Data[i] != i - 1) {
                IZ_PRINTF("Failed Parallel For Func!!!\n");
                IZ_ASSERT(IZ_FALSE);
            }
        }
    }

    {
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            Data[i] = i;
        }

        izanagi::threadmodel::CParallel::ForEach(
            &allocator,
            Data, sizeof(IZ_UINT),
            COUNTOF(Data),
            ParallelForEachFunc);
    
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            if (Data[i] != i - 1) {
                IZ_PRINTF("Failed Parallel ForEach Func!!!\n");
                IZ_ASSERT(IZ_FALSE);
            }
        }
    }

    {
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            Data[i] = i;
        }

        izanagi::threadmodel::CParallel::For(
            &allocator,
            0, 100,
            CForFunctor());
    
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            if (Data[i] != i - 1) {
                IZ_PRINTF("Failed Parallel For Functor!!!\n");
                IZ_ASSERT(IZ_FALSE);
            }
        }
    }

    {
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            Data[i] = i;
        }

        izanagi::threadmodel::CParallel::ForEach(
            &allocator,
            Data, sizeof(IZ_UINT),
            COUNTOF(Data),
            CForEachFunctor());
    
        for (IZ_UINT i = 0; i < COUNTOF(Data); i++) {
            if (Data[i] != i - 1) {
                IZ_PRINTF("Failed Parallel ForEach Functor!!!\n");
                IZ_ASSERT(IZ_FALSE);
            }
        }
    }

    izanagi::threadmodel::CThreadPool::Terminate();

    allocator.Dump();

    return 0;
}
