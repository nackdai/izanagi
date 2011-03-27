#if !defined(__UTILITY_H__)
#define __UTILITY_H__

#include <vector>

class CUtility {
private:
	CUtility();
	~CUtility();

public:
	// パスを '/' or '\' で分解する
	static void BreakPath(
		LPCSTR lpszPath,
		LPCSTR lpszRoot,
		std::vector<CString>& tPathList);
};

#endif	// #if !defined(__UTILITY_H__)
