#include <shlwapi.h>
#include "Option.h"

namespace {
    // オプションで共有するバッファ
    CHAR s_BUF[1024]; 
}

COption::COption()
{
    type = izanagi::E_PLATFORM_DX9;
}

/**
* 解析
*/
BOOL COption::Analysis(int argc, TCHAR* argv[])
{
    for (int i = 1; i < argc; i++) {
        BOOL result = FALSE;
        izanagi::tool::CString cmd(argv[i]);

        if (i < argc - 1) {
            if (result = (cmd == "-i")) {
                // -i
                in.format("%s", argv[++i]);
                in.replace('/', '\\');
            }
            else if (result = (cmd == "-o")) {
                // -o
                out.format("%s", argv[++i]);
            }
            else if (result = (cmd == "-p")) {
                // -p
                izanagi::tool::CString tmp(argv[++i]);
                tmp = tmp.make_lower();

                if (tmp == "dx9") {
                    type = izanagi::E_PLATFORM_DX9;
                }
                else if (tmp == "gles2") {
                    type = izanagi::E_PLATFORM_GLES2;
                }
                else {
                    result = FALSE;
                }
            }
        }

        if (!result) {
            // 引数無しオプション
        }

        if (!result) {
            // TODO
            ::fprintf(stderr, "Invalid Option [%s]\n\n", cmd.c_str());
            //IZ_ASSERT(FALSE);
            return FALSE;
        }
    }

    // 後処理
    if (!AfterAnalysis()) {
        IZ_ASSERT(FALSE);
        return FALSE;
    }

    return TRUE;
}

BOOL COption::IsValid() const
{
    return (!out.empty() && !in.empty());
}

// 解析の後処理
BOOL COption::AfterAnalysis()
{
    memset(s_BUF, 0, sizeof(s_BUF));

    BOOL ret = !in.empty();

    if (out.empty() && !in.empty()) {
        // 出力ファイルが設定されていない
        // 入力ファイルを基に出力ファイル名を決める

        // ファイル名取得
        ret = izanagi::tool::CFileUtility::RemoveExtension(
            s_BUF,
            sizeof(s_BUF),
            in.c_str());

        if (ret)
        {
            out.format("%s.img", s_BUF);
            memset(s_BUF, 0, sizeof(s_BUF));
        }
    }

    return ret;
}
