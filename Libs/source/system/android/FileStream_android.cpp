#include <android/asset_manager.h>
#include "system/io/FileStream.h"

extern AAssetManager* GetAssetManager();

namespace izanagi
{
    // ファイルオープン
    void CFileStream::OnOpen(IZ_FILE_HANDLE* file, const char* path, const char* mode)
    {
        AAssetManager* mgr = GetAssetManager();
        IZ_ASSERT(mgr != IZ_NULL);

        *file = AAssetManager_open(mgr, path, AASSET_MODE_RANDOM);
    }

    void CFileStream::OnClose(IZ_FILE_HANDLE file)
    {
        if (file != IZ_NULL) {
            AAsset_close(file);
        }
    }

    IZ_INT CFileStream::OnSeek(IZ_FILE_HANDLE file, IZ_INT offset, IZ_INT origin)
    {
        off_t pos = AAsset_seek(file, offset, origin);
        return (pos >= 0 ? 0 : -1);
    }

    IZ_LONG CFileStream::GetLength(IZ_FILE_HANDLE file)
    {
        // ファイルサイズ取得
        return (IZ_LONG)AAsset_getLength(file);
    }

    IZ_INT CFileStream::OnRead(void* buf, IZ_UINT size, IZ_UINT count, IZ_FILE_HANDLE file)
    {
        return AAsset_read(file, buf, size * count);
    }

    IZ_UINT CFileStream::Tell(IZ_FILE_HANDLE file)
    {
        IZ_ASSERT(IZ_FALSE);
        return 0;
    }

    IZ_UINT CFileStream::OnWrite(const void* src, size_t size, size_t count, IZ_FILE_HANDLE file)
    {
        IZ_ASSERT(IZ_FALSE);
        return 0;
    }

    IZ_INT CFileStream::Print(IZ_FILE_HANDLE file, const char* str)
    {
        IZ_ASSERT(IZ_FALSE);
        return -1;
    }
}
