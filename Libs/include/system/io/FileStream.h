#if !defined(__IZANAGI_IO_FILE_STREAM_H__)
#define __IZANAGI_IO_FILE_STREAM_H__

#include "izDefs.h"

namespace izanagi
{
    /**
    * 超簡易ファイル入力
    */
    class CFileStream
    {
    protected:
        CFileStream()
        {
            m_File = IZ_NULL;
        }
        ~CFileStream() {}

        NO_COPIABLE(CFileStream);

    protected:
        void OnOpen(const char* path, const char* mode);
        void OnClose();

    protected:
        FILE* m_File;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_IO_FILE_STREAM_H__)
