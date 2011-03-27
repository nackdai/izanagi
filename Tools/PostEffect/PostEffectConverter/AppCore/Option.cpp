#include <stdafx.h>
#include "shlwapi.h"
#include "Option.h"
#include "Preproc.h"

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
	isCompileAsm = FALSE;
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
			if (result = (cmd == "-c")) {
				// -c
				compile.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-co")) {
				// -co
				compile_opt.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-I")) {
				// -I
				CString tmp;
				tmp.Format(_T("%s"), argv[i + 1]);

				includes.push_back(tmp);

				i++;
			}
			else if (result = (cmd == "-D")) {
				// -D
				CString tmp;
				tmp.Format(_T("%s"), argv[i + 1]);

				defines.push_back(tmp);

				i++;
			}
			else if (result = (cmd == "-obj")) {
				// -obj
				obj_dir.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-src")) {
				// -src
				shader.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-o")) {
				// -o
				out_file.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-E")) {
				// -E
				shader.Format(_T("%s"), argv[i + 1]);
				i++;

				isPreproc = TRUE;
			}
			else if (result = (cmd == "-l")) {
				// -l
				option_file.Format(_T("%s"), argv[i + 1]);
				i++;
			}
		}

		if (!result) {
			// 引数無しオプション
			if (result = (cmd == "-asm")) {
				// -asm
				isCompileAsm = TRUE;
			}
		}

		if (!result) {
			// TODO
			printf("無効なオプションです[%s]\n\n", cmd);
			//ASSERT(FALSE);
			return FALSE;
		}
	}

	if (option_file.IsEmpty()) {
		// 設定ファイルが指定されていない
		AnalysisInternal();
	}
	else {
		std::vector<CString> tvArgsList;

		// 設定ファイル解析
		if (AnalysisOptionFile(tvArgsList)) {
			if (!tvArgsList.empty()) {
				// うーん・・・
				std::vector<char*> args_tmp;
				{
					// 最初にはダミーを入れる
					args_tmp.push_back("");

					std::vector<CString>::iterator it = tvArgsList.begin();
					while (it != tvArgsList.end()) {
						const char* p = *it;
						args_tmp.push_back(const_cast<char*>(p));
						it++;
					}
				}

				// 再度解析
				if (!Analysis((int)args_tmp.size(), &args_tmp[0])) {
					ASSERT(FALSE);

					// TODO

					return FALSE;
				}
			}
			else {
				// 設定ファイルが空て・・・
				ASSERT(FALSE);

				// TODO

				return FALSE;
			}
		}
	}

	return TRUE;
}

namespace {
	// 読み取り専用かどうか
	inline BOOL _IsReadOnly(LPCSTR lpszPathName)
	{
		HANDLE hFind;
		WIN32_FIND_DATA fd;

		// ファイル検索
		hFind = FindFirstFile(lpszPathName, &fd);

		if(hFind != INVALID_HANDLE_VALUE) {
			if (fd.dwFileAttributes & FILE_ATTRIBUTE_READONLY) {
				return TRUE;
			}
		}

		return FALSE;
	}
}

/**
* オプションの正当性チェック
*/
BOOL COption::IsValid()
{
	if (shader.IsEmpty()) {
		// シェーダファイルが空
		// TODO

		return FALSE;
	}
	else if (!PathFileExists(shader)) {
		// ファイルの存在を確認
		// TODO

		return FALSE;
	}

	if (!IsPreproc()) {
		// プリプロセスモードのときは調べない
		if (compile.IsEmpty()) {
			// コンパイルコマンドが空
			// TODO

			return FALSE;
		}
	}

	if (out_file.IsEmpty()) {
		// 出力ファイルが空
		// TODO

		return FALSE;
	}
	else if (_IsReadOnly(out_file)) {
		// 出力ファイルが上書き不可
		// TODO

		return FALSE;
	}

	return TRUE;
}

#if 0
#define ENABLE_LINEAR_SAMPLE	"__ENABLE_LINEAR_SAMPLE__"
#endif
#define Z_RGB				"__Z_RGB__"
#define Z_GBA				"__Z_GBA__"

namespace {
	inline void _DeleteString(
		std::vector<CString>& tvList,
		LPCSTR lpszStr)
	{
		std::vector<CString>::iterator it = tvList.begin();
		while (it != tvList.end()) {
			if (*it == lpszStr) {
				it = tvList.erase(it);
				break;
			}
			else {
				it++;
			}
		}
	}

	inline void _PushString(
		std::vector<CString>& tvList,
		LPCSTR lpszStr)
	{
		CString str(_T(lpszStr));
		tvList.push_back(str);
	}
}	// namespace

namespace {
	// 環境変数が有効かどうか
	inline BOOL _IsValidEnvironmentVariable(LPCSTR lpszName)
	{
		DWORD size = GetEnvironmentVariable(lpszName, s_BUF, sizeof(lpszName));
		return (size > 0);
	}

