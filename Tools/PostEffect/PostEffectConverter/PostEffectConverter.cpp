// PostEffectConverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "PostEffectConverter.h"

#include <vector>
#include "shlwapi.h"
#include "Option.h"
#include "PostEffectConverterImpl.h"
#include "Preproc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 唯一のアプリケーション オブジェクトです。

//CWinApp theApp;

using namespace std;

namespace {
	void _DispUsage()
	{
		printf(
			"PostEffectConverter Version 0.01β\n"
			"\n"
			"ポストエフェクトシェーダコンバータ\n"
			"\n"
			"USAGE : PostEffectConverter [options]\n"
			"\n"
			"[Options]\n"
			" -l   [in]     : 設定ファイル指定\n"
			" -src [in]     : ポストエフェクトシェーダファイル指定\n"
			"\n"
			" -c  [exe]     : シェーダコンパイラ指定\n"
			"                 指定が無い場合は自動で設定されます。\n"
			"                \"%%DXSDK_DIR%%Utilities/Bin/x86/fxc\"\n"
			" -co [opt]     : シェーダコンパイラに渡すオプション\n"
			" -obj [dir]    : 中間ファイル出力ディレクトリ\n"
			"                 ディレクトリが存在しない場合は自動で作成します。\n"
			" -o [out]      : 出力ファイル\n"
			"                 指定がない場合は、-srcオプションで指定されたファイル名を\n"
			"                 基に出力ファイルを自動で作成します。\n"
			" -asm          : アセンブラコード出力\n"
			"                 -objファイルで指定したディレクトリに出力されます。\n"
			"\n"
			" -I [include]  : インクルードパス指定\n"
			" -D [define]   : 定義指定\n"
			"\n"
			"[注意]\n"
			" *-lと-srcを同時指定した場合は、-lが優先されます。\n"
			" *-I、-D は オプションによって適用のされ方が異なります。\n"
			"    -l   : 設定ファイルに対しての指定。\n"
			"    -src : ポストエフェクトシェーダファイルに対しての指定。\n"
		);
	}

	// 存在するかどうか
	inline BOOL _IsExist(LPCSTR lpszName)
	{
		HANDLE hFind;
		WIN32_FIND_DATA fd;

		// ファイル検索
		hFind = FindFirstFile(lpszName, &fd);

		return (hFind != INVALID_HANDLE_VALUE);
	}
}

int _tmain(int argc, TCHAR* argv[], TCHAR* envp[])
{
	int nRetCode = 0;

	COption cOption;

	// MFC を初期化して、エラーの場合は結果を印刷します。
	if (!AfxWinInit(::GetModuleHandle(NULL), NULL, ::GetCommandLine(), 0))
	{
		// TODO: 必要に応じてエラー コードを変更してください。
		_tprintf(_T("致命的なエラー: MFC の初期化ができませんでした。\n"));
		nRetCode = 1;
	}
	else
	{
		try {
			if (!cOption.Analysis(argc, argv)) {
				// オプション解析に失敗
				nRetCode = 1;
				goto __EXIT__;
			}

			if (!cOption.IsValid()) {
				// オプションが不正
				nRetCode = 1;
				goto __EXIT__;
			}

			// 中間ファイル出力ディレクトリの指定がある
			if (!cOption.obj_dir.IsEmpty()) {
				if (!_IsExist(cOption.obj_dir)) {
					// 存在しないので作成する
					CreateDirectory(cOption.obj_dir, NULL);
				}
			}

			if (cOption.IsPreproc()) {
				// プリプロセス処理のみを行う
				nRetCode = Preproc(cOption);
				goto __EXIT__;
			}

			// 自分自身をプリプロセス処理モードで呼び出す
			if (!ExecWithPreprocMode(argv[0], cOption)) {
				nRetCode = 1;
				goto __EXIT__;
			}

			if (!CPostEffectConverter::GetInstance().Begin(cOption.preproc_file)) {
				nRetCode = 1;
				goto __EXIT__;
			}

			// シェーダコンパイル
			if (!CPostEffectConverter::GetInstance().CompileShader(
					cOption.isCompileAsm,
					cOption.compile,
					cOption.preproc_file,
					cOption.obj_dir))
			{
				nRetCode = 1;
				goto __EXIT__;
			}

#if 1
			if (!CPostEffectConverter::GetInstance().Export(cOption.out_file)) {
				nRetCode = 1;
				goto __EXIT__;
			}
#endif
		}
		catch (...) {
			printf("予期しないエラーが発生しました\n");
			nRetCode = 1;
		}
	}
__EXIT__:
	return nRetCode;
}
