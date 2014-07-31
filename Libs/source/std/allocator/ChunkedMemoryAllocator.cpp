#include "std/allocator/ChunkedMemoryAllocator.h"
#include "std/StdUtil.h"
#include "math/MathUtil.h"

namespace izanagi {

    CChunkedMemoryAllocator::CChunkedMemoryAllocator()
    {
        m_nBufSize = 0;
        m_pBuf = IZ_NULL;
        m_nChunkSize = 0;
    }

    CChunkedMemoryAllocator::CChunkedMemoryAllocator(IZ_UINT nBufSize, void* pBuf, IZ_UINT nChunkSize)
    {
        IZ_ASSERT(Init(nBufSize, pBuf, nChunkSize));
    }

    CChunkedMemoryAllocator::~CChunkedMemoryAllocator()
    {
    }

    /**
    * Initialize memory allocator.
    */
    IZ_BOOL CChunkedMemoryAllocator::Init(IZ_UINT nBufSize, void* pBuf, IZ_UINT nChunkSize)
    {
        IZ_ASSERT(!m_AllocList.HasItem());
        IZ_ASSERT(nChunkSize > 0);

        m_FreeList.Clear();

        IZ_BOOL ret = IZ_FALSE;

        m_nBufSize = nBufSize;
        m_pBuf = reinterpret_cast<IZ_UINT8*>(pBuf);

        // ChunkSize must be power of 2.
        m_nChunkSize = math::CMath::ComputeNextPow2(nChunkSize);

        // フリーリスト初期化
        SHeapHeader* p = GetHeapHeader(m_pBuf);

        if (p != IZ_NULL) {
            p->buf = m_pBuf + sizeof(SHeapHeader);
            p->size = m_nBufSize - sizeof(SHeapHeader);
            m_FreeList.AddTop(p);

            ret = IZ_TRUE;
        }

        return ret;
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
    void* CChunkedMemoryAllocator::Alloc(size_t size, const IZ_CHAR* file, IZ_UINT line)
    {
        size = _ComputeAlignValue(size, m_nChunkSize);

        void* ret = AllocInternal(size, file, line);
        return ret;
    }

    /**
    * Allocate memory buffer (with filling zero).
    */
    void* CChunkedMemoryAllocator::AllocZero(size_t size, const IZ_CHAR* file, IZ_UINT line)
    {
        size = _ComputeAlignValue(size, m_nChunkSize);

        void* ret = Alloc(size, file, line);
        if (ret != IZ_NULL) {
            FILL_ZERO(ret, size);
        }
        return ret;
    }

    /**
    * Re-allocate memory buffer.
    */
    void* CChunkedMemoryAllocator::Realloc(void* ptr, size_t size, const IZ_CHAR* file, IZ_UINT line)
    {
        size = _ComputeAlignValue(size, m_nChunkSize);

        void* ret = CStandardMemoryAllocator::Realloc(ptr, size, file, line);
        return ret;
    }
}
