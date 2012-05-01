#if !defined(__IZANAGI_ARCHIVE_ARCHIVE_ZIP_H__)
#define __IZANAGI_ARCHIVE_ARCHIVE_ZIP_H__

#include "izDefs.h"
#include "izStd.h"
#include "izIo.h"
#include "Archive.h"

namespace izanagi {
	class CArchiveZip : public IArchive {
	protected:
		struct ZipDirHeader;
		struct ZipDirFileHeader;

		static const IZ_INT ZipDirHeaderSize;
		static const IZ_INT ZipDirFileHeaderSize;

	public:
		static IArchive* CreateArchiveZip(
			IMemoryAllocator* pAllocator,
			IInputStream* pIn);

	protected:
		CArchiveZip();
		~CArchiveZip();

		NO_COPIABLE(CArchiveZip);

	protected:
		void InternalRelease()
		{
			delete this;
			FREE(m_Allocator, this);
		}

	public:
		// Get InputStream.
		IInputStream* GetInputStream();

		// Get num of files in ARC.
		IZ_UINT GetFileNum() const;

		// Get max file's size in ARC.
		IZ_UINT GetMaxFileSize() const;

		// Seek file by key.
		IZ_BOOL Seek(IZ_UINT nKey);

		// Seek file by path.
		IZ_BOOL Seek(IZ_PCSTR path);

		// Return current file's description.
		IZ_BOOL GetFileDesc(SArchiveFileDesc* pDesc);

	private:
		// Initialize.
		IZ_BOOL Init(
			const ZipDirHeader& sDirHeader,
			IInputStream* pIn);

		// Read ZipDirHeader from stream.
		static IZ_UINT ReadZipDirHeader(
			IInputStream* pIn,
			ZipDirHeader* pHeader);

		// Read ZipDirFileHeader from stream.
		static IZ_CHAR* ReadZipDirFileHeader(
			IInputStream* pIn,
			IZ_CHAR* pString,
			IZ_UINT* pStrPos,
			ZipDirFileHeader* pHeader);

		inline IZ_CHAR* GetName(IZ_UINT pos) const;

		IZ_BOOL SeekStream(IZ_UINT nPos);

	private:
		enum {
			HASH_SIZE = 5,
		};

		typedef CStdHash<CKey, ZipDirFileHeader, HASH_SIZE>		CZipHash;

		IMemoryAllocator* m_Allocator;

		IInputStream* m_pInput;

		ZipDirFileHeader* m_pFileHeaders;
		IZ_CHAR* m_pString;

		IZ_UINT m_nFileNum;
		IZ_UINT m_nMaxFileSize;

		const ZipDirFileHeader* m_pCurFileHeader;

		CZipHash m_HashList;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_ARCHIVE_ARCHIVE_ZIP_H__)
