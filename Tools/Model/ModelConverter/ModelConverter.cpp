// ModelConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "izModelLib.h"
#include "izToolKit.h"
#include "Option.h"

#define INVALID_RET_VAL     (1)

namespace {
    void _DispUsage()
    {
        printf(
            "ModelConverter Version 0.01 b\n"
            "\n"
            "This tool converts a model data file to izanagi original model data format file.\n"
            "\n"
            "Usage : ModelConverter [options]\n"
            "\n"
            "[Options]\n"
            " -i [in]   : input file\n"
            "\n"
            " -o [out]  : output file\n"
            "             If file name is not specified, file name is made from input file name automatically.\n"
            "\n"
            " -e [type] : output file type\n"
            "             If this option is not specified, export msh and skl.\n"
            "   mdl : export mdl (msh and skl are packed in mdl.)\n"
            "   msh : export msh (msh is mesh data.)\n"
            "   skl : export skl (skl is skelton data.)\n"
            "\n"
            " -f [type] : input file type\n"
            "             If this option is not specified, file type is calculated from input file extension.\n"
            "   dae  : Collada\n"
            "   x    : XFile\n"
            "   pmd  : PMD\n"
            "   json : JSON\n"
            "\n"
            " -mtx [num] : 指定した数の関節数までメッシュをまとめる\n"
            "              4 以上でかつ 2 の倍数しか指定できない\n"
            "\n"
            " -list     : メッシュをトライアングルリストとして出力\n"
            "             デフォルトではトライアングルストリップとして出力\n"
            " -bone_info : 骨情報を出力\n"
        );
    }
}

int main(int argc, char* argv[])
{
    COption option;

    // オプション解析
    if (!option.Analysis(argc, argv)) {
        _DispUsage();
        return INVALID_RET_VAL;
    }

    // オプションチェック
    if (!option.IsValid()) {
        _DispUsage();
        return INVALID_RET_VAL;
    }

    // インポーター作成
    IImporter* importer = IImporter::CreateImporter(option.modelType);
    
    // 入力ファイルを開く
    VRETURN_VAL(
        importer->Open(option.in.c_str()),
        INVALID_RET_VAL);

    // トライアングルリストで出力するかどうかを設定.
    CMshExporter::GetInstance().SetIsExportTriList(option.needExportTriList);

#if 1
    // 出力
    if (option.exportType == ExportTypeMdl) {
        // Model
        VRETURN_VAL(
            CMdlExporter::GetInstance().Export(
                option.maxJointMtxNum,
                option.out.c_str(),
                importer),
            INVALID_RET_VAL);
    }
    else {
        // Mesh
        VRETURN_VAL(
            CMshExporter::GetInstance().Export(
                option.maxJointMtxNum,
                option.outMsh.c_str(),
                importer),
            INVALID_RET_VAL);

        // Skeleton
        VRETURN_VAL(
            CSklExporter::GetInstance().Export(
                option.outSkl.c_str(),
                importer),
            INVALID_RET_VAL);
    }
#endif

    if (option.needExportBoneInfo)
    {
        // TODO
        static char tmp[256];
        izanagi::tool::CFileUtility::GetPathWithoutFileName(
            tmp,
            sizeof(tmp),
            option.outSkl.c_str());

        izanagi::tool::CString boneInfo;
        if (::strlen(tmp) == 0)
        {
            tmp[0] = '.';
            tmp[1] = 0;
        }
        boneInfo.format("%s\\bone_info.txt", tmp);

        VRETURN_VAL(
            CSklExporter::GetInstance().ExportBoneInfo(
                boneInfo.c_str(),
                importer),
            INVALID_RET_VAL);
    }

    return 0;
}
