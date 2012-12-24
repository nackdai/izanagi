#include "Option.h"
#include "izToolKit.h"

COption::COption()
{
    exportType = ExportTypeMsh | ExportTypeSkl;
    maxJointMtxNum = 0;
    isExportTriList = IZ_FALSE;
}

COption::~COption()
{
}

// 解析
IZ_BOOL COption::Analysis(int argc, char* argv[])
{
    for (IZ_INT i = 1; i < argc; i++) {
        IZ_BOOL result = IZ_FALSE;
        std::string opt(argv[i]);

        if (i < argc - 1) {
            if (result = (opt == "-i")) {
                in = argv[++i];
            }
            else if (result = (opt == "-o")) {
                out = argv[++i];
            }
            else if (result = (opt == "-e")) {
                std::string subOpt(argv[++i]);

                if (result = (subOpt == "mdl")) {
                    exportType = ExportTypeMdl;
                }
                else if (result = (subOpt == "msh")) {
                    exportType = ExportTypeMsh;
                }
                else if (result = (subOpt == "skl")) {
                    exportType = ExportTypeSkl;
                }
            }
            else if (result = (opt == "-f")) {
                std::string subOpt(argv[++i]);

                result = AnalysisModelType(subOpt);
            }
            else if (result = (opt == "-mtx")) {
                maxJointMtxNum = ::atoi(argv[++i]);
            }
        }

        if (!result)
        {
            if (result = (opt == "-list"))
            {
                isExportTriList = IZ_TRUE;
            }
        }

        if (!result) {
            // TODO
            printf("無効なオプションです[%s]\n\n", opt.c_str());
            //ASSERT(FALSE);
            return IZ_FALSE;
        }
    }

    IZ_BOOL ret = AnalysisInternal();

    return ret;
}

static char BUF[MAX_PATH];

IZ_BOOL COption::AnalysisInternal()
{
    if (out.empty()
        || (outMsh.empty() && (exportType & ExportTypeMsh))
        || (outSkl.empty() && (exportType & ExportTypeSkl)))
    {
        // 出力ファイルが空なので、入力ファイルから作成する

        // 拡張子を除いた入力ファイル名を取得
        IZ_BOOL result = izanagi::tool::CFileUtility::RemoveExtension(
                            BUF,
                            sizeof(BUF),
                            in.c_str());
        VRETURN(result);

        // 出力タイプに応じて拡張子を付加する
        if (exportType == ExportTypeMdl) {
            out.format("%s.mdl", BUF);
        }
        else {
            if (exportType & ExportTypeMsh) {
                outMsh.format("%s.msh", BUF);
            }
            if (exportType & ExportTypeSkl) {
                outSkl.format("%s.skl", BUF);
            }
        }
    }

    if (!IsValidModelType()) {
        // 入力ファイルの拡張子から判定する

        // 拡張子を取得
        IZ_PCSTR ext = izanagi::tool::CFileUtility::GetExtension(
                        BUF,
                        sizeof(BUF),
                        in.c_str());
        VRETURN(ext != IZ_NULL);

        std::string strExt(ext);

        AnalysisModelType(strExt);
    }

    return IZ_TRUE;
}

IZ_BOOL COption::IsValid()
{
    VRETURN(!in.empty());
    VRETURN(!out.empty() || !outMsh.empty() || !outSkl.empty());
    VRETURN(IsValidModelType());

    return IZ_TRUE;
}
