#if !defined(__FILE_TREE_H__)
#define __FILE_TREE_H__

#include <vector>
#include <map>
#include "izStd.h"
#include "FileDefs.h"

// ファイルツリー
class CFileTree {
private:
	static CFileTree s_cInstance;

public:
	static CFileTree& GetInstance() { return s_cInstance; }

	typedef std::vector<SFile>::iterator FILE_LIST_ITER;

private:
	CFileTree() {}
	~CFileTree() {}

public:
	// XMLから情報を読み込む
	BOOL ReadFromXML(LPCSTR lpszIn);

	// 指定ディレクトリの情報を登録する
	BOOL RegisterFileTree(LPCSTR lpszPath);

	std::vector<SFile>& GetFileList() { return m_FileList; }

private:
	void RegisterFileTreeInternal(LPCSTR lpszDir);

private:
	std::map<izanagi::CKey, SFile> m_FileInfoMap;
	std::vector<SFile> m_FileList;

	std::vector<izanagi::tool::CString> m_TmpDirList;
};

#endif	// #if !defined(__FILE_TREE_H__)
