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
        /** Archive出力準備
         *
         * @param path[in] Archive出力ファイルパス
         * @param fileNum[in] パックするファイル数
         * @param maxFileSize[in] パックするファイルの最大バイト数
         */
        static void BeginExport(char* path, int fileNum, int maxFileSize);

        /** Archive出力終了
         */
        static void EndExport();

        /** Archiveに含めるファイルを登録
         *
         * @param name[in] ファイル名
         * @param path[in] ファイルパス
         */
        static void Register(char* name, char* path);
	};
}
}
