#include "shlwapi.h"
#include "Option.h"
#include "Preproc.h"
#include "izToolKit.h"

namespace {
    // オプションで共有するバッファ
    CHAR s_BUF[1024]; 
}

/**
* コンストラクタ
*/
COption::COption()
{
    type = ShaderCompilerType_DX9;

    isPreproc = FALSE;
    isPreprocShader = FALSE;
}

/**
* 引数解析
*/
BOOL COption::Analysis(int argc, char* argv[])
{
    for (int i = 1; i < argc; i++) {
        BOOL result = FALSE;
        izanagi::tool::CString cmd(argv[i]);

        if (i < argc - 1) {
            if (result = (cmd == "-I")) {
                // -I
                izanagi::tool::CString tmp;
                tmp.format("%s", argv[i + 1]);

                includes.push_back(tmp);

                i++;
            }
            else if (result = (cmd == "-D")) {
                // -D
                izanagi::tool::CString tmp;
                tmp.format("%s", argv[i + 1]);

                defines.push_back(tmp);

                i++;
            }
            else if (result = (cmd == "-obj")) {
                // -obj
                obj_dir.format("%s", argv[i + 1]);
                i++;
            }
            else if (result = (cmd == "-src")) {
                // -src
                in_file.format("%s", argv[i + 1]);
                i++;
            }
            else if (result = (cmd == "-out_tmp")) {
                // -out_tmp
                out_file.format("%s", argv[i + 1]);
                i++;
            }
            else if (result = (cmd == "-E")) {
                // -E
                in_file.format("%s", argv[i + 1]);
                i++;

                isPreproc = TRUE;
            }
            else if (result = (cmd == "-o")) {
                // -o
                export_dir.format("%s", argv[i + 1]);
                i++;
            }
            else if (result = (cmd == "-t")) {
                // -t
                izanagi::tool::CString strType(argv[i + 1]);
                i++;

                strType = strType.make_lower();

                result = IZ_TRUE;

                if (strType == "dx9") {
                    type = ShaderCompilerType_DX9;
                }
                else if (strType == "gles2") {
                    type = ShaderCompilerType_GLES2;
                }
                else {
                    result = IZ_FALSE;
                }
            }
        }

        if (!result) {
            if (result = (cmd == "-PreprocShd")) {
                // -PreprocShd
                isPreprocShader = TRUE;
            }
        }

        if (!result) {
            // TODO
            printf("無効なオプションです[%s]\n\n", cmd);
            IZ_ASSERT(IZ_FALSE);
            return FALSE;
        }
    }

    VRETURN(EndAnalysis());
    VRETURN(IsValid());

    return TRUE;
}

/**
* オプションの正当性チェック
*/
BOOL COption::IsValid()
{
    if (in_file.empty()) {
        // シェーダファイルが空
        // TODO

        return FALSE;
    }
    else if (!izanagi::tool::CFileUtility::IsExist(in_file)) {
        // ファイルの存在を確認
        // TODO

        return FALSE;
    }

    if (IsPreproc()) {
        if (out_file.empty()) {
            // 出力ファイルが空
            // TODO

            return FALSE;
        }
        else if (izanagi::tool::CFileUtility::IsReadOnly(out_file)) {
            // 出力ファイルが上書き不可
            // TODO

            return FALSE;
        }
    }

    return TRUE;
}

BOOL COption::EndAnalysis()
{
    // 環境変数の展開
    if (!in_file.empty()) {
        VRETURN(
            izanagi::tool::CEnvVarHelper::ExpandEnvStrings(
                s_BUF,
                sizeof(s_BUF),
                in_file));
    }

#if 0
    // 出力ファイル
    if (out_file.empty() && !in_file.empty()) {
        FILL_ZERO(s_BUF, sizeof(s_BUF));

        // 出力ファイルが空
        memcpy(s_BUF, in_file, min(sizeof(s_BUF), strlen(in_file)));

        // ファイル名取得
        LPSTR file_name = PathFindFileName(s_BUF);

        // 拡張子削除
        PathRemoveExtension(file_name);

        out_file.format(_T("%s.shd"), file_name);
    }
#endif

    // プリプロセス済みファイル
    if (IsPreproc() && !out_file.empty()) {
        preproc_file = out_file;
    }
    else if (!in_file.empty()) {
        FILL_ZERO(s_BUF, sizeof(s_BUF));

        // 出力ファイルが空
        memcpy(s_BUF, in_file, min(sizeof(s_BUF), strlen(in_file)));

        // ファイル名取得
        izanagi::tool::CString file_name(
            izanagi::tool::CFileUtility::GetFileNameFromPath(s_BUF));

        izanagi::tool::CString ext(
            izanagi::tool::CFileUtility::GetExtension(
                s_BUF,
                sizeof(s_BUF),
                file_name));

        // 拡張子削除
        VRETURN(
            izanagi::tool::CFileUtility::RemoveExtension(
                s_BUF,
                sizeof(s_BUF),
                file_name));

        if (obj_dir.empty()) {
            preproc_file.format("%s.%s_", s_BUF, ext.c_str());
        }
        else {
            izanagi::tool::CString tmp(s_BUF);

            // 中間ディレクトリに出力する
            VRETURN(
                izanagi::tool::CFileUtility::CombinePath(
                    s_BUF, 
                    sizeof(s_BUF),
                    obj_dir,
                    tmp));

            preproc_file.format("%s.%s_", s_BUF, ext.c_str());
        }
    }

    // includeパスの環境変数の展開
    std::vector<izanagi::tool::CString>::iterator it = includes.begin();
    while (it != includes.end()) {
        izanagi::tool::CString& str = *it;
        
        VRETURN(
            izanagi::tool::CEnvVarHelper::ExpandEnvStrings(
                s_BUF,
                sizeof(s_BUF),
                str));
        
        it++;
    }

    if (!IsPreproc()) {
        // TODO
        // デフォルトincludeパスの追加
    }

    return TRUE;
}

BOOL COption::PreprocInputFile()
{
    BOOL ret = TRUE;

    // exe名
    VRETURN(
        izanagi::tool::CFileUtility::GetExeModuleName(
            s_BUF,
            sizeof(s_BUF)));

    // 自分自身をプリプロセス処理モードで呼び出す
    ret = ExecWithPreprocMode(s_BUF, *this);

    if (!ret) {
        // 失敗・・・
        IZ_ASSERT(IZ_FALSE);

        // TODO

        goto __EXIT__;
    }

    this->in_file = this->preproc_file;
    this->preproc_file.clear();

    isPreproc = FALSE;

__EXIT__:
    return ret;
}
