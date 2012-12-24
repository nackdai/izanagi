#if !defined(__IZANAGI_TOOL_KIT_STD_SIMPLE_MEMORY_ALLOCATOR_H__)
#define __IZANAGI_TOOL_KIT_STD_SIMPLE_MEMORY_ALLOCATOR_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi {
namespace tool {
    /**
    * Tool用シンプルメモリアロケータ
    */
    class CSimpleMemoryAllocator : public CDebugMemoryAllocator {
    private:
        // ヒープヘッダ
        struct SHeapHeader : public CPlacementNew {
            IZ_UINT8* buf;
            size_t size;
            
            CStdList<SHeapHeader>::Item item;

            IZ_UINT line;
            const IZ_CHAR* file;

            void Init()
            {
                buf = IZ_NULL;
                size = 0;
                
                item.Init(this);

                line = 0;
                file = IZ_NULL;
            }

            void SetDebugInfo(IZ_UINT line, const IZ_CHAR* file)
            {
                this->line = line;
                this->file = file;
            }

            IZ_UINT8* GetTopAddr() const
            {
                return (IZ_UINT8*)this;
            }

            IZ_UINT8* GetTailAddr() const
            {
                return buf + size;
            }

            size_t GetAllSize() const
            {
                return size + sizeof(SHeapHeader);
            }
        };

    public:
        CSimpleMemoryAllocator() {}
        ~CSimpleMemoryAllocator() {}

        NO_COPIABLE(CSimpleMemoryAllocator);

    public:
        void* Alloc(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);
        void* AllocZero(size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);

        void* Realloc(void* ptr, size_t size, const IZ_CHAR* file = IZ_NULL, IZ_UINT line = IZ_NULL);

        IZ_BOOL Free(void* data);

        IZ_UINT GetSize();
        IZ_UINT GetAllocatedSize();
        IZ_UINT GetFreedSize();

        IZ_BOOL Dump();

    private:
        SHeapHeader* AddHeader(IZ_BYTE* ptr, size_t size, const IZ_CHAR* file, IZ_UINT line);
        SHeapHeader* FindHeader(void* ptr);

    private:
        CStdList<SHeapHeader> m_AllocList;
    };
}   // namespace tool
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TOOL_KIT_STD_SIMPLE_MEMORY_ALLOCATOR_H__)
