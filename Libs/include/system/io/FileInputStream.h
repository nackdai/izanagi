#if !defined(__IZANAGI_IO_FILE_INPUT_STREAM_H__)
#define __IZANAGI_IO_FILE_INPUT_STREAM_H__

#include "InputStream.h"
#include "FileStream.h"

namespace izanagi
{
    /**
    * 超簡易ファイル入力
    */
    class CFileInputStream : public IInputStream, CFileStream
    {
    public:
        CFileInputStream()
        {
            m_File = IZ_NULL;
            m_Size = 0;
            m_Pos = 0;
        }
        ~CFileInputStream()
        {
            Close();
        }

        NO_COPIABLE(CFileInputStream);

    public:
        // ファイルオープン
        IZ_BOOL Open(IZ_PCSTR path)
        {
            IZ_BOOL ret = IZ_FALSE;

            if (m_File != IZ_NULL) {
                IZ_ASSERT(IZ_FALSE);
                Close();
            }

            OnOpen(&m_File, path, "rb");
            ret = (m_File != IZ_NULL);

            if (ret) {
                // ファイルサイズ取得
                m_Size = GetLength(m_File);
            }

            return ret;
        }

        // ファイルクローズ
        void Close()
        {
            OnClose(m_File);

            m_File = IZ_NULL;

            m_Size = 0;
            m_Pos = 0;
        }

        // 読み込み
        IZ_UINT Read(void* buf, IZ_INT offset, size_t size)
        {
            IZ_ASSERT(m_File != IZ_NULL);

            if (offset > 0) {
                VRETURN(Seek(offset, E_IO_STREAM_SEEK_POS_CUR));
            }
            IZ_UINT ret = (IZ_UINT)OnRead(buf, 1, size, m_File);

            if (ret == size) {
                // TODO
                // ファイル位置更新
                m_Pos = m_Pos + (IZ_INT)size;
            }
            else {
                m_Pos = m_Size;
            }

            return ret;
        }

        // サイズ取得
        IZ_LONG GetSize()
        {
            return m_Size;
        }

        // 現在位置取得
        IZ_LONG GetCurPos()
        {
            return m_Pos;
        }

        // シーク
        IZ_BOOL Seek(IZ_INT nOffset, E_IO_STREAM_SEEK_POS nSeekPos)
        {
            IZ_ASSERT(m_File != IZ_NULL);

            IZ_UINT nPos = SEEK_SET;
            IZ_LONG offset = static_cast<IZ_LONG>(nOffset);

            switch (nSeekPos) {
            case E_IO_STREAM_SEEK_POS_START:
                nPos = SEEK_SET;
                offset = CLAMP(offset, 0, m_Size);
                break;
            case E_IO_STREAM_SEEK_POS_CUR:
                nPos = SEEK_CUR;
                offset = CLAMP(offset, -m_Pos, m_Size - m_Pos);
                break;
            case E_IO_STREAM_SEEK_POS_END:
                nPos = SEEK_END;
                offset = CLAMP(offset, -m_Size, 0);
                break;
            }

            // TODO
            IZ_ASSERT(IS_IN_BOUND(offset, IZ_INT32_MIN, IZ_INT32_MAX));

            IZ_BOOL ret = IZ_TRUE;
            if (nOffset != 0) {
                // シーク
                ret = (OnSeek(m_File, static_cast<IZ_INT>(offset), nPos) == 0);

                if (ret) {
                    // 現在位置更新
                    switch (nSeekPos) {
                    case E_IO_STREAM_SEEK_POS_START:
                        m_Pos = offset;
                        break;
                    case E_IO_STREAM_SEEK_POS_CUR:
                        m_Pos += offset;
                        break;
                    case E_IO_STREAM_SEEK_POS_END:
                        m_Pos = m_Size + offset;
                        break;
                    }
                }
            }
            IZ_ASSERT(ret);

            return ret;
        }

        // 終了処理
        void Finalize()
        {
            Close();
        }

    protected:
        FILE_HANDLE m_File;
        IZ_LONG m_Size;
        IZ_LONG m_Pos;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IO_FILE_INPUT_STREAM_H__)
