#if !defined(__UTILITY_H__)
#define __UTILITY_H__

#include <vector>
#include "izToolKit.h"

class CUtility {
private:
	CUtility();
	~CUtility();

public:
	// パスを '/' or '\' で分解する
	static void BreakPath(
		LPCSTR lpszPath,
		LPCSTR lpszRoot,
		std::vector<izanagi::tool::CString>& tPathList);
};

#endif	// #if !defined(__UTILITY_H__)
