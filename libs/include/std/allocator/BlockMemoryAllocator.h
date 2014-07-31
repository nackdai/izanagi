#if !defined(__IZANAGI_STD_BLOCK_MEMORY_ALLOCATOR_H__)
#define __IZANAGI_STD_BLOCK_MEMORY_ALLOCATOR_H__

#include "StandardMemoryAllocator.h"
#include "ChunkedMemoryAllocator.h"

namespace izanagi {
    /**
    */
    class CBlockMemoryAllocator : public CStandardMemoryAllocator
    {
    public:
        CBlockMemoryAllocator();
        CBlockMemoryAllocator(IZ_UINT nBufSize, void* pBuf);

        ~CBlockMemoryAllocator();

        NO_COPIABLE(CBlockMemoryAllocator);

    public:
        // 初期化
        IZ_BOOL Init(IZ_UINT nBufSize, void* pBuf);

        // メモリ確保
        virtual void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);
        virtual void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        virtual void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        virtual IZ_BOOL Free(void* data);
        
        virtual IZ_UINT GetSize();
        virtual IZ_UINT GetAllocatedSize();
        virtual IZ_UINT GetFreedSize();

        // ダンプ
        virtual IZ_BOOL Dump();
        
    private:
        enum {
            BlockNum = 17,
            BlockMax = BlockNum - 1,

            Minimum = 16,
            Align = 32,
        };

        static const IZ_UINT BlockSizes[BlockNum];

        CChunkedMemoryAllocator m_Allocators[BlockNum];
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_BLOCK_MEMORY_ALLOCATOR_H__)
