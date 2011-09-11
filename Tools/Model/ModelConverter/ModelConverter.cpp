// ModelConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "izModelLib.h"
#include "izToolKit.h"
#include "Option.h"

#define INVALID_RET_VAL		(1)

namespace {
	void _DispUsage()
	{
		printf(
			"ModelConverter Version 0.01 b\n"
			"\n"
			"モデルコンバータ\n"
			"\n"
			"Usage : ModelConverter [options]\n"
			"\n"
			"[Options]\n"
			" -i [in]   : 入力ファイル\n"
			"\n"
			" -o [out]  : 出力ファイル\n"
			"             指定がない場合は入力ファイル名から自動生成\n"
			"\n"
			" -e [type] : 出力ファイルタイプ\n"
			"             指定がない場合は msh と skl の２つを出力\n"
			"   mdl : mdlファイル出力（msh, skl を１つにパックしたもの）\n"
			"   msh : mshファイル出力（メッシュデータ）\n"
			"   skl : sklファイル出力（スケルトンデータ）\n"
			"\n"
			" -f [type] : 入力ファイルがどのようなファイルなのかを指定\n"
			"             指定がない場合は入力ファイルの拡張子から自動判定\n"
			"   dae : Colladaファイル入力\n"
			"   x   : XFileファイル入力\n"
			"   fbx : FBXファイル入力\n"
			"\n"
			" -mtx [num] : 指定した数の関節数までメッシュをまとめる\n"
			"              4 から 16 まで指定可能\n"
		);
	}
}

int _tmain(int argc, _TCHAR* argv[])
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

	IImporter* importer = IZ_NULL;
	
	if (option.fileType == FileTypeCollada) {
		// COLLADA
		importer = IImporter::CreateImporter<CColladaImporter>();
	}
	else if (option.fileType == FileTypeXFile) {
		// XFile
		importer = IImporter::CreateImporter<CXFileImporter>();
	}
	else if (option.fileType == FileTypeFBX) {
		// FBX
	}
	
	// 入力ファイルを開く
	VRETURN_VAL(
		importer->Open(option.in.c_str()),
		INVALID_RET_VAL);

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

	return 0;
}
