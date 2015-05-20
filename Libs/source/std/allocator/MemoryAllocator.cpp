#include "std/allocator/MemoryAllocator.h"

namespace izanagi {
    CMemoryAllocator::FuncAlloc CMemoryAllocator::s_Alloc = IZ_NULL;
    CMemoryAllocator::FuncRealloc CMemoryAllocator::s_Realloc = IZ_NULL;
    CMemoryAllocator::FuncFree CMemoryAllocator::s_Free = IZ_NULL;

    void* CMemoryAllocator::Alloc(size_t size)
    {
        void* ret = IZ_NULL;

        if (s_Alloc) {
            ret = (*s_Alloc)(size);
        }
        else {
            ret = malloc(size);
        }

        if (ret != IZ_NULL) {
            FILL_ZERO(ret, size);
        }

        return ret;
    }

    void* CMemoryAllocator::Realloc(void* ptr, size_t size)
    {
        void* ret = IZ_NULL;

        if (s_Realloc) {
            ret = (*s_Realloc)(ptr, size);
        }
        else {
            ret = realloc(ptr, size);
        }

        return ret;
    }

    void CMemoryAllocator::Free(void* ptr)
    {
        if (s_Alloc) {
            (*s_Free)(ptr);
        }
        else {
            free(ptr);
        }
    }
    
    void CMemoryAllocator::SetAlloc(CMemoryAllocator::FuncAlloc func)
    {
        s_Alloc = func;
    }

    void CMemoryAllocator::SetRealloc(CMemoryAllocator::FuncRealloc func)
    {
        s_Realloc = func;
    }

    void CMemoryAllocator::SetFree(CMemoryAllocator::FuncFree func)
    {
        s_Free = func;
    }
}   // namespace izanagi
