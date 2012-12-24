#include "Utility.h"

// パスを '/' or '\' で分解する
void CUtility::BreakPath(
    LPCSTR lpszPath,
    LPCSTR lpszRoot,
    std::vector<izanagi::tool::CString>& tPathList)
{
    static CHAR buf[1024];

    UINT nBufPos = 0;
    size_t len = strlen(lpszPath);

    BOOL bEnableAdd = FALSE;

    for (size_t i = 0; i < len; i++) {
        CHAR ch = lpszPath[i];
        if ((ch == '/') || (ch == '\\')) {
            buf[nBufPos] = '\0';
            nBufPos = 0;

            izanagi::tool::CString str(buf);
            if (str.length() > 0) {
                if (!bEnableAdd && (str == lpszRoot)) {
                    bEnableAdd = TRUE;
                }
                if (bEnableAdd) {
                    tPathList.push_back(str);
                }
            }
        }
        else {
            buf[nBufPos++] = ch;
        }
    }

    if (bEnableAdd && (nBufPos > 0)) {
        buf[nBufPos] = '\0';
        tPathList.push_back(izanagi::tool::CString(buf));
    }
}
