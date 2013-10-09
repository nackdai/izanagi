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
        void BeginExport(char* path, int fileNum, int maxFileSize);
        void EndExport();

        void Register(char* name, char* path);
	};
}
}
