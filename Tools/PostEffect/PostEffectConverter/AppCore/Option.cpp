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
				compiler.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-co")) {
				// -co
				compileOpt.Format(_T("%s"), argv[i + 1]);
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
				objDir.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-src")) {
				// -src
				shader.Format(_T("%s"), argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-o")) {
				// -o
				outFile.Format(_T("%s"), argv[i + 1]);
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
				optionFile.Format(_T("%s"), argv[i + 1]);
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

	if (optionFile.IsEmpty()) {
		// 設定ファイルが指定されていない
		if (!AnalysisInternal()) {
			return FALSE;
		}
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
		if (compiler.IsEmpty()) {
			// コンパイルコマンドが空
			// TODO

			return FALSE;
		}
	}

	if (outFile.IsEmpty()) {
		// 出力ファイルが空
		// TODO

		return FALSE;
	}
	else if (izanagi::izanagi_tk::CFileUtility::IsReadOnly(outFile)) {
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

/**
* オプション解析内部処理
*/
BOOL COption::AnalysisInternal()
{
	memset(s_BUF, 0, sizeof(s_BUF));

	// 環境変数の展開
	if (!shader.IsEmpty()) {
		if (izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
				s_BUF,
				sizeof(s_BUF),
				shader))
		{
			shader.Format(_T("%s"), s_BUF);
			shader.Replace('\\', '/');
			memset(s_BUF, 0, sizeof(s_BUF));
		}
	}

	if (compiler.IsEmpty()) {
		// コンパイラの指定がないので、デフォルトを指定
		compiler.Format(_T("%%DXSDK_DIR%%Utilities/Bin/x86/fxc"));
	}

	if (!compiler.IsEmpty()) {
		if (izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
				s_BUF,
				sizeof(s_BUF),
				compiler))
		{
			compiler.Format(_T("%s"), s_BUF);
			compiler.Replace('\\', '/');
			memset(s_BUF, 0, sizeof(s_BUF));
		}

		// コンパイルオプションとの結合
		CString tmp;
		tmp.Format(_T("\"\"%s\"\" %s"), compiler, compileOpt);
		compiler = tmp;
	}

	// 出力ファイル
	if (outFile.IsEmpty() && !shader.IsEmpty()) {
		// 出力ファイルが空
		IZ_BOOL result = (::sprintf_s(s_BUF, sizeof(s_BUF), "%s\0", shader) >= 0);
		VRETURN(result);

		// ファイル名取得
		std::string file_name(izanagi::izanagi_tk::CFileUtility::GetFileNameFromPath(s_BUF));

		// 拡張子削除
		result = izanagi::izanagi_tk::CFileUtility::RemoveExtension(s_BUF, sizeof(s_BUF), file_name.c_str());
		VRETURN(result);

		outFile.Format(_T("%s.pes"), file_name);
		memset(s_BUF, 0, sizeof(s_BUF));
	}

	if (!shader.IsEmpty()) {
		// プリプロセス済みファイル
		{
			// 出力ファイルが空
			IZ_BOOL result = (::sprintf_s(s_BUF, sizeof(s_BUF), "%s\0", shader) >= 0);
			VRETURN(result);

			// ファイル名取得
			std::string file_name(izanagi::izanagi_tk::CFileUtility::GetFileNameFromPath(s_BUF));

			// 拡張子削除
			result = izanagi::izanagi_tk::CFileUtility::RemoveExtension(s_BUF, sizeof(s_BUF), file_name.c_str());
			VRETURN(result);

			if (objDir.IsEmpty()) {
				preprocFile.Format(_T("%s.fx_"), file_name);
			}
			else {
				// 中間ディレクトリに出力する
				izanagi::izanagi_tk::CFileUtility::CombinePath(
					s_BUF,
					sizeof(s_BUF),
					objDir,
					file_name.c_str());

				preprocFile.Format(_T("%s.fx_"), s_BUF);
			}

			memset(s_BUF, 0, sizeof(s_BUF));
		}

		// シェーダパス
		if (!baseDir.empty()
			&& izanagi::izanagi_tk::CFileUtility::IsRelativePath(shader))
		{
			// ベースディレクトリ相対のシェーダファイルのパスを作る
			izanagi::izanagi_tk::CFileUtility::CombinePath(
				s_BUF,
				sizeof(s_BUF),
				baseDir.c_str(), shader);
			shader.Format(_T("%s"), s_BUF);
		}
	}

	{
		std::vector<CString>::iterator it = includes.begin();
		while (it != includes.end()) {
			CString& str = *it;

			// includeパスの環境変数の展開
			if (izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					s_BUF,
					sizeof(s_BUF),
					str))
			{
				str.Format(_T("%s"), s_BUF);
				str.Replace('\\', '/');

				if (!baseDir.empty()
					&& izanagi::izanagi_tk::CFileUtility::IsRelativePath(str))
				{
					// ベースディレクトリ相対のインクルートパスを作成
					izanagi::izanagi_tk::CFileUtility::CombinePath(
						s_BUF,
						sizeof(s_BUF),
						baseDir.c_str(), str);

					str.Format(_T("%s"), s_BUF);
				}

				memset(s_BUF, 0, sizeof(s_BUF));
			}
			
			it++;
		}
	}

	if (!IsPreproc()) {
		// TODO
		// デフォルトincludeパスの追加
	}

	return TRUE;
}

