#if !defined(__IZANAGI_IO_BUFFER_INPUT_STREAM_H__)
#define __IZANAGI_IO_BUFFER_INPUT_STREAM_H__

#include "InputStream.h"

namespace izanagi {
    /**
    */
    class CBufferInputStream : public IInputStream {
    protected:
        inline CBufferInputStream();
        virtual ~CBufferInputStream() {}

    public:
        // 読み込み
        inline IZ_UINT Read(void* buf, IZ_INT offset, size_t size);

        // サイズ取得
        inline IZ_LONG GetSize();

        // 現在位置取得
        inline IZ_LONG GetCurPos();

        // シーク
        inline IZ_BOOL Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos);

        // バッファ取得
        inline const void* GetBuffer();
        inline const void* GetBufferCurPos();

    protected:
        const IZ_UINT8* m_pBuffer;

        IZ_LONG m_nSize;
        IZ_LONG m_nPos;
    };

    // inline ********************************

    // コンストラクタ
    CBufferInputStream::CBufferInputStream()
    {
        m_pBuffer = IZ_NULL;
        m_nSize = 0;
        m_nPos = 0;
    }

    /**
    * 読み込み
    */
    IZ_UINT CBufferInputStream::Read(void* buf, IZ_INT offset, size_t size)
    {
        IZ_ASSERT(m_pBuffer != IZ_NULL);

        if (offset > 0) {
            Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
        }

        IZ_LONG ret = 0;

        if (m_nPos < m_nSize) {
            ret = (IZ_INT)size;
            if (m_nPos + ret > m_nSize) {
                ret = m_nSize - m_nPos;
            }

            IZ_ASSERT(IS_IN_BOUND(ret, 0, IZ_UINT32_MAX));
            memcpy(buf, m_pBuffer + m_nPos, static_cast<size_t>(ret));

            m_nPos += (IZ_UINT)size;
            m_nPos = IZ_MIN(m_nPos, m_nSize);
        }

        return (IZ_UINT)ret;
    }

    /**
    * サイズ取得
    */
    IZ_LONG CBufferInputStream::GetSize()
    {
        return m_nSize;
    }

    /**
    * 現在位置取得
    */
    IZ_LONG CBufferInputStream::GetCurPos()
    {
        return m_nPos;
    }

    /**
    * シーク
    */
    IZ_BOOL CBufferInputStream::Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
    {
        IZ_LONG offset = static_cast<IZ_LONG>(nOffset);

        if (nOffset != 0) {
            switch (nSeekPos) {
            case E_IO_STREAM_SEEK_POS_START:
                offset = CLAMP(offset, 0, m_nSize);
                m_nPos = nOffset;
                break;
            case E_IO_STREAM_SEEK_POS_CUR:
                offset = CLAMP(offset, -m_nPos, m_nSize - m_nPos);
                m_nPos += nOffset;
                break;
            case E_IO_STREAM_SEEK_POS_END:
                offset = CLAMP(offset, -m_nSize, 0);
                m_nPos = m_nSize + offset;
                break;
            }
        }

        return IZ_TRUE;
    }

    /**
    * バッファ取得
    */
    const void* CBufferInputStream::GetBuffer()
    {
        return m_pBuffer;
    }

    /**
    * バッファ取得
    */
    const void* CBufferInputStream::GetBufferCurPos()
    {
        IZ_ASSERT(m_nPos < m_nSize);
        IZ_ASSERT(m_pBuffer != IZ_NULL);
        return (m_pBuffer + m_nPos);
    }
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IO_BUFFER_INPUT_STREAM_H__)
