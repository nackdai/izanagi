#include <shlwapi.h>
#include "Option.h"

namespace {
	// オプションで共有するバッファ
	CHAR s_BUF[1024]; 
}

/**
* 解析
*/
IZ_BOOL COption::Analysis(int argc, TCHAR* argv[])
{
	for (int i = 1; i < argc; i++) {
		BOOL result = FALSE;
		izanagi::izanagi_tk::CString cmd(argv[i]);

		if (i < argc - 1) {
			if (result = (cmd == "-i")) {
				// -i
				in.format("%s", argv[++i]);
				in.replace('/', '\\');
			}
			else if (result = (cmd == "-o")) {
				// -o
				out.format("%s", argv[++i]);
			}
		}

		if (!result) {
			// 引数無しオプション
		}

		if (!result) {
			// TODO
			printf("無効なオプションです[%s]\n\n", cmd);
			//IZ_ASSERT(FALSE);
			return FALSE;
		}
	}

	// 後処理
	if (!AfterAnalysis()) {
		IZ_ASSERT(FALSE);
		return FALSE;
	}

	return TRUE;
}

IZ_BOOL COption::IsValid() const
{
	return (!out.empty() && !in.empty());
}

// 解析の後処理
IZ_BOOL COption::AfterAnalysis()
{
	memset(s_BUF, 0, sizeof(s_BUF));

	BOOL ret = TRUE;

	if (out.empty() && !in.empty()) {
		// 出力ファイルが設定されていない
		// 入力ファイルを基に出力ファイル名を決める
		memcpy(s_BUF, in, min(sizeof(s_BUF), strlen(in)));

		// ファイル名取得
		LPSTR file_name = PathFindFileName(s_BUF);

		// 拡張子削除
		PathRemoveExtension(file_name);

		out.format("%s.png", file_name);
		memset(s_BUF, 0, sizeof(s_BUF));

		ret = TRUE;
	}

	return ret;
}
