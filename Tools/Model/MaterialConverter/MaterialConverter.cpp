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
            "MaterialConverter Version 0.01 b\n"
            "\n"
            "マテリアルコンバータ\n"
            "\n"
            "Usage : MaterialConverter [options]\n"
            "\n"
            "[Options]\n"
            " -i [in]   : 入力ファイル\n"
            " -o [out]  : 出力ファイル\n"
            "             指定がない場合は入力ファイル名から自動生成\n"
            " -f [type] : 入力ファイルがどのようなファイルなのかを指定\n"
            "             指定がない場合は入力ファイルの拡張子から自動判定\n"
            "   dae : Colladaファイル入力\n"
            "   x   : XFileファイル入力\n"
            "   pmd : PMDファイル入力\n"
            "   fbx : FBXファイル入力\n"
            "\n"
            "******\n"
            "マテリアルが複数存在する場合は\n"
            "出力ファイルは [出力ファイル名]_[idx].mtrl という形で出力される\n"
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

#ifdef ENABLE_FBX_IMPORT
    if (importer == IZ_NULL && option.modelType == ModelType::ModelTypeFBX) {
        importer = new CFbxImporter();
    }
#endif

    // 入力ファイルを開く
    VRETURN_VAL(
        importer->Open(option.in.c_str()),
        INVALID_RET_VAL);

    static IZ_CHAR tmp[MAX_PATH];

    // マテリアル数を取得
    IZ_UINT mtrlNum = importer->GetMaterialNum();

    if (mtrlNum == 0) {
        // 何もしない

        // マテリアルが存在しないことを出力
        printf("There is not material in %s\n", option.in.c_str());
    }
    else if (mtrlNum == 1) {    
        VRETURN_VAL(
            CMtrlExporter::GetInstance().Export(
                option.out.c_str(),
                importer,
                0),
            INVALID_RET_VAL);
}
    else {
        // 出力ファイル名から拡張子を取り除く
        VRETURN_VAL(
            option.RemoveExtFromExportFileName(),
            INVALID_RET_VAL);

        for (IZ_UINT i = 0; i < mtrlNum; i++) {
            // 出力ファイル名を作成
            sprintf_s(tmp, sizeof(tmp), "%s_%d.mtrl\0", option.out.c_str(), i);
        
            VRETURN_VAL(
                CMtrlExporter::GetInstance().Export(
                    tmp,
                    importer,
                    i),
                INVALID_RET_VAL);
        }
    }

    return 0;
}

