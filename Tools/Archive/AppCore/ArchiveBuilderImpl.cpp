#include <stdafx.h>
#include <vector>
#include "ArchiveBuilderImpl.h"
#include "FileTree.h"
#include "FileIdCreator.h"
#include "Utility.h"
#include "Option.h"
#include "AppError.h"
#include "izResource.h"
#include "izToolKit.h"

static const BYTE ZERO_BUF[4] = { 0, 0, 0, 0 };

namespace {
	inline BOOL _PaddingZero(
		izanagi::IOutputStream* pOut,
		IZ_UINT nSize)
	{
		nSize = ((nSize & 0x03) == 0 ? nSize + 1 : nSize);

		size_t padding = nSize & 0x03;
		padding = (padding > 0 ? 4 - padding : 0);

		if (padding > 0) {
			IZ_OUTPUT_WRITE_VRETURN(pOut, ZERO_BUF, 0, padding);
		}

		return TRUE;
	}

	inline void _CombinePath(
		CString& strRet,
		const std::vector<CString>& tPathList)
	{
		strRet.Empty();

		std::vector<CString>::const_iterator it = tPathList.begin();

		for (UINT cnt = 0; it != tPathList.end(); it++, cnt++) {
			const CString& str = *it;

			if (cnt == 0) {
				strRet = str;
			}
			else {
				strRet += '/';
				strRet += str;
			}
		}
	}
}	// namespace

//////////////////////////////////////////////////////////////////////////////

class CStringExporter {
public:
	CStringExporter(izanagi::IOutputStream* pOut) { m_pOut = pOut; }
	CStringExporter() {}

public:
	BOOL Export(
		LPCSTR lpszStr,
		LPCSTR lpszRoot);

	UINT GetExportedSize() { return m_pOut->GetSize(); }

protected:
	izanagi::IOutputStream* m_pOut;
};

BOOL CStringExporter::Export(
	LPCSTR lpszPath,
	LPCSTR lpszRoot)
{
	CString strPath;
	std::vector<CString> tPathList;

	// パスを '/' or '\' で分解する
	CUtility::BreakPath(
		lpszPath,
		lpszRoot,
		tPathList);

	_CombinePath(
		strPath,
		tPathList);

	size_t len = strPath.GetLength();

	IZ_OUTPUT_WRITE_VRETURN(m_pOut, strPath, 0, len);

	_PaddingZero(
		m_pOut,
		len);

	return TRUE;
}

//////////////////////////////////////////////////////////////////////////////

namespace {
	inline UINT _GetFileSize(LPCSTR lpszFile)
	{
		FILE* fp = NULL;

		fopen_s(&fp, lpszFile, "rb");
		ASSERT(fp != NULL);

		fseek(fp, 0, SEEK_END);
		UINT ret = ftell(fp);

		fclose(fp);

		return ret;
	}

	inline UINT _CopyFileToFile(
		LPCSTR lpszFile,
		izanagi::IOutputStream* pOut)
	{
		static const UINT BUF_SIZE = 2048;
		static BYTE BUF[BUF_SIZE];

		izanagi::CFileInputStream cIn;
		VRETURN(cIn.Open(lpszFile));

		IZ_UINT nReadSize = 0;

		for (;;) {
			IZ_UINT nSize = cIn.Read(BUF, 0, sizeof(BUF));
			if (nSize == 0) {
				break;
			}

			if (!pOut->Write(BUF, 0, nSize)) {
				return 0;
			}

			nReadSize += nSize;
		}

		_PaddingZero(
			pOut,
			nReadSize);

		return nReadSize;
	}
}	// namespace

//////////////////////////////////////////////////////////////////////////////

// NOTE
// フォーマット
// +--------------------+
// |   ファイルヘッダ   |
// +--------------------+
// |   文字列バッファ   |
// +--------------------+
// |    ファイル情報    +-- numFiles 個
// +--------------------+
// |   ファイルデータ   |
// +--------------------+

