#if !defined(__IZANAGI_RESOURCE_ARCHIVE_IMPL_H__)
#define __IZANAGI_RESOURCE_ARCHIVE_IMPL_H__

#include "izStd.h"
#include "resource/archive/ARCFormat.h"
#include "resource/archive/ArchiveInterface.h"

namespace izanagi
{
    class IInputStream;

namespace resource
{
    //
    class CArchive : public IArchive {
    public:
        /** インスタンス作成
         */
        static IArchive* CreateArchive(
            IMemoryAllocator* allocator,
            IInputStream* input);

    private:
        static CArchive* CreateArchive(
            const S_ARC_HEADER& header,
            IMemoryAllocator* allocator,
            IInputStream* input);

    protected:
        CArchive();
        virtual ~CArchive();

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** Get num of files in ARC.
         */
        virtual IZ_UINT GetFileNum() const { return m_sHeader.numFiles; }

        /** Get max file size in ARC.
         */
        virtual IZ_UINT GetMaxFileSize() const { return m_sHeader.maxFileSize; }

        /** Get InputStream.
         */
        IInputStream* GetInputStream() { return m_pInput; }

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

    protected:
        // Seek input stream.
        IZ_BOOL SeekStream(IZ_UINT nPos);

        // Set file description.
        void SetFileDesc(
            SArchiveFileDesc* dst,
            const S_ARC_FILE_HEADER& src);

        // Get file's name.
        IZ_PCSTR GetName(IZ_UINT pos) const;

        // Get file header.
        const S_ARC_FILE_HEADER* GetFileHeaderByIdx(IZ_UINT nKey) const;

    protected:
        enum {
            HASH_SIZE = 5,
        };

        typedef CStdHash<CKey, S_ARC_FILE_HEADER, HASH_SIZE>    CArcHash;
        typedef CArcHash::Item  CArcHashItem;

        IMemoryAllocator* m_Allocator;

        IInputStream* m_pInput;

        // ファイルヘッダ
        S_ARC_HEADER m_sHeader;

        IZ_BYTE* m_pNameBuffer;
        S_ARC_FILE_HEADER* m_pFileHeaders;

        const S_ARC_FILE_HEADER* m_CurrentFielHeader;

        CArcHashItem* m_pHashItems;
        CArcHash m_HashList;
    };
}   // namespace resource
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_RESOURCE_ARCHIVE_IMPL_H__)
