#if !defined(__IZANAGI_ARCHIVE_ARCHIVE_DUMMY_H__)
#define __IZANAGI_ARCHIVE_ARCHIVE_DUMMY_H__

#include "Archive.h"
#include "io/FileInputStream.h"

namespace izanagi {
    /**
    * ダミーアーカイブ
    */
    class CArchiveDummy : public IArchive {
    public:
        CArchiveDummy() {}
        ~CArchiveDummy() { m_Input.Finalize(); }

        NO_COPIABLE(CArchiveDummy);

    public:
        // Get InputStream.
        IInputStream* GetInputStream();

        // Get num of files in ARC.
        IZ_UINT GetFileNum() const { return 0; }

        // Get max file's size in ARC.
        IZ_UINT GetMaxFileSize() const { return 0; }

        // Seek file by key.
        IZ_BOOL Seek(IZ_UINT nKey) { return IZ_TRUE; }

        // Seek file by path.
        IZ_BOOL Seek(IZ_PCSTR path);

        // Return current file's description.
        IZ_BOOL GetFileDesc(SArchiveFileDesc* pDesc) { return IZ_TRUE; }

    protected:
        CFileInputStream m_Input;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ARCHIVE_ARCHIVE_DUMMY_H__)