// 出力
BOOL CArchiveBuilderImpl::Export(const SOption& sOption)
{
	LPCSTR lpszRoot = izanagi::izanagi_tk::CFileUtility::GetFileNameFromPath(sOption.in_dir);
	LPCSTR lpszOut = sOption.output;		// 出力

	size_t nFileNum = CFileTree::GetInstance().GetFileList().size();
	std::vector<izanagi::S_ARC_FILE_HEADER> tFileHeaderList(nFileNum);

	// 出力先
	izanagi::CFileOutputStream cOut;
	THROW_EXCEPTION(
		cOut.Open(lpszOut),
		CErrorLog,
		"Failed : File Open [%s]",
		lpszOut);

	CStringExporter cStringExporter(&cOut);

	// ヘッダ
	izanagi::S_ARC_HEADER sHeader;
	{
		FILL_ZERO(&sHeader, sizeof(sHeader));

		sHeader.magic = izanagi::ARC_MAGIC_NUMBER;
		sHeader.version = izanagi::E_ARC_VERSION_CURRENT;

		sHeader.sizeHeader = sizeof(sHeader);
	}

	// ヘッダ分あける
	VRETURN(
		cOut.Seek(
			sizeof(sHeader), 
			izanagi::E_IO_STREAM_SEEK_POS_START));

	// ファイルヘッダ
	{
		// ヘッダ位置
		sHeader.posFileHeader = cOut.GetCurPos();

		// ファイルヘッダ分あける
		VRETURN(
			cOut.Seek(
				sizeof(izanagi::S_ARC_FILE_HEADER) * tFileHeaderList.size(),
				izanagi::E_IO_STREAM_SEEK_POS_CUR));

		// ファイルヘッダサイズ
		sHeader.sizeFileHeader = cOut.GetCurPos() - sHeader.posFileHeader;
	}

	CFileTree::FILE_LIST_ITER it = CFileTree::GetInstance().GetFileList().begin();
	CFileTree::FILE_LIST_ITER it_end = CFileTree::GetInstance().GetFileList().end();

	// 文字列データ
	{
		// 文字列データ位置
		sHeader.posName = cOut.GetCurPos();

		// Export strings.
		for (; it != it_end; it++) {
			SFile& sFile = *it;
			sFile.name_pos = cStringExporter.GetExportedSize();
			VRETURN(cStringExporter.Export(sFile.path, lpszRoot));
		}

		// Export terminate of strings.
		IZ_OUTPUT_WRITE_VRETURN(&cOut, ZERO_BUF, 0, 1);
		VRETURN(_PaddingZero(&cOut, 1));

		// 文字列データサイズ
		sHeader.sizeName = cOut.GetCurPos() - sHeader.posName;
	}

	CFileIdCreator::GetInstance().SetRoot(lpszRoot);

	it = CFileTree::GetInstance().GetFileList().begin();

	{
		// ファイルデータ位置
		sHeader.posFileData = cOut.GetCurPos();

		// Export file data.
		for (IZ_UINT i = 0; it != it_end; it++, i++) {
			const SFile& sFile = *it;
			izanagi::S_ARC_FILE_HEADER& sFileHeader = tFileHeaderList[i];

			sFileHeader.sizeName = static_cast<IZ_UINT>(strlen(sFile.path));
			sFileHeader.posName = sFile.name_pos - sHeader.posName;
			
			sFileHeader.pos = cOut.GetCurPos();;
			sFileHeader.size = _GetFileSize(sFile.path);

			sFileHeader.isCompressed = sFile.isCompress;
			sFileHeader.isEncrypted = sFile.isEncrypt;

			// 最大ファイルサイズ
			sHeader.maxFileSize = IZ_MAX(sHeader.maxFileSize, sFileHeader.size);

			// TODO

			VRETURN(_CopyFileToFile(sFile.path, &cOut) > 0);

			// ファイルヘッダ出力用登録
			CFileIdCreator::GetInstance().Register(i, sFile.path);
		}
	}

	// ヘッダ情報
	{
		sHeader.numFiles = static_cast<IZ_UINT>(nFileNum);
		sHeader.sizeFile = cOut.GetSize();
		sHeader.sizeFileData = sHeader.sizeFile - sHeader.posFileData;
	}

	// 先頭に戻る
	VRETURN(cOut.Seek(0, izanagi::E_IO_STREAM_SEEK_POS_START));

	// ヘッダ出力
	IZ_OUTPUT_WRITE_VRETURN(
		&cOut,
		&sHeader,
		0,
		sizeof(sHeader));

	// ファイルヘッダ出力
	IZ_OUTPUT_WRITE_VRETURN(
		&cOut, 
		&tFileHeaderList[0],
		0,
		sizeof(izanagi::S_ARC_FILE_HEADER) * tFileHeaderList.size());

	return TRUE;
}

// 中間データ出力
BOOL CArchiveBuilderImpl::ExportObj(const SOption& sOption)
{
	CFileTree::FILE_LIST_ITER it = CFileTree::GetInstance().GetFileList().begin();
	CFileTree::FILE_LIST_ITER it_end = CFileTree::GetInstance().GetFileList().end();

	// TODO
	// Compress
	// Encrypt

	return TRUE;
}