	// 環境変数の展開
	inline BOOL _ExpandEnvironmentStrings(LPCSTR lpszStr)
	{
		BOOL ret = FALSE;

		if (strlen(lpszStr) > 0) {
			DWORD size = ExpandEnvironmentStrings(lpszStr, s_BUF, sizeof(s_BUF));
			ret = (size > 0);

			if (!ret) {
				// TODO
			}
		}

		return ret;
	}
}	// namespace

/**
* オプション解析内部処理
*/
void COption::AnalysisInternal()
{
	memset(s_BUF, 0, sizeof(s_BUF));

	// 環境変数の展開
	if (!shader.IsEmpty()) {
		if (_ExpandEnvironmentStrings(shader)) {
			shader.Format(_T("%s"), s_BUF);
			shader.Replace('\\', '/');
			memset(s_BUF, 0, sizeof(s_BUF));
		}
	}

	if (compile.IsEmpty()) {
		compile.Format(_T("%%DXSDK_DIR%%Utilities/Bin/x86/fxc"));
	}

	if (!compile.IsEmpty()) {
		if (_ExpandEnvironmentStrings(compile)) {
			compile.Format(_T("%s"), s_BUF);
			compile.Replace('\\', '/');
			memset(s_BUF, 0, sizeof(s_BUF));
		}

		// コンパイルオプションとの結合
		CString tmp;
		tmp.Format(_T("\"\"%s\"\" %s"), compile, compile_opt);
		compile = tmp;
	}

	// 出力ファイル
	if (out_file.IsEmpty() && !shader.IsEmpty()) {
		// 出力ファイルが空
		memcpy(s_BUF, shader, min(sizeof(s_BUF), strlen(shader)));

		// ファイル名取得
		LPSTR file_name = PathFindFileName(s_BUF);

		// 拡張子削除
		PathRemoveExtension(file_name);

		out_file.Format(_T("%s.pes"), file_name);
		memset(s_BUF, 0, sizeof(s_BUF));
	}

	// プリプロセス済みファイル
	if (!shader.IsEmpty()) {
		// 出力ファイルが空
		memcpy(s_BUF, shader, min(sizeof(s_BUF), strlen(shader)));

		// ファイル名取得
		LPSTR file_name = PathFindFileName(s_BUF);

		// 拡張子削除
		PathRemoveExtension(file_name);

		if (obj_dir.IsEmpty()) {
			preproc_file.Format(_T("%s.fx_"), file_name);
		}
		else {
			// 中間ディレクトリに出力する
			PathCombine(
				s_BUF,
				obj_dir,
				file_name);

			preproc_file.Format(_T("%s.fx_"), s_BUF);
		}

		memset(s_BUF, 0, sizeof(s_BUF));
	}

	// includeパスの環境変数の展開
	std::vector<CString>::iterator it = includes.begin();
	while (it != includes.end()) {
		CString& str = *it;
		
		if (_ExpandEnvironmentStrings(str)) {
			str.Format(_T("%s"), s_BUF);
			str.Replace('\\', '/');
			memset(s_BUF, 0, sizeof(s_BUF));
		}
		
		it++;
	}

	if (!IsPreproc()) {
		// TODO
		// デフォルトincludeパスの追加
	}
}

/**
* コピー
*/
void COption::Copy(const COption& rhs)
{
	// 一応
	Clear();

	compile = rhs.compile;

	obj_dir = rhs.obj_dir;

	shader = rhs.shader;
	preproc_file = rhs.preproc_file;
	out_file = rhs.out_file;

	defines.insert(defines.end(), rhs.defines.begin(), rhs.defines.end());
	includes.insert(includes.end(), rhs.includes.begin(), rhs.includes.end());
	
	isPreproc = rhs.isPreproc;

	option_file = rhs.option_file;
}

/**
* クリア
*/
void COption::Clear()
{
	compile.Empty();
	obj_dir.Empty();
	shader.Empty();
	preproc_file.Empty();
	out_file.Empty();

	defines.clear();
	includes.clear();

	isPreproc = FALSE;

	option_file.Empty();
}

namespace {
	// テンポラリファイル名を作成する
	inline void _CreateTmpFileName(CString& strFile)
	{
		// システム時間取得
		SYSTEMTIME time;
		GetLocalTime(&time);

		strFile.Format(
			_T("__tmp_%d_%d_%d_%d_%d_%d__"),
			time.wYear,
			time.wMonth,
			time.wDay,
			time.wHour,
			time.wMinute,
			time.wSecond);
	}

