#if !defined(__IZANAGI_IO_FILE_OUTPUT_STREAM_H__)
#define __IZANAGI_IO_FILE_OUTPUT_STREAM_H__

#include "OutputStream.h"
#include "FileStream.h"

namespace izanagi
{
    /**
     */
    class CFileOutputStream : public IOutputStream, CFileStream
    {
    public:
        CFileOutputStream()
        {
            m_Pos = 0;
        }
        ~CFileOutputStream()
        {
            Finalize();
        }

        NO_COPIABLE(CFileOutputStream);

    public:
        IZ_BOOL Open(IZ_PCSTR path)
        {
            return Open(path, E_IO_FILE_MODE_BIN);
        }

        IZ_BOOL Open(IZ_PCSTR path, E_IO_FILE_MODE mode)
        {
            IZ_BOOL ret = IZ_FALSE;

            if (m_File != IZ_NULL) {
                IZ_ASSERT(IZ_FALSE);
                Finalize();
            }

            OnOpen(
                path,
                mode == E_IO_FILE_MODE_TEXT ? "wt" : "wb");
            ret = (m_File != IZ_NULL);

            return ret;
        }

        void Finalize()
        {
            OnClose();

            m_Pos = 0;
        }

        // 出力
        IZ_UINT Write(const void* buf, IZ_UINT offset, size_t size)
        {
            IZ_ASSERT(m_File != IZ_NULL);
            IZ_ASSERT(size > 0);

            if (offset > 0) {
                Seek(offset, E_IO_STREAM_SEEK_POS_CUR);
            }

            IZ_UINT ret = (IZ_UINT)fwrite(buf, 1, size, m_File);
            IZ_ASSERT(ret == size);

            if (ret == size) {
                m_Pos += offset + size;
            }

            return ret;
        }

        IZ_BOOL Write(IZ_PCSTR text)
        {
            IZ_ASSERT(m_File != IZ_NULL);

            int result = ::fprintf_s(m_File, "%s", text);
            IZ_ASSERT(result >= 0);

            return (result >= 0);
        }

        // サイズ取得
        IZ_UINT GetSize()
        {
            IZ_ASSERT(m_File != IZ_NULL);

            IZ_UINT ret = ftell(m_File);
            return ret;
        }

        // 現在位置取得
        IZ_UINT GetCurPos()
        {
            return m_Pos;
        }

        // シーク
        IZ_BOOL Seek(IZ_INT offset, E_IO_STREAM_SEEK_POS seekPos)
        {
            IZ_ASSERT(m_File != IZ_NULL);

            IZ_UINT nPos = SEEK_SET;

            switch (seekPos) {
            case E_IO_STREAM_SEEK_POS_START:
                nPos = SEEK_SET;
                break;
            case E_IO_STREAM_SEEK_POS_CUR:
                nPos = SEEK_CUR;
                break;
            case E_IO_STREAM_SEEK_POS_END:
                // 出力時にはファイル終端は存在しないので・・・
                VRETURN(IZ_FALSE);
                break;
            }

            IZ_BOOL ret = IZ_TRUE;
            {
                // シーク
                ret = (fseek(m_File, offset, nPos) == 0);

                if (ret) {
                    // 現在位置更新
                    switch (seekPos) {
                    case E_IO_STREAM_SEEK_POS_START:
                        m_Pos = offset;
                        break;
                    case E_IO_STREAM_SEEK_POS_CUR:
                        m_Pos += offset;
                        break;
                    case E_IO_STREAM_SEEK_POS_END:
                        VRETURN(IZ_FALSE);
                        break;
                    }
                }
            }
            IZ_ASSERT(ret);

            return ret;
        }

        IZ_BOOL IsValid()
        {
            return (m_File != NULL);
        }

        IZ_BOOL IsEnableOutput() { return IsValid(); }

    private:
        IZ_UINT m_Pos;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IO_FILE_OUTPUT_STREAM_H__)
