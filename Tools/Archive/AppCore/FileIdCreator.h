#if !defined(__FILE_ID_CREATOR_H__)
#define __FILE_ID_CREATOR_H__

#include <map>
#include <vector>
#include "izToolKit.h"

#define _ENABLE_EXPORT_ENUM

class CFileIdCreator {
	struct SIdInfo {
		UINT id;
		izanagi::izanagi_tk::CString str;

		SIdInfo* parent;
		std::vector<SIdInfo*> child;

		SIdInfo()
		{
			id = 0;
			parent = NULL;
		}
	};

	static CFileIdCreator s_cInstance;

public:
	static CFileIdCreator& GetInstance() { return s_cInstance; }

private:
	CFileIdCreator() {}
	~CFileIdCreator() {}

public:
	void SetRoot(LPCSTR lpszRoot)
	{ 
		m_strRoot.format("%s", lpszRoot);
	}

	void Register(
		UINT nId,
		LPCSTR lpszPath);

	BOOL Export(LPCSTR lpszExport);

private:
	BOOL Export(
		FILE* fpOut,
		const SIdInfo& sInfo,
		UINT nIndent);

private:
	izanagi::izanagi_tk::CString m_strRoot;
	std::map<UINT, SIdInfo> m_InfoList;

	BOOL m_bIsEnum;
};

#endif	// #if !defined(__FILE_ID_CREATOR_H__)