	// exeの名前を取得
	inline void _GetExeName(CString& strExe)
	{
		// exeの場所を取得
		static char BUF[MAX_PATH];
		GetModuleFileName(NULL, BUF, MAX_PATH);
		//strExe.Format(_T("%s"), PathFindFileName(BUF));
		strExe.Format(_T("%s"), BUF);
	}

	// オプションを表している文字列かどうか
	inline BOOL _IsOptionString(LPCSTR str)
	{
		// NOTE
		// 先頭が'-'かどうかで判定

		// 先頭にある空白は除く
		UINT pos = 0;
		while (str[pos] == ' ') {
			pos++;
		}

		return (str[pos] == '-');
	}

	// 文字列を分解
	void _BreakString(
		LPCSTR str,
		std::vector<CString>& tvRet)
	{
		UINT len = (UINT)strlen(str);
		UINT pos = 0;
		BOOL bIsQuotationArea = FALSE;	// ""で囲まれている部分かどうか

		for (UINT i = 0; i < len; i++) {
			CHAR c = str[i];

			if (((c == ' ') && !bIsQuotationArea)
				|| (c == '\n'))
			{
				s_BUF[pos] = '\0';

				CString str;
				str.Format(_T("%s"), s_BUF);

				tvRet.push_back(str);

				pos = 0;
			}
			else if (c == '\"') {
				if (str[i - 1] != '\\') {
					//「"」の前の文字が「\」でなければ、文字としての「"」でない
					bIsQuotationArea = !bIsQuotationArea;
				}
				else {
					s_BUF[pos++] = c;
				}
			}
			else {
				s_BUF[pos++] = c;
			}
		}
	}
}	// namespace

/**
* オプションファイル解析
*/
BOOL COption::AnalysisOptionFile(std::vector<CString>& tvArgs)
{
	BOOL ret = TRUE;

	FILE* fp = NULL;

	// テンポラリファイル名を作成する
	CString strTmp;
	_CreateTmpFileName(strTmp);
		
	// プリプロセスする
	{
		// プリプロセス用
		COption cOpt;
		cOpt.Copy(*this);
		{
			// プリプロセス用にいろいろ変更・・・
			cOpt.shader = cOpt.option_file;		// 入力
			cOpt.preproc_file = strTmp;			// 出力
		}

		// exe名
		CString strExe;
		_GetExeName(strExe);

		// 自分自身をプリプロセス処理モードで呼び出す
		ret = ExecWithPreprocMode(strExe, cOpt);

		if (!ret) {
			// 失敗・・・
			ASSERT(FALSE);

			// TODO

			goto __EXIT__;
		}
	}

	// オプションを引数リストに戻す
	ConvetOptionToArgs(tvArgs);

	// ファイル開く
	fopen_s(&fp, strTmp, "rt");
	if (fp == NULL) {
		ASSERT(FALSE);

		// TODO

		return FALSE;
	}

	memset(s_BUF, 0, sizeof(s_BUF));

	// リストファイルを解析
	while (fgets(s_BUF, sizeof(s_BUF), fp) != NULL) {
		CString str;
		str.Format(_T("%s"), s_BUF);

		if (strlen(str) > 0) {
			if (_IsOptionString(str)) {
				// オプションを表している文字列
				std::vector<CString> tvTmp;

				_BreakString(str, tvTmp);

				// リストに登録
				tvArgs.insert(tvArgs.end(), tvTmp.begin(), tvTmp.end());
			}
		}

		// 次に向けてクリア
		memset(s_BUF, 0, sizeof(s_BUF));
	}

__EXIT__:
	// ファイル閉じる
	if (fp != NULL) {
		fclose(fp);
	}

	// テンポラリファイルを削除
	DeleteFile(strTmp);

	// クリアする
	Clear();
	
	return ret;
}

/**
* オプションを引数リストに戻す
*/
void COption::ConvetOptionToArgs(std::vector<CString>& tvArgs)
{
	// コンパイルコマンド
	if (!compile.IsEmpty()) {
		tvArgs.push_back(CString("-c"));
		tvArgs.push_back(compile);
	}

	// コンパイルオプション
	if (!compile_opt.IsEmpty()) {
		tvArgs.push_back(CString("-co"));
		tvArgs.push_back(compile_opt);
	}

	// 中間ファイルディレクトリ
	if (!obj_dir.IsEmpty()) {
		tvArgs.push_back(CString("-obj"));
		tvArgs.push_back(obj_dir);
	}

	// コンパイルするシェーダ
	if (!shader.IsEmpty()) {
		tvArgs.push_back(CString("-src"));
		tvArgs.push_back(shader);
	}

	// 出力ファイル
	if (!out_file.IsEmpty()) {
		tvArgs.push_back(CString("-o"));
		tvArgs.push_back(out_file);
	}

	// アセンブラ表示のためのコンパイルをするかどうか
	if (isCompileAsm) {
		tvArgs.push_back(CString("-asm"));
	}
}
