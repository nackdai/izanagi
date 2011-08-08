#include <stdafx.h>
#include <shlwapi.h>
#include "Option.h"

namespace {
	// オプションで共有するバッファ
	CHAR s_BUF[1024]; 
}

/**
* 解析
*/
BOOL COption::Analysis(int argc, TCHAR* argv[])
{
	for (int i = 1; i < argc; i++) {
		BOOL result = FALSE;
		CString cmd(argv[i]);

		if (i < argc - 1) {
			if (result = (cmd == _T("-i"))) {
				// -i
				in.Format(_T("%s"), argv[++i]);
				in.Replace("/", "\\");
			}
			else if (result = (cmd == _T("-o"))) {
				// -o
				out.Format(_T("%s"), argv[++i]);
			}
		}

		if (!result) {
			// 引数無しオプション
		}

		if (!result) {
			// TODO
			printf("無効なオプションです[%s]\n\n", cmd);
			//ASSERT(FALSE);
			return FALSE;
		}
	}

	// 後処理
	if (!AfterAnalysis()) {
		ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

BOOL COption::IsValid() const
{
	return (!out.IsEmpty() && !in.IsEmpty());
}

// 解析の後処理
BOOL COption::AfterAnalysis()
{
	memset(s_BUF, 0, sizeof(s_BUF));

	BOOL ret = TRUE;

	if (out.IsEmpty() && !in.IsEmpty()) {
		// 出力ファイルが設定されていない
		// 入力ファイルを基に出力ファイル名を決める
		memcpy(s_BUF, in, min(sizeof(s_BUF), strlen(in)));

		// ファイル名取得
		LPSTR file_name = PathFindFileName(s_BUF);

		// 拡張子削除
		PathRemoveExtension(file_name);

		out.Format(_T("%s.img"), file_name);
		memset(s_BUF, 0, sizeof(s_BUF));

		ret = TRUE;
	}

	return ret;
}
