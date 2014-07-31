#if !defined(__IZANAGI_STD_STANDARD_MEMORY_ALLOCATOR_H__)
#define __IZANAGI_STD_STANDARD_MEMORY_ALLOCATOR_H__

#include "izDefs.h"
#include "std/collection/StdSimpleList.h"
#include "MemoryAllocator.h"

namespace izanagi {
    /** 標準メモリアロケータ.
     */
    class CStandardMemoryAllocator
        : public IZ_MEMORY_ALLOCATOR_BASE
    {
    protected:
        struct SHeapHeader
            : public IZ_DEBUG_HEAP_HEADER_BASE
        {
            IZ_UINT8* buf;
            IZ_UINT size;
            SHeapHeader* prev;
            SHeapHeader* next;

            void Init()
            {
                buf = IZ_NULL;
                size = 0;
                prev = IZ_NULL;
                next = IZ_NULL;

                ClearDebugInfo();
            }

            IZ_UINT8* GetTopAddr() const
            {
                return (IZ_UINT8*)this;
            }

            IZ_UINT8* GetTailAddr() const
            {
                return buf + size;
            }

            IZ_UINT GetAllSize() const
            {
                IZ_ASSERT(buf > reinterpret_cast<IZ_UINT8*>(const_cast<SHeapHeader*>(this)));
                IZ_UINT sub = (IZ_UINT)(buf - reinterpret_cast<IZ_UINT8*>(const_cast<SHeapHeader*>(this)));
                return size + sub;
            }
        };

    public:
        CStandardMemoryAllocator();
        CStandardMemoryAllocator(IZ_UINT nBufSize, void* pBuf);

        ~CStandardMemoryAllocator();

        NO_COPIABLE(CStandardMemoryAllocator);

    public:
        // 初期化
        IZ_BOOL Init(IZ_UINT nBufSize, void* pBuf);

        // メモリ確保
        virtual void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);
        virtual void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        // メモリ確保(アライン有り)
        virtual void* AllocAlign(size_t size, IZ_UINT align, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);
        virtual void* AllocAlignZero(size_t size, IZ_UINT align, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        virtual void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = 0);

        // メモリ解放
        virtual IZ_BOOL Free(void* data);
        
        virtual IZ_UINT GetSize();
        virtual IZ_UINT GetAllocatedSize();
        virtual IZ_UINT GetFreedSize();

        // ダンプ
        virtual IZ_BOOL Dump();

    protected:
        // メモリ確保
        void* AllocInternal(
            size_t size,
            const IZ_CHAR* file,
            IZ_UINT line);

        // メモリ確保(アライン有り)
        void* AllocInternal(
            size_t size,
            IZ_UINT align,
            const IZ_CHAR* file,
            IZ_UINT line);

        // フリーリストに挿入
        void InsertFreeList(SHeapHeader* pFree);

        // Realloc可能なフリーのメモリ領域を探す
        SHeapHeader* FindReallocableFreedHeapHeader(void* ptr, size_t size);

    protected:
        // SHeapHeader探索
        inline SHeapHeader* FindAllocatedHeapHeader(void* ptr);

        // SHeapHeader確保
        inline SHeapHeader* GetHeapHeader(IZ_UINT8* buf);

    protected:
        IZ_UINT m_nBufSize;
        IZ_UINT8* m_pBuf;

        CStdSimpleList<SHeapHeader> m_FreeList;
        CStdSimpleList<SHeapHeader> m_AllocList;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STANDARD_MEMORY_ALLOCATOR_H__)
