// ArchiveBuilder.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "ArchiveBuilder.h"

#include "FileTree.h"
#include "ArchiveBuilderImpl.h"
#include "FileIdCreator.h"
#include "Option.h"
#include "AppError.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一のアプリケーション オブジェクトです。

CWinApp theApp;

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

#define VGOTO(b)	if (!(b)) { ASSERT(FALSE); nRetCode = 1; goto __EXIT__; }

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	// MFC を初期化して、エラーの場合は結果を印刷します。
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 必要に応じてエラー コードを変更してください。
		_tprintf(_T("致命的なエラー: MFC の初期化ができませんでした。\n"));
		nRetCode = 1;
	}
	else
	{
		// エラーストリング表示用関数セット
		izanagi::izanagi_tk::CException::SetPrintLogFunc(_PrintString);

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
		catch (izanagi::izanagi_tk::CException* e) {
			e->PrintLog();
			VGOTO(FALSE);
		}
		catch (...) {
			printf("Failed\n");
			VGOTO(FALSE);
		}

__EXIT__:
		// For compile error.
		// Nothing is done.
		int x = 0;
	}

	return nRetCode;
}
