// ArchiveLibProxy.h

#pragma once

using namespace System;

namespace izanagi {
namespace tool {

	public ref class ArchiveLibProxy
	{
    private:
        ArchiveLibProxy();
        ~ArchiveLibProxy();
        !ArchiveLibProxy();

    public:
        void BeginExport(int fileNum, int maxFileSize);
        void EndExport();

        void ExportFile(char* name, int fileSize);
	};
}
}
