#include <stdafx.h>
#include <shlwapi.h>
#include "FileTree.h"

#define DELIMITER_0	'/'
#define DELIMITER_1	'\\'

#ifndef VRETURN
#define VRETURN(b)	if(!(b)) { ASSERT(FALSE); return FALSE; }
#endif

CFileTree CFileTree::s_cInstance;

// XMLから情報を読み込む
BOOL CFileTree::ReadFromXML(LPCSTR lpszIn)
{
	// TODO
	ASSERT(FALSE);
	return FALSE;
}

// 指定ディレクトリの情報を登録する
BOOL CFileTree::RegisterFileTree(LPCSTR lpszPath)
{
	CString strDir(PathFindFileName(lpszPath));
	
	// 末尾の'/'を消す
	{
		size_t nStrLen = strlen(strDir);
		int nDelimiterPos_0 = strDir.ReverseFind(DELIMITER_0);
		int nDelimiterPos_1 = strDir.ReverseFind(DELIMITER_1);

		if ((nStrLen == nDelimiterPos_0)
			|| (nStrLen == nDelimiterPos_1))
		{
			LPCSTR str = strDir;
			const_cast<CHAR*>(str)[nStrLen] = '\0';
		}
	}

	// 自分自身をチェック
	{
		WIN32_FIND_DATA dataFile;
		HANDLE hIter = FindFirstFile(strDir, &dataFile);

		VRETURN(hIter != NULL);
		VRETURN(dataFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY);
	}

	// TODO
	m_FileList.reserve(100);

	// TODO
	m_TmpDirList.reserve(10);
	m_TmpDirList.push_back(strDir);

	while (m_TmpDirList.size() > 0) {
		std::vector<CString>::iterator it = m_TmpDirList.begin();
		ASSERT(it != m_TmpDirList.end());

		const CString& strDir = *it;
		RegisterFileTreeInternal(strDir);

		m_TmpDirList.erase(it);
	}

	return TRUE;
}

void CFileTree::RegisterFileTreeInternal(LPCSTR lpszDir)
{
	ASSERT(lpszDir != NULL);

	WIN32_FIND_DATA dataFile;
	HANDLE hIter = NULL;
	{
		CString strSearchPath(lpszDir);
		strSearchPath += "/*.*";

		hIter = FindFirstFile(strSearchPath, &dataFile);
	}

	while (hIter != INVALID_HANDLE_VALUE) {
		CString strName(dataFile.cFileName);

		if (strName == '.' || strName == "..") {
			// '.' or '..' は除く
			// Nothing is done
		}
		else if (!(dataFile.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
			// ファイル
			SFile sFile;
			{
				sFile.path.Format(_T("%s/%s"), lpszDir, dataFile.cFileName);
			}

			m_FileList.push_back(sFile);
		}
		else {
			// ディレクトリ
			CString strDir;
			strDir.Format(_T("%s/%s"), lpszDir, dataFile.cFileName);

			m_TmpDirList.push_back(strDir);
		}

		if (!FindNextFile(hIter, &dataFile)) {
			break;
		}
	}
}
