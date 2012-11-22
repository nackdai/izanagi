#include <shlwapi.h>
#include "Option.h"

namespace {
	// オプションで共有するバッファ
	CHAR s_BUF[1024]; 
}

COption::COption()
{
}

// 解析
BOOL COption::Analysis(int argc, char* argv[])
{
	for (int i = 1; i < argc; i++) {
		BOOL result = FALSE;
		izanagi::tool::CString cmd(argv[i]);

		if (i < argc - 1) {
			if (result = (cmd == "-i")) {
				// -i
				in_dir.format("%s", argv[++i]);
			}
			else if (result = (cmd == "-o")) {
				// -o
				output.format("%s", argv[++i]);
			}
			else if (result = (cmd == "-h")) {
				// -h
				file_id.format("%s", argv[++i]);
			}
			else if (result = (cmd == "-obj")) {
				// -obj
				obj_dir.format("%s", argv[++i]);
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

#ifndef COUNTOF
#define COUNTOF(a)	(sizeof(a) / sizeof(a[0]))
#endif

// 解析の後処理
BOOL COption::AfterAnalysis()
{
	memset(s_BUF, 0, sizeof(s_BUF));

	// ラベル
	if (in_dir.empty()) {
		// 適当に決める

		// システム時間取得
		SYSTEMTIME time;
		GetLocalTime(&time);

		label.format(
			"__tmp_%d_%d_%d_%d_%d_%d__",
			time.wYear,
			time.wMonth,
			time.wDay,
			time.wHour,
			time.wMinute,
			time.wSecond);
	}
	else {
		// 入力ディレクトリを基に出力ファイル名を決める
		LPCSTR p = in_dir;
		size_t pos = strlen(p);
		size_t cnt = COUNTOF(s_BUF);

		// バッファを超えないようにする
		pos = (pos >= cnt - 1 ? pos - cnt - 1 : 0);

		sprintf_s(s_BUF, "%s", p + pos);

		// PathFindFileNameが動くように
		// パス末尾のデリミタを取り除く
		if ((s_BUF[strlen(s_BUF)] == '/')
			|| (s_BUF[strlen(s_BUF)] == '\\'))
		{
			s_BUF[strlen(s_BUF)] = '\0';
		}

		p = PathFindFileName(s_BUF);

		label.format("%s", p);
	}

	// 出力ファイル
	if (output.empty()) {
		sprintf_s(s_BUF, "%s", label);

		// ファイル名取得
		LPSTR file_name = PathFindFileName(s_BUF);

		// 拡張子削除
		PathRemoveExtension(file_name);

		output.format("%s.arc", file_name);
		memset(s_BUF, 0, sizeof(s_BUF));
	}


	// 入力ディレクトリ
	if (in_dir.empty()) {
		// 入力ディレクトリが指定されていないので
		// カレントディレクトリにする
		in_dir.format("./");
	}

	// 中間ディレクトリ
	if (obj_dir.empty()) {
		// 適当に決める

		// システム時間取得
		SYSTEMTIME time;
		GetLocalTime(&time);

		obj_dir.format(
			"__tmp_%d_%d_%d_%d_%d_%d__",
			time.wYear,
			time.wMonth,
			time.wDay,
			time.wHour,
			time.wMinute,
			time.wSecond);
	}

	return TRUE;
}
