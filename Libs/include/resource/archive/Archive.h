#if !defined(__IZANAGI_ARCHIVE_ARCHIVE_H__)
#define __IZANAGI_ARCHIVE_ARCHIVE_H__

#include "izStd.h"
#include "ARCFormat.h"

namespace izanagi {
	class IInputStream;

	/**
	*/
	struct SArchiveFileDesc {
		IZ_PCSTR name;				// file name

		IZ_UINT size;
		IZ_UINT sizeCompressed;

		struct {
			IZ_UINT isCompressed	: 1;	// whether file is compressed
			IZ_UINT isEncrypted		: 1;	// whether file is encrypted
		};

		IZ_UINT key;
	};

	/**
	*/
	class IArchive : public CObject {
	protected:
		IArchive() {}
		virtual ~IArchive() {}

	public:
		// Get InputStream.
		virtual IInputStream* GetInputStream() = 0;

		// Get num of files in ARC.
		virtual IZ_UINT GetFileNum() const = 0;

		// Get max file's size in ARC.
		virtual IZ_UINT GetMaxFileSize() const = 0;

		// Seek file by key.
		virtual IZ_BOOL Seek(IZ_UINT nKey) = 0;

		// Seek file by path.
		virtual IZ_BOOL Seek(IZ_PCSTR path) = 0;

		// Return current file's description.
		virtual IZ_BOOL GetFileDesc(SArchiveFileDesc* pDesc) = 0;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_ARCHIVE_ARCHIVE_H__)
