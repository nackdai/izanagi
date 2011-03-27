#include <stdafx.h>
#include <shlwapi.h>
#include <direct.h>
#include "Option.h"

namespace {
	// オプションで共有するバッファ
	CHAR s_BUF[1024]; 
}

// コンストラクタ
COption::COption()
{
	fontSize = DEFAULT_FONT_SIZE;

	texWidth = DEFAULT_TEX_WIDTH;
	texHeight = DEFAULT_TEX_HEIGHT;

	typeAA = GGO_BITMAP;

	charEncode = izanagi::E_FONT_CHAR_ENCODE_SJIS;

	fontFace.Format("%s", _T("ＭＳ 明朝"));

	isFixedPitch = FALSE;
	isBold = FALSE;
	isItalic = FALSE;
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
				list_file.Format(_T("%s"), argv[++i]);
			}
			else if (result = (cmd == _T("-o"))) {
				// -o
				out_file.Format(_T("%s"), argv[++i]);
			}
			else if (result = (cmd == _T("-fs"))) {
				// -fs
				fontSize = atoi(argv[++i]);
			}
			else if (result = (cmd == _T("-w"))) {
				// -w
				texWidth = atoi(argv[++i]);
			}
			else if (result = (cmd == _T("-h"))) {
				// -h
				texHeight = atoi(argv[++i]);
			}
			else if (result = (cmd == _T("-code"))) {
				// -c
				CString tmp(argv[++i]);

				if (tmp.CompareNoCase(_T("SJIS")) == 0) {
					charEncode = izanagi::E_FONT_CHAR_ENCODE_SJIS;
				}
				else if ((tmp.CompareNoCase(_T("UTF16")) == 0)
						|| (tmp.CompareNoCase(_T("UNICODE")) == 0))
				{
					charEncode = izanagi::E_FONT_CHAR_ENCODE_UTF16;
				}
				else if (tmp.CompareNoCase(_T("UTF8")) == 0) {
					charEncode = izanagi::E_FONT_CHAR_ENCODE_UTF8;
				}
				else {
					result = FALSE;
				}
			}
			else if (result = (cmd == _T("-ft"))) {
				// -ft
				fontFace.Format(_T("%s"), argv[++i]);
			}
			else if (result = (cmd == _T("-aa"))) {
				// -aa
				// アンチエイリアス
				INT num = atoi(argv[++i]);
				switch (num) {
					case 0:
						typeAA = GGO_BITMAP;
						break;
					case 2:
						typeAA = GGO_GRAY2_BITMAP;
						break;
					case 4:
						typeAA = GGO_GRAY4_BITMAP;
						break;
					case 8:
						typeAA = GGO_GRAY8_BITMAP;
						break;
					default:
						result = FALSE;
						break;
				}
			}
		}

		if (!result) {
			// 引数無しオプション
			if (result = (cmd == _T("-fixed_pitch"))) {
				// -fixed_pitch
				isFixedPitch = TRUE;
			}
			else if (result = (cmd == _T("-bold"))) {
				// -bold
				isBold = TRUE;
			}
			else if (result = (cmd == _T("-italic"))) {
				// -italic
				isItalic = TRUE;
			}
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

// 解析の後処理
BOOL COption::AfterAnalysis()
{
	memset(s_BUF, 0, sizeof(s_BUF));

	BOOL ret = FALSE;

	if (!out_file.IsEmpty()) {
		ret = TRUE;
	}
	else if (out_file.IsEmpty() && !list_file.IsEmpty()) {
		// 出力ファイルが設定されていない
		// 入力ファイルを基に出力ファイル名を決める
		memcpy(s_BUF, list_file, min(sizeof(s_BUF), strlen(list_file)));

		// ファイル名取得
		LPSTR file_name = PathFindFileName(s_BUF);

		// 拡張子削除
		PathRemoveExtension(file_name);

		out_file.Format(_T("%s.fnt"), file_name);
		memset(s_BUF, 0, sizeof(s_BUF));

		ret = TRUE;
	}

	return ret;
}

/**
* 正当性チェック
*/
BOOL COption::IsValid()
{
	if (list_file.IsEmpty()) {
		printf("リストファイルが指定されていません\n");
		return FALSE;
	}

	if ((MIN_FONT_SIZE > fontSize) || (fontSize > MAX_FONT_SIZE)) {
		printf("フォントサイズが不正値です\n");
		return FALSE;
	}

	if ((MIN_TEX_WIDTH > texWidth) || (texWidth > MAX_TEX_WIDTH)) {
		printf("テクスチャ幅が不正値です\n");
		return FALSE;
	}

	if ((MIN_TEX_HEIGHT > texHeight) || (texHeight > MAX_TEX_HEIGHT)) {
		printf("テクスチャ高さが不正値です\n");
		return FALSE;
	}

	return TRUE;
}

/**
* リストファイルを解析する
*/
BOOL COption::AnalysisListFile()
{
	// TODO
	// 将来的にはリストファイルはXMLにする
	FILE* pFile = NULL;
	fopen_s(&pFile, list_file, "rt");
	if (pFile == NULL) {
		printf("リストファイルが存在しません [%s]\n", list_file);
	}

	// リストファイルのフルパスを求める
	static TCHAR BASE_PATH[_MAX_PATH];
	PathSearchAndQualify(
		list_file,
		BASE_PATH,
		sizeof(BASE_PATH));

	// 現在のディレクトリを取得
	static TCHAR CUR_DIR[_MAX_PATH];
	GetCurrentDirectory(sizeof(CUR_DIR), CUR_DIR);

	// ファイル名部分を取り除く
	PathRemoveFileSpec(BASE_PATH);

	// ディレクトリを移動
	if (_chdir(BASE_PATH) != 0) {
		ASSERT(FALSE);
		return FALSE;
	}

	static const UINT BUF_SIZE = 1024;
	static TCHAR BUF[BUF_SIZE];

	memset(BUF, 0, BUF_SIZE);

	while (fgets(BUF, sizeof(BUF), pFile) != NULL) {
		// パスの前後の空白を取り除く
		PathRemoveBlanks(BUF);

		CString str;
		str.Format(_T("%s"), BUF);

		if (!str.IsEmpty()) {
			// フルパス取得
			BOOL result = (_fullpath(BUF, str, BUF_SIZE) != NULL);
			ASSERT(result);

			if (result) {
				// 登録
				str.Format(_T("%s"), BUF);
				in_file_list.push_back(str);
			}
		}

		memset(BUF, 0, BUF_SIZE);
	}

	// 元のディレクトリに移動
	if (_chdir(CUR_DIR) != 0) {
		ASSERT(FALSE);
	}

	BOOL ret = (in_file_list.size() > 0);
	if (!ret) {
		printf("リストファイルに記述が存在しません\n");
	}

	return ret;
}
