// AnimationConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
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
			"   fbx : FBXファイル入力\n"
			" -idx [idx] : 複数モーションが含まれている場合にどのモーションを出力するか指定\n"
			"              指定がない場合は常に0となる\n"
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

	return 0;
}