/**
* コピー
*/
void COption::Copy(const COption& rhs)
{
	// 一応
	Clear();

	compiler = rhs.compiler;

	objDir = rhs.objDir;

	shader = rhs.shader;
	preprocFile = rhs.preprocFile;
	outFile = rhs.outFile;

	defines.insert(defines.end(), rhs.defines.begin(), rhs.defines.end());
	includes.insert(includes.end(), rhs.includes.begin(), rhs.includes.end());
	
	isPreproc = rhs.isPreproc;

	optionFile = rhs.optionFile;
}

/**
* クリア
*/
void COption::Clear()
{
	compiler.Empty();
	objDir.Empty();
	shader.Empty();
	preprocFile.Empty();
	outFile.Empty();

	defines.clear();
	includes.clear();

	isPreproc = FALSE;

	optionFile.Empty();
}

namespace {
	// テンポラリファイル名を作成する
	inline void _CreateTmpFileName(CString& strFile)
	{
		// システム時間取得
		SYSTEMTIME time;
		::GetLocalTime(&time);

		strFile.Format(
			_T("__tmp_%d_%d_%d_%d_%d_%d__"),
			time.wYear,
			time.wMonth,
			time.wDay,
			time.wHour,
			time.wMinute,
			time.wSecond);
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
			cOpt.shader = cOpt.optionFile;		// 入力
			cOpt.preprocFile = strTmp;			// 出力
		}

		// exe名
		izanagi::izanagi_tk::CFileUtility::GetExeModuleName(s_BUF, sizeof(s_BUF));

		// 自分自身をプリプロセス処理モードで呼び出す
		ret = ExecWithPreprocMode(s_BUF, cOpt);

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

	if (!optionFile.IsEmpty()) {
		optionFile.Replace("/", "\\");
		ret = izanagi::izanagi_tk::CFileUtility::GetPathWithoutFileName(s_BUF, sizeof(s_BUF), optionFile);
		VRETURN(ret);

		baseDir.append(s_BUF);
	}

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
	if (!compiler.IsEmpty()) {
		tvArgs.push_back(CString("-c"));
		tvArgs.push_back(compiler);
	}

	// コンパイルオプション
	if (!compileOpt.IsEmpty()) {
		tvArgs.push_back(CString("-co"));
		tvArgs.push_back(compileOpt);
	}

	// 中間ファイルディレクトリ
	if (!objDir.IsEmpty()) {
		tvArgs.push_back(CString("-obj"));
		tvArgs.push_back(objDir);
	}

	// コンパイルするシェーダ
	if (!shader.IsEmpty()) {
		tvArgs.push_back(CString("-src"));
		tvArgs.push_back(shader);
	}

	// 出力ファイル
	if (!outFile.IsEmpty()) {
		tvArgs.push_back(CString("-o"));
		tvArgs.push_back(outFile);
	}

	// アセンブラ表示のためのコンパイルをするかどうか
	if (isCompileAsm) {
		tvArgs.push_back(CString("-asm"));
	}
}
