#if !defined(__IZANAGI_STD_CHUNKED_MEMORY_ALLOCATOR_H__)
#define __IZANAGI_STD_CHUNKED_MEMORY_ALLOCATOR_H__

#include "StandardMemoryAllocator.h"

namespace izanagi {
    /**
    */
    class CChunkedMemoryAllocator : public CStandardMemoryAllocator
    {
    public:
        CChunkedMemoryAllocator();
        CChunkedMemoryAllocator(IZ_UINT nBufSize, void* pBuf, IZ_UINT nChunkSize);

        ~CChunkedMemoryAllocator();

        NO_COPIABLE(CChunkedMemoryAllocator);

    public:
        // 初期化
        IZ_BOOL Init(IZ_UINT nBufSize, void* pBuf, IZ_UINT nChunkSize);

        // メモリ確保
        virtual void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);
        virtual void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        virtual void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

    private:
        IZ_UINT m_nChunkSize;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_CHUNKED_MEMORY_ALLOCATOR_H__)
