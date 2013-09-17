#if !defined(__IZANAGI_RESOURCE_ARCHIVE_INTERFACE_H__)
#define __IZANAGI_RESOURCE_ARCHIVE_INTERFACE_H__

#include "izStd.h"
#include "resource/archive/ARCFormat.h"

namespace izanagi
{
    class IInputStream;

namespace resource
{
    /**
     */
    struct SArchiveFileDesc {
        IZ_PCSTR name;              // file name

        IZ_UINT size;
        IZ_UINT sizeCompressed;

        struct {
            IZ_UINT isCompressed    : 1;    // whether file is compressed
            IZ_UINT isEncrypted     : 1;    // whether file is encrypted
        };

        IZ_UINT filePos;

        IZ_UINT key;
    };

    /**
     */
    class IArchive : public CObject {
    protected:
        IArchive() {}
        virtual ~IArchive() {}

        NO_COPIABLE(IArchive);

    public:
        /** Get InputStream.
         */
        PURE_VIRTUAL(IInputStream* GetInputStream());

        /** Get num of files in ARC.
         */
        PURE_VIRTUAL(IZ_UINT GetFileNum() const);

        /** Get max file's size in ARC.
         */
        PURE_VIRTUAL(IZ_UINT GetMaxFileSize() const);

        /** Seek file by key.
         */
        PURE_VIRTUAL(
            IZ_BOOL SeekByKey(
                IZ_UINT key,
                SArchiveFileDesc* desc));

        /** Seek file by path.
         */
        PURE_VIRTUAL(
            IZ_BOOL SeekByPath(
                IZ_PCSTR path,
                SArchiveFileDesc* desc));

        /** Read data.
         */
        PURE_VIRTUAL(
            IZ_BOOL Read(
                void* dst,
                const SArchiveFileDesc& desc));
    };
}   // namespace resource
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_RESOURCE_ARCHIVE_INTERFACE_H__)
