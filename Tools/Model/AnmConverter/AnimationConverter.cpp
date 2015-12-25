// AnimationConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "izModelLib.h"
#include "izToolKit.h"
#include "Option.h"

#define ENABLE_FBX_IMPORT

#ifdef ENABLE_FBX_IMPORT
#include "..\FbxImporter\FbxImporter.h"
#ifdef _DEBUG
#pragma comment(lib, "..\\..\\Model\\FbxImporter\\lib\\Debug\\FbxImporter.lib")
#else
#pragma comment(lib, "..\\..\\Model\\FbxImporter\\lib\\Release\\FbxImporter.lib")
#endif
#endif

#define INVALID_RET_VAL     (1)

namespace {
    void _DispUsage()
    {
        printf(
            "AnimationConverter Version 0.01 b\n"
            "\n"
            "モーションコンバータ\n"
            "\n"
            "Usage : AnimationConverter [options]\n"
            "\n"
            "[Options]\n"
            " -i [in]    : 入力ファイル\n"
            " -o [out]   : 出力ファイル\n"
            "              指定がない場合は入力ファイル名から自動生成\n"
            " -b [model] : ベースとなるモデルファイル\n"
            "              ファイルタイプは入力ファイルと同じでないといけない\n"
            "              指定がない場合は入力ファイルと同じ\n"
            " -f [type]  : 入力ファイルがどのようなファイルなのかを指定\n"
            "              指定がない場合は入力ファイルの拡張子から自動判定\n"
            "   dae : Colladaファイル入力\n"
            "   x   : XFileファイル入力\n"
            "   vmd : VMDファイル入力\n"
            " -idx [idx] : 複数モーションが含まれている場合にどのモーションを出力するか指定\n"
            "              指定がない場合は常に0となる\n"
        );
    }

    IZ_BOOL _ConvertVmdIk2Fk(
        const char* vmd,
        const char* pmd,
        izanagi::tool::CString& dst)
    {
        // TODO
        // MMDIK2FKの存在チェック

        izanagi::tool::CString cmd;
        izanagi::tool::CFileUtility::GetExecutionDirectory(cmd);

        cmd +="\\MMDIK2FK.exe ";
        cmd += pmd;
        cmd += " ";
        cmd += vmd;
        cmd += " ";

        // 一時出力ファイル作成
        izanagi::tool::CString tmp;
        izanagi::tool::CFileUtility::CreateTmpFileName(tmp);
        tmp += ".vmd";

        cmd += tmp;

        FILE* fp = _popen(cmd, "w");
        IZ_ASSERT(fp != IZ_NULL);

        // ０で正常終了
        int result = _pclose(fp);

        IZ_BOOL ret = (result == 0);

        if (ret)
        {
            dst = tmp;
        }

        return ret;
    }
}

#define ENABLE_CONV_IK_FK

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

#ifdef ENABLE_FBX_IMPORT
    if (importer == IZ_NULL && option.modelType == ModelType::ModelTypeFBX) {
        importer = new CFbxImporter();
    }
#endif

    // VMDの場合はIK -> FK 変換を行う
#ifdef ENABLE_CONV_IK_FK
    if (option.modelType == ModelTypePMD)
    {
        VRETURN_VAL(
            _ConvertVmdIk2Fk(
                option.in.c_str(),
                option.base.c_str(),
                option.in),
            INVALID_RET_VAL);
    }
#endif

    // 入力ファイルを開く
    VRETURN_VAL(
        importer->Open(option.in.c_str(), IZ_TRUE),
        INVALID_RET_VAL);

    // ベースモデルファイルを開く
    VRETURN_VAL(
        importer->ReadBaseModel(option.base.c_str()),
        INVALID_RET_VAL);

    // モーションファイル出力
    VRETURN_VAL(
        CAnmExporter::GetInstance().Export(
            option.out.c_str(),
            option.idx,
            importer),
        INVALID_RET_VAL);

    // IK -> FK 変換したファイルを削除する
#ifdef ENABLE_CONV_IK_FK
    if (option.modelType == ModelTypePMD)
    {
        izanagi::tool::CFileUtility::RemoveFile(option.in.c_str());
    }
#endif

    return 0;
}

