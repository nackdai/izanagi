// ArchiveBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "FileTree.h"
#include "ArchiveBuilderImpl.h"
#include "FileIdCreator.h"
#include "Option.h"
#include "AppError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

using namespace std;

namespace {
	inline void _DispUsage()
	{
		printf(
			"ArchiveBuilder Ver.0.01\n"
			"\n"
			"[USAGE]\n"
			"ArchiveBuilder.exe [options]\n"
			"\n"
			"[options]\n"
			" -i [dir]    : 入力ディレクトリ\n"
			"               指定が無い場合はカレントディレクトリ\n"
			" -o [out]    : 出力ファイル\n"
			"               指定が無い場合は入力ディレクトリ名から自動で作成\n"
			" -h [header] : ファイルID出力ヘッダファイル\n"
			"               指定が無い場合は入力ディレクトリ名から自動で作成\n"
			"\n"
			" -obj [dir]  : 中間ファイル出力ディレクトリ\n"
			"               ディレクトリが存在しない場合は自動で作成する\n"
		);
	}

	void _PrintString(IZ_PCSTR pszStr)
	{
		printf("%s\n", pszStr);
	}
}	// namespace

#ifdef VGOTO
#undef VGOTO
#endif

#define VGOTO(b)	if (!(b)) { IZ_ASSERT(FALSE); nRetCode = 1; goto __EXIT__; }

int main(int argc, TCHAR* argv[])
{
	int nRetCode = 0;

	// エラーストリング表示用関数セット
	izanagi::tool::CException::SetPrintLogFunc(_PrintString);

	// オプション解析
	COption cOption;
	VGOTO(cOption.Analysis(argc, argv));

	// ファイルツリー登録
	CFileTree::GetInstance().RegisterFileTree(cOption.in_dir);

	try {
		// 中間ファイル出力
		CArchiveBuilderImpl::ExportObj(cOption);

		// .arcファイル出力
		CArchiveBuilderImpl::Export(cOption);

		// ファイルID出力
		CFileIdCreator::GetInstance().Export(cOption.file_id);
	}
	catch (izanagi::tool::CException* e) {
		e->PrintLog();
		VGOTO(FALSE);
	}
	catch (...) {
		printf("Failed\n");
		VGOTO(FALSE);
	}

__EXIT__:
	return nRetCode;
}
