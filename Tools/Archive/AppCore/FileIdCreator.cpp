#include "FileIdCreator.h"
#include "Utility.h"
#include "izStd.h"
#include "izToolKit.h"

CFileIdCreator CFileIdCreator::s_cInstance;

void CFileIdCreator::Register(
	UINT nId,
	LPCSTR lpszPath)
{
	IZ_ASSERT(m_strRoot.length() > 0);

	std::vector<izanagi::tool::CString> tPathList;

	// パスを '/' or '\' で分解する
	CUtility::BreakPath(
		lpszPath,
		m_strRoot,
		tPathList);

	std::vector<izanagi::tool::CString>::iterator it = tPathList.begin();

	// パスとして構築しなおす
	{
		std::vector<izanagi::tool::CString>::iterator prevItem = tPathList.end();

		for (; it != tPathList.end(); it++) {
			izanagi::tool::CString& str = *it;

			if (prevItem != tPathList.end()) {
				const izanagi::tool::CString& strPrev = *prevItem;

				izanagi::tool::CString tmp(str);
				str.format("%s/%s", strPrev.c_str(), tmp.c_str());
			}

			prevItem = it;
		}
	}

	it = tPathList.begin();
	std::map<UINT, SIdInfo>::iterator prevItem = m_InfoList.end();

	// 登録済みか探す
	for (; it != tPathList.end(); it++) {
		const izanagi::tool::CString& str = *it;

		UINT nKey = izanagi::CKey::GenerateValue(str);

		std::map<UINT, SIdInfo>::iterator item = m_InfoList.find(nKey);
		if (item == m_InfoList.end()) {
			// 未登録
			SIdInfo sIdInfo;
			{
				sIdInfo.id = nId;
				sIdInfo.str = str;
			}

			// 登録
			std::pair<std::map<UINT, SIdInfo>::iterator, bool> result = m_InfoList.insert(
																			std::pair<UINT, SIdInfo>(nKey, sIdInfo));
			IZ_ASSERT(result.second);
			item = result.first;

			if (prevItem != m_InfoList.end()) {
				// 親子関係の構築
				SIdInfo& sResult = item->second;
				sResult.parent = &prevItem->second;
				sResult.parent->child.push_back(&sResult);
			}
		}
		else {
			// 登録済み
			if (prevItem != m_InfoList.end()) {
				SIdInfo& sParentInfo = prevItem->second;
				SIdInfo& sIdInfo = item->second;

				// 親子関係の構築
				sParentInfo.child.push_back(&sIdInfo);
				sIdInfo.parent = &sParentInfo;
			}
		}

		prevItem = item;
	}
}

BOOL CFileIdCreator::Export(LPCSTR lpszExport)
{
	FILE* fpOut = NULL;
	fopen_s(&fpOut, lpszExport, "wt");
	VRETURN(fpOut != NULL);

	m_bIsEnum = FALSE;

	UINT nRootKey = izanagi::CKey::GenerateValue(m_strRoot);

	std::map<UINT, SIdInfo>::const_iterator it = m_InfoList.find(nRootKey);
	IZ_ASSERT(it != m_InfoList.end());

	const SIdInfo& sRootInfo = it->second;

	BOOL ret = Export(
				fpOut,
				sRootInfo,
				0);
	IZ_ASSERT(ret);

	fclose(fpOut);

	return ret;
}

namespace {
	static izChar s_Buf[1024];

	inline void _PrintWithIndent(
		FILE* fpOut,
		UINT nIndent,
		LPCSTR format,
		...)
	{
		for (UINT i = 0; i < nIndent; i++) {
			fprintf(fpOut, "\t");
		}

		va_list	argp;
		va_start(argp, format);
		vsprintf_s(s_Buf, sizeof(s_Buf), format, argp);
		va_end(argp);

		fprintf(fpOut, s_Buf);
	}
}	// namespace

BOOL CFileIdCreator::Export(
	FILE* fpOut,
	const CFileIdCreator::SIdInfo& sInfo,
	UINT nIndent)
{
	if (sInfo.child.size() == 0) {
#ifdef _ENABLE_EXPORT_ENUM
		if (!m_bIsEnum) {
			m_bIsEnum = TRUE;
			_PrintWithIndent(fpOut, nIndent, "enum {\n");
		}
#endif	// #ifdef _ENABLE_EXPORT_ENUM

		// Get file extension
		izanagi::tool::CString strExt(
			izanagi::tool::CFileUtility::GetExtension(
				s_Buf,
				sizeof(s_Buf),
				sInfo.str));

		// Get file name from path without extension.
		VRETURN(
			izanagi::tool::CFileUtility::GetFileNameFromPathWithoutExt(
				s_Buf,
				sizeof(s_Buf),
				sInfo.str));

		 IZ_PCSTR pszPathWithoutExt = s_Buf;

		if (strExt.length() > 0) {
			_PrintWithIndent(
				fpOut,
#ifdef _ENABLE_EXPORT_ENUM
				nIndent + 1,
				"%s_%s = %d,\n",
#else	// #ifdef _ENABLE_EXPORT_ENUM
				nIndent,
				"static const IZ_UINT %s_%s = %d;\n",
#endif	// #ifdef _ENABLE_EXPORT_ENUM
				pszPathWithoutExt,
				strExt,
				sInfo.id);
		}
		else {
			_PrintWithIndent(
				fpOut,
#ifdef _ENABLE_EXPORT_ENUM
				nIndent + 1,
				"%s = %d,\n",
#else	// #ifdef _ENABLE_EXPORT_ENUM
				nIndent,
				"static const IZ_UINT %s = %d;\n",
#endif	// #ifdef _ENABLE_EXPORT_ENUM
				pszPathWithoutExt,
				sInfo.id);
		}
	}
	else {
		_PrintWithIndent(
			fpOut, 
			nIndent, 
			"class %s {\n", 
			izanagi::tool::CFileUtility::GetFileNameFromPath(sInfo.str));
		_PrintWithIndent(fpOut, nIndent, "public:\n");

		std::vector<SIdInfo*>::const_iterator it = sInfo.child.begin();
		for (; it != sInfo.child.end(); it++) {
			const CFileIdCreator::SIdInfo* pChildInfo = *it;

			BOOL result = Export(
							fpOut,
							*pChildInfo,
							nIndent + 1);
			VRETURN(result);
		}

#ifdef _ENABLE_EXPORT_ENUM
		if (m_bIsEnum) {
			m_bIsEnum = FALSE;
			_PrintWithIndent(fpOut, nIndent + 1, "};\n");
		}
#endif	// #ifdef _ENABLE_EXPORT_ENUM
		
		// end of class.
		_PrintWithIndent(fpOut, nIndent, "};\n");
	}

	return TRUE;
}
