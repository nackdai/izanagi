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

    void ArchiveLibProxy::BeginExport(int fileNum, int maxFileSize)
    {
        ::BeginExport(fileNum, maxFileSize);
    }

    void ArchiveLibProxy::EndExport()
    {
        ::EndExport();
    }

    void ArchiveLibProxy::ExportFile(char* name, int fileSize)
    {
        ::ExportFile(name, fileSize);
    }
}
}