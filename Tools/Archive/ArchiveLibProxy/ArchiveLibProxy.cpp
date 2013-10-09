// これは メイン DLL ファイルです。

#include "stdafx.h"

#include "ArchiveLibProxy.h"
#include "ArchiveLib.h"

namespace izanagi {
namespace tool {
    ArchiveLibProxy::ArchiveLibProxy()
    {
    }

    ArchiveLibProxy::~ArchiveLibProxy()
    {
    }

    ArchiveLibProxy::!ArchiveLibProxy()
    {
    }

    void ArchiveLibProxy::BeginExport(char* path, int fileNum, int maxFileSize)
    {
        ::BeginExport(path, fileNum, maxFileSize);
    }

    void ArchiveLibProxy::EndExport()
    {
        ::EndExport();
    }

    void ArchiveLibProxy::Register(char* name, char* path)
    {
        ::Register(name, path);
    }
}
}