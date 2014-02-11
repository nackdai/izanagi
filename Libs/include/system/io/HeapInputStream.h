#if !defined(__IZANAGI_IO_HEAP_INPUT_STREAM_H__)
#define __IZANAGI_IO_HEAP_INPUT_STREAM_H__

#include "InputStream.h"
#include "BufferInputStream.h"
#include "std/allocator/MemoryAllocator.h"

namespace izanagi {
    /**
    * 超簡易メモリ入力ストリーム
    * （ただし、アロケータからメモリ確保）
    */
    class CHeapInputStream : public CBufferInputStream {
    public:
        inline CHeapInputStream();
        inline ~CHeapInputStream();

    public:
        // 初期化
        inline IZ_BOOL Init(IMemoryAllocator* pAllocator, IZ_UINT size);

        // 終了処理
        inline void Finalize();

    protected:
        IMemoryAllocator* m_Allocator;
    };

    // inline ********************************

    // コンストラクタ
    CHeapInputStream::CHeapInputStream()
    {
        m_Allocator = IZ_NULL;
    }

    // デストラクタ
    CHeapInputStream::~CHeapInputStream()
    {
        Finalize();
    }

    /**
    * 初期化
    */
    IZ_BOOL CHeapInputStream::Init(IMemoryAllocator* pAllocator, IZ_UINT size)
    {
        if ((m_Allocator != IZ_NULL) && (m_pBuffer != IZ_NULL)) {
            FREE(m_Allocator, const_cast<IZ_UINT8*>(m_pBuffer));
            m_pBuffer = IZ_NULL;
        }

        m_Allocator = pAllocator;

        // メモリ確保
        m_pBuffer = static_cast<IZ_UINT8*>(ALLOC_ZERO(m_Allocator, size));
        VRETURN(m_pBuffer != IZ_NULL);

        m_nSize = size;
        m_nPos = 0;

        return IZ_TRUE;
    }

    /**
    * 終了処理
    */
    void CHeapInputStream::Finalize()
    {
        if ((m_Allocator != IZ_NULL) && (m_pBuffer != IZ_NULL)) {
            FREE(m_Allocator, const_cast<IZ_UINT8*>(m_pBuffer));
        }
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IO_HEAP_INPUT_STREAM_H__)
