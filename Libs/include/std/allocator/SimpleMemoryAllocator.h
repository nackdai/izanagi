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
        virtual void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);
        virtual void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        // メモリ確保(アライン有り)
        virtual void* AllocAlign(size_t size, IZ_UINT align, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);
        virtual void* AllocAlignZero(size_t size, IZ_UINT align, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        virtual void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        // メモリ解放
        virtual IZ_BOOL Free(void* data);
        
        virtual IZ_UINT GetSize() { return 0; }
        virtual IZ_UINT GetAllocatedSize() { return 0; }
        virtual IZ_UINT GetFreedSize() { return 0; }

        // ダンプ
        virtual IZ_BOOL Dump() { return IZ_TRUE; }
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_SIMPLE_MEMORY_ALLOCATOR_H__)
