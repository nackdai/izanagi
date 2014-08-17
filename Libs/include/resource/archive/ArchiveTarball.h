#if !defined(__IZANAGI_RESOURCE_ARCHIVE_TARBALL_H__)
#define __IZANAGI_RESOURCE_ARCHIVE_TARBALL_H__

#include "izStd.h"
#include "resource/archive/ArchiveInterface.h"

namespace izanagi
{
    class IInputStream;

namespace resource
{
    // http://monoist.atmarkit.co.jp/mn/articles/1004/05/news082_2.html

    class CArchiveTar : public IArchive {
    public:
        /** インスタンス作成
         */
        static IArchive* CreateArchive(
            IMemoryAllocator* allocator,
            IInputStream* input);

    protected:
        CArchiveTar();
        virtual ~CArchiveTar();

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** Get num of files in ARC.
         */
        virtual IZ_UINT GetFileNum() const;

        /** Get max file size in ARC.
         */
        virtual IZ_UINT GetMaxFileSize() const;

        /** Get InputStream.
         */
        IInputStream* GetInputStream();

        /** Seek file by key.
         */
        virtual IZ_BOOL SeekByKey(
            IZ_UINT key,
            SArchiveFileDesc* desc);

        /** Seek file by path.
         */
        virtual IZ_BOOL SeekByPath(
            IZ_PCSTR path,
            SArchiveFileDesc* desc);

        /** Read data.
         */
        virtual IZ_BOOL Read(
            void* dst,
            const SArchiveFileDesc& desc);

    private:
        class CTarHeader;

        enum {
            HASH_SIZE = 5,
        };

        typedef CStdHash<CKey, CTarHeader, HASH_SIZE> TarHeaderHash;

        IMemoryAllocator* m_Allocator;

        IInputStream* m_Input;

        TarHeaderHash m_Headers;

        IZ_UINT m_MaxBytes;
    };
}   // namespace resource
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_RESOURCE_ARCHIVE_TARBALL_H__)
