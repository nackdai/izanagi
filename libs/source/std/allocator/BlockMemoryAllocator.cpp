#include "std/allocator/BlockMemoryAllocator.h"
#include "std/StdUtil.h"
#include "math/MathUtil.h"

namespace izanagi {

    const IZ_UINT CBlockMemoryAllocator::BlockSizes[CBlockMemoryAllocator::BlockNum] = 
    {
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

    CBlockMemoryAllocator::CBlockMemoryAllocator()
    {
    }

    CBlockMemoryAllocator::CBlockMemoryAllocator(IZ_UINT nBufSize, void* pBuf)
    {
        IZ_ASSERT(Init(nBufSize, pBuf));
    }

    CBlockMemoryAllocator::~CBlockMemoryAllocator()
    {
    }

    /**
    * Initialize memory allocator.
    */
    IZ_BOOL CBlockMemoryAllocator::Init(IZ_UINT nBufSize, void* pBuf)
    {
        // とりあえず等分して使う
        IZ_UINT8* buf = (IZ_UINT8*)pBuf;
        IZ_UINT size = nBufSize / (BlockNum + 1);

        VRETURN(m_Allocators[0].Init(size, buf, Minimum));
        buf += size;
        nBufSize -= size;

        for (IZ_UINT i = 1; i < BlockNum; i++) {
            VRETURN(m_Allocators[i].Init(size, buf, Align));
            buf += size;
            nBufSize -= size;
        }

        VRETURN(CStandardMemoryAllocator::Init(nBufSize, buf));

        return IZ_TRUE;
    }

    namespace {
        // アラインサイズ計算
        template <typename _T>
        inline _T _ComputeAlignValue(_T v, IZ_UINT align)
        {
            _T ret = (v + (align - 1)) / align * align;
            return ret;
        }
    }   // namespace

    /**
    * Allocate memory buffer.
    */
    void* CBlockMemoryAllocator::Alloc(size_t size, const IZ_CHAR* file, IZ_UINT line)
    {
        size = (size <= Minimum ? Minimum : _ComputeAlignValue(size, Align));

        if (size > BlockSizes[BlockMax]) {
            return CStandardMemoryAllocator::AllocInternal(size, file, line);
        }

        IZ_UINT idx = size / Align;

        void* ret = m_Allocators[idx].Alloc(size, file, line);
        return ret;
    }

    /**
    * Allocate memory buffer (with filling zero).
    */
    void* CBlockMemoryAllocator::AllocZero(size_t size, const IZ_CHAR* file, IZ_UINT line)
    {
        void* ret = Alloc(size, file, line);
        if (ret != IZ_NULL) {
            FILL_ZERO(ret, size);
        }
        return ret;
    }

    /**
    * Re-allocate memory buffer.
    */
    void* CBlockMemoryAllocator::Realloc(void* ptr, size_t size, const IZ_CHAR* file, IZ_UINT line)
    {
        VRETURN_NULL(Free(ptr));

        // Re-alloc
        void* ret = Alloc(size, file, line);
        return ret;
    }

    IZ_BOOL CBlockMemoryAllocator::Free(void* data)
    {
        // Free
        // ptrが所属しているAllocatorを探さないといけない
        // とりあず全検索する・・・

        IZ_BOOL result = IZ_FALSE;

        for (IZ_UINT i = 0; i < BlockNum; i++) {
            result = m_Allocators[i].Free(data);
            if (result) {
                break;
            }
        }

        if (!result) {
            result = CStandardMemoryAllocator::Free(data);
        }

        return result;
    }

    IZ_UINT CBlockMemoryAllocator::GetSize()
    {
        IZ_UINT ret = 0;

        for (IZ_UINT i = 0; i < BlockNum; i++) {
            ret += m_Allocators[i].GetSize();
        }

        ret += CStandardMemoryAllocator::GetSize();

        return ret;
    }

    IZ_UINT CBlockMemoryAllocator::GetAllocatedSize()
    {
        IZ_UINT ret = 0;

        for (IZ_UINT i = 0; i < BlockNum; i++) {
            ret += m_Allocators[i].GetAllocatedSize();
        }

        ret += CStandardMemoryAllocator::GetAllocatedSize();

        return ret;
    }

    IZ_UINT CBlockMemoryAllocator::GetFreedSize()
    {
        IZ_UINT ret = 0;

        for (IZ_UINT i = 0; i < BlockNum; i++) {
            ret += m_Allocators[i].GetFreedSize();
        }

        ret += CStandardMemoryAllocator::GetFreedSize();

        return ret;
    }

    IZ_BOOL CBlockMemoryAllocator::Dump()
    {
        IZ_PRINTF("CBlockMemoryAllocator Dump =====\n");

        for (IZ_UINT i = 0; i < BlockNum; i++) {
            IZ_PRINTF("Idx(%d) %d[bytes] - ", i, BlockSizes[i]);
            m_Allocators[i].Dump();
        }

        CStandardMemoryAllocator::Dump();

        IZ_PRINTF("================================\n");

        return IZ_TRUE;
    }
}
