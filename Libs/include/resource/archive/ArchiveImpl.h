#if !defined(__IZANAGI_ARCHIVE_ARCHIVE_IMPL_H__)
#define __IZANAGI_ARCHIVE_ARCHIVE_IMPL_H__

#include "izStd.h"
#include "io/stream/InputStream.h"
#include "Archive.h"

namespace izanagi {
	//
	class CArchive : public IArchive {
	public:
		// インスタンス作成
		static CArchive* CreateArchive(
			IMemoryAllocator* pAllocator,
			IInputStream* pInput);

	private:
		static CArchive* CreateArchive(
			const S_ARC_HEADER& sHeader,
			IMemoryAllocator* pAllocator,
			IInputStream* pInput);

	protected:
		CArchive();
		~CArchive();

		NO_COPIABLE(CArchive);

	protected:
		void InternalRelease()
		{
			delete this;
			FREE(m_pAllocator, this);
		}

	protected:
		// Get num of files in ARC.
		IZ_UINT GetFileNum() const { return m_sHeader.numFiles; }

		// Get max file size in ARC.
		IZ_UINT GetMaxFileSize() const { return m_sHeader.maxFileSize; }

		// Get InputStream.
		IInputStream* GetInputStream() { return m_pInput; }

		// Seek file by key.
		IZ_BOOL Seek(IZ_UINT nKey);

		// Seek file by path.
		IZ_BOOL Seek(IZ_PCSTR path);

		// Return current file's description.
		IZ_BOOL GetFileDesc(SArchiveFileDesc* pDesc);

	protected:
		IZ_BOOL SeekStream(IZ_UINT nPos);

		IZ_BOOL GetFileDesc(
			IZ_UINT nKey,
			SArchiveFileDesc* pDesc);

		// Get file's size by key.
		IZ_UINT GetSize(IZ_UINT nKey) const;

		// Get file's name.
		IZ_PCSTR GetName(IZ_UINT nPos) const;

		// Get file header.
		const S_ARC_FILE_HEADER* GetFileHeader(IZ_UINT nKey) const
		{
			IZ_UINT nNumAlls = m_sHeader.numFiles;
			VRETURN_NULL(nKey < nNumAlls);

			IZ_ASSERT(m_pFileHeaders != IZ_NULL);
			return &m_pFileHeaders[nKey];
		}

	protected:
		enum {
			HASH_SIZE = 5,
		};

		typedef CStdHash<CKey, S_ARC_FILE_HEADER, HASH_SIZE>	CArcHash;
		typedef CArcHash::Item	CArcHashItem;

		IMemoryAllocator* m_pAllocator;

		IInputStream* m_pInput;

		// ファイルヘッダ
		S_ARC_HEADER m_sHeader;

		IZ_BYTE* m_pNameBuffer;
		S_ARC_FILE_HEADER* m_pFileHeaders;

		CArcHashItem* m_pHashItems;
		CArcHash m_HashList;

		const S_ARC_FILE_HEADER* m_pCurFileHeader;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_ARCHIVE_ARCHIVE_IMPL_H__)
