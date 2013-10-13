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
        /** Archive�o�͏���
         *
         * @param path[in] Archive�o�̓t�@�C���p�X
         * @param fileNum[in] �p�b�N����t�@�C����
         * @param maxFileSize[in] �p�b�N����t�@�C���̍ő�o�C�g��
         */
        static void BeginExport(char* path, int fileNum, int maxFileSize);

        /** Archive�o�͏I��
         */
        static void EndExport();

        /** Archive�Ɋ܂߂�t�@�C����o�^
         *
         * @param name[in] �t�@�C����
         * @param path[in] �t�@�C���p�X
         */
        static void Register(char* name, char* path);
	};
}
}
