#include <android/asset_manager.h>
#include "system/io/FileStream.h"

extern AAssetManager* GetAssetManager();

namespace izanagi
{
    // ファイルオープン
    void CFileStream::OnOpen(FILE_HANDLE* file, const char* path, const char* mode)
    {
        AAssetManager* mgr = GetAssetManager();
        IZ_ASSERT(mgr != IZ_NULL);

        *file = AAssetManager_open(mgr, path, AASSET_MODE_RANDOM);
    }

    void CFileStream::OnClose(FILE_HANDLE file)
    {
        if (file != IZ_NULL) {
            AAsset_close(file);
        }
    }

    IZ_INT CFileStream::OnSeek(FILE_HANDLE file, IZ_INT offset, IZ_INT origin)
    {
        return AAsset_seek(file, offset, origin);
    }

    IZ_LONG CFileStream::GetLength(FILE_HANDLE file)
    {
        // ファイルサイズ取得
        return (IZ_LONG)AAsset_getLength(file);
    }

    IZ_INT CFileStream::OnRead(void* buf, IZ_UINT size, IZ_UINT count, FILE_HANDLE file)
    {
        return AAsset_read(file, buf, size * count);
    }

    IZ_UINT CFileStream::Tell(FILE_HANDLE file)
    {
        IZ_ASSERT(IZ_FALSE);
        return 0;
    }

    IZ_UINT CFileStream::OnWrite(const void* src, size_t size, size_t count, FILE_HANDLE file)
    {
        IZ_ASSERT(IZ_FALSE);
        return 0;
    }

    IZ_INT CFileStream::Print(FILE_HANDLE file, const char* str)
    {
        IZ_ASSERT(IZ_FALSE);
        return -1;
    }
}
