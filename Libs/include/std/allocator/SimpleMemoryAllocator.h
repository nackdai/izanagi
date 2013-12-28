#if !defined(__IZANAGI_STD_SIMPLE_MEMORY_ALLOCATOR_H__)
#define __IZANAGI_STD_SIMPLE_MEMORY_ALLOCATOR_H__

#include "izDefs.h"
#include "MemoryAllocator.h"

namespace izanagi {
    /** シンプルメモリアロケータ.
     */
    class CSimpleMemoryAllocator : public IMemoryAllocator
    {
    public:
        CSimpleMemoryAllocator() {}
        ~CSimpleMemoryAllocator() {}

        NO_COPIABLE(CSimpleMemoryAllocator);

    public:
        // メモリ確保
        void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);
        void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        // メモリ確保(アライン有り)
        void* AllocAlign(size_t size, IZ_UINT align, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);
        void* AllocAlignZero(size_t size, IZ_UINT align, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        // メモリ解放
        IZ_BOOL Free(void* data);
        
        IZ_UINT GetSize() { return 0; }
        IZ_UINT GetAllocatedSize() { return 0; }
        IZ_UINT GetFreedSize() { return 0; }

        // ダンプ
        IZ_BOOL Dump() { return IZ_TRUE; }
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_SIMPLE_MEMORY_ALLOCATOR_H__)
