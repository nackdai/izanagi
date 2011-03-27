#include <stdafx.h>
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
		CString cmd(argv[i]);

		if (i < argc - 1) {
			if (result = (cmd == _T("-I"))) {
				// -I
				CString tmp;
				tmp.Format(_T("%s"), argv[i + 1]);

				includes.push_back(tmp);

				i++;
			}
			else if (result = (cmd == _T("-D"))) {
				// -D
				CString tmp;
				tmp.Format(_T("%s"), argv[i + 1]);

				defines.push_back(tmp);

				i++;
			}
			else if (result = (cmd == _T("-obj"))) {
				// -obj
				obj_dir.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == _T("-src"))) {
				// -src
				in_file.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == _T("-out_tmp"))) {
				// -out_tmp
				out_file.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == _T("-E"))) {
				// -E
				in_file.Format(_T("%s"), argv[i + 1]);
				i++;

				isPreproc = TRUE;
			}
			else if (result = (cmd == _T("-o"))) {
				// -o
				export_dir.Format(_T("%s"), argv[i + 1]);
				i++;
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
			ASSERT(FALSE);
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
	if (in_file.IsEmpty()) {
		// シェーダファイルが空
		// TODO

		return FALSE;
	}
	else if (!izanagi::izanagi_tk::CFileUtility::IsExist(in_file)) {
		// ファイルの存在を確認
		// TODO

		return FALSE;
	}

	if (IsPreproc()) {
		if (out_file.IsEmpty()) {
			// 出力ファイルが空
			// TODO

			return FALSE;
		}
		else if (izanagi::izanagi_tk::CFileUtility::IsReadOnly(out_file)) {
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
	if (!in_file.IsEmpty()) {
		VRETURN(
			izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
				in_file,
				s_BUF,
				sizeof(s_BUF)));
	}

#if 0
	// 出力ファイル
	if (out_file.IsEmpty() && !in_file.IsEmpty()) {
		FILL_ZERO(s_BUF, sizeof(s_BUF));

		// 出力ファイルが空
		memcpy(s_BUF, in_file, min(sizeof(s_BUF), strlen(in_file)));

		// ファイル名取得
		LPSTR file_name = PathFindFileName(s_BUF);

		// 拡張子削除
		PathRemoveExtension(file_name);

		out_file.Format(_T("%s.shd"), file_name);
	}
#endif

	// プリプロセス済みファイル
	if (IsPreproc() && !out_file.IsEmpty()) {
		preproc_file = out_file;
	}
	else if (!in_file.IsEmpty()) {
		FILL_ZERO(s_BUF, sizeof(s_BUF));

		// 出力ファイルが空
		memcpy(s_BUF, in_file, min(sizeof(s_BUF), strlen(in_file)));

		// ファイル名取得
		CString file_name(
			izanagi::izanagi_tk::CFileUtility::GetFileNameFromPath(s_BUF));

		CString ext(
			izanagi::izanagi_tk::CFileUtility::GetExtension(
				s_BUF,
				sizeof(s_BUF),
				file_name));

		// 拡張子削除
		VRETURN(
			izanagi::izanagi_tk::CFileUtility::RemoveExtension(
				s_BUF,
				sizeof(s_BUF),
				file_name));

		if (obj_dir.IsEmpty()) {
			preproc_file.Format(_T("%s.%s_"), s_BUF, ext);
		}
		else {
			CString tmp(s_BUF);

			// 中間ディレクトリに出力する
			VRETURN(
				izanagi::izanagi_tk::CFileUtility::CombinePath(
					s_BUF, 
					sizeof(s_BUF),
					obj_dir,
					tmp));

			preproc_file.Format(_T("%s.%s_"), s_BUF, ext);
		}
	}

	// includeパスの環境変数の展開
	std::vector<CString>::iterator it = includes.begin();
	while (it != includes.end()) {
		CString& str = *it;
		
		VRETURN(
			izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
				str,
				s_BUF,
				sizeof(s_BUF)));
		
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
		izanagi::izanagi_tk::CFileUtility::GetExeModuleName(
			s_BUF,
			sizeof(s_BUF)));

	// 自分自身をプリプロセス処理モードで呼び出す
	ret = ExecWithPreprocMode(s_BUF, *this);

	if (!ret) {
		// 失敗・・・
		ASSERT(FALSE);

		// TODO

		goto __EXIT__;
	}

	this->in_file = this->preproc_file;
	this->preproc_file.Empty();

	isPreproc = FALSE;

__EXIT__:
	return ret;
}
