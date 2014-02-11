#include "system/io/FileStream.h"

namespace izanagi
{
    // ファイルオープン
    void CFileStream::OnOpen(const char* path, const char* mode)
    {
        fopen_s(&m_File, path, mode);
    }

    void CFileStream::OnClose()
    {
        if (m_File != IZ_NULL) {
            fclose(m_File);
        }

        m_File = IZ_NULL;
    }
}
