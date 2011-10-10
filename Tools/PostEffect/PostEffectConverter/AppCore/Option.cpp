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
		izanagi::izanagi_tk::CString cmd(argv[i]);

		if (i < argc - 1) {
			if (result = (cmd == "-c")) {
				// -c
				compiler.format("%s", argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-co")) {
				// -co
				compileOpt.format("%s", argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-I")) {
				// -I
				izanagi::izanagi_tk::CString tmp;
				tmp.format("%s", argv[i + 1]);

				includes.push_back(tmp);

				i++;
			}
			else if (result = (cmd == "-D")) {
				// -D
				izanagi::izanagi_tk::CString tmp;
				tmp.format("%s", argv[i + 1]);

				defines.push_back(tmp);

				i++;
			}
			else if (result = (cmd == "-obj")) {
				// -obj
				objDir.format("%s", argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-src")) {
				// -src
				shader.format("%s", argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-o")) {
				// -o
				outFile.format("%s", argv[i + 1]);
				i++;
			}
			else if (result = (cmd == "-E")) {
				// -E
				shader.format("%s", argv[i + 1]);
				i++;

				isPreproc = TRUE;
			}
			else if (result = (cmd == "-l")) {
				// -l
				optionFile.format("%s", argv[i + 1]);
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
			//IZ_ASSERT(FALSE);
			return FALSE;
		}
	}

	if (optionFile.empty()) {
		// 設定ファイルが指定されていない
		if (!AnalysisInternal()) {
			return FALSE;
		}
	}
	else {
		std::vector<izanagi::izanagi_tk::CString> tvArgsList;

		// 設定ファイル解析
		if (AnalysisOptionFile(tvArgsList)) {
			if (!tvArgsList.empty()) {
				// うーん・・・
				std::vector<char*> args_tmp;
				{
					// 最初にはダミーを入れる
					args_tmp.push_back("");

					std::vector<izanagi::izanagi_tk::CString>::iterator it = tvArgsList.begin();
					while (it != tvArgsList.end()) {
						const char* p = *it;
						args_tmp.push_back(const_cast<char*>(p));
						it++;
					}
				}

				// 再度解析
				if (!Analysis((int)args_tmp.size(), &args_tmp[0])) {
					IZ_ASSERT(FALSE);

					// TODO

					return FALSE;
				}
			}
			else {
				// 設定ファイルが空て・・・
				IZ_ASSERT(FALSE);

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
	if (shader.empty()) {
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
		if (compiler.empty()) {
			// コンパイルコマンドが空
			// TODO

			return FALSE;
		}
	}

	if (outFile.empty()) {
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
		std::vector<izanagi::izanagi_tk::CString>& tvList,
		LPCSTR lpszStr)
	{
		std::vector<izanagi::izanagi_tk::CString>::iterator it = tvList.begin();
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
		std::vector<izanagi::izanagi_tk::CString>& tvList,
		LPCSTR lpszStr)
	{
		izanagi::izanagi_tk::CString str(lpszStr);
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
	if (!shader.empty()) {
		if (izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
				s_BUF,
				sizeof(s_BUF),
				shader))
		{
			shader.format("%s", s_BUF);
			shader.replace('\\', '/');
			memset(s_BUF, 0, sizeof(s_BUF));
		}
	}

	if (compiler.empty()) {
		// コンパイラの指定がないので、デフォルトを指定
		compiler.format("%%DXSDK_DIR%%Utilities/Bin/x86/fxc");
	}

	if (!compiler.empty()) {
		if (izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
				s_BUF,
				sizeof(s_BUF),
				compiler))
		{
			compiler.format("%s", s_BUF);
			compiler.replace('\\', '/');
			memset(s_BUF, 0, sizeof(s_BUF));
		}

		// コンパイルオプションとの結合
		izanagi::izanagi_tk::CString tmp;
		tmp.format("\"\"%s\"\" %s", compiler.c_str(), compileOpt.c_str());
		compiler = tmp;
	}

	// 出力ファイル
	if (outFile.empty() && !shader.empty()) {
		// 出力ファイルが空
		IZ_BOOL result = (::sprintf_s(s_BUF, sizeof(s_BUF), "%s\0", shader.c_str()) >= 0);
		VRETURN(result);

		// ファイル名取得
		std::string file_name(izanagi::izanagi_tk::CFileUtility::GetFileNameFromPath(s_BUF));

		// 拡張子削除
		result = izanagi::izanagi_tk::CFileUtility::RemoveExtension(s_BUF, sizeof(s_BUF), file_name.c_str());
		VRETURN(result);

		outFile.format("%s.pes", file_name);
		memset(s_BUF, 0, sizeof(s_BUF));
	}

	if (!shader.empty()) {
		// プリプロセス済みファイル
		{
			// 出力ファイルが空
			IZ_BOOL result = (::sprintf_s(s_BUF, sizeof(s_BUF), "%s\0", shader.c_str()) >= 0);
			VRETURN(result);

			// ファイル名取得
			std::string file_name(izanagi::izanagi_tk::CFileUtility::GetFileNameFromPath(s_BUF));

			// 拡張子削除
			result = izanagi::izanagi_tk::CFileUtility::RemoveExtension(s_BUF, sizeof(s_BUF), file_name.c_str());
			VRETURN(result);

			if (objDir.empty()) {
				preprocFile.format("%s.fx_", file_name.c_str());
			}
			else {
				// 中間ディレクトリに出力する
				izanagi::izanagi_tk::CFileUtility::CombinePath(
					s_BUF,
					sizeof(s_BUF),
					objDir.c_str(),
					file_name.c_str());

				preprocFile.format("%s.fx_", s_BUF);
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
				baseDir.c_str(),
				shader);
			shader.format("%s", s_BUF);
		}
	}

	{
		std::vector<izanagi::izanagi_tk::CString>::iterator it = includes.begin();
		while (it != includes.end()) {
			izanagi::izanagi_tk::CString& str = *it;

			// includeパスの環境変数の展開
			if (izanagi::izanagi_tk::CEnvVarHelper::ExpandEnvStrings(
					s_BUF,
					sizeof(s_BUF),
					str.c_str()))
			{
				str.format("%s", s_BUF);
				str.replace('\\', '/');

				if (!baseDir.empty()
					&& izanagi::izanagi_tk::CFileUtility::IsRelativePath(str))
				{
					// ベースディレクトリ相対のインクルートパスを作成
					izanagi::izanagi_tk::CFileUtility::CombinePath(
						s_BUF,
						sizeof(s_BUF),
						baseDir.c_str(),
						str);

					str.format("%s", s_BUF);
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
	compiler.clear();
	objDir.clear();
	shader.clear();
	preprocFile.clear();
	outFile.clear();

	defines.clear();
	includes.clear();

	isPreproc = FALSE;

	optionFile.clear();
}

namespace {
	// テンポラリファイル名を作成する
	inline void _CreateTmpFileName(izanagi::izanagi_tk::CString& strFile)
	{
		// システム時間取得
		SYSTEMTIME time;
		::GetLocalTime(&time);

		strFile.format(
			"__tmp_%d_%d_%d_%d_%d_%d__",
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
		std::vector<izanagi::izanagi_tk::CString>& tvRet)
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

				izanagi::izanagi_tk::CString str;
				str.format("%s", s_BUF);

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
BOOL COption::AnalysisOptionFile(std::vector<izanagi::izanagi_tk::CString>& tvArgs)
{
	BOOL ret = TRUE;

	FILE* fp = NULL;

	// テンポラリファイル名を作成する
	izanagi::izanagi_tk::CString strTmp;
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
			IZ_ASSERT(FALSE);

			// TODO

			goto __EXIT__;
		}
	}

	// オプションを引数リストに戻す
	ConvetOptionToArgs(tvArgs);

	// ファイル開く
	fopen_s(&fp, strTmp, "rt");
	if (fp == NULL) {
		IZ_ASSERT(FALSE);

		// TODO

		return FALSE;
	}

	memset(s_BUF, 0, sizeof(s_BUF));

	// リストファイルを解析
	while (fgets(s_BUF, sizeof(s_BUF), fp) != NULL) {
		izanagi::izanagi_tk::CString str;
		str.format("%s", s_BUF);

		if (strlen(str) > 0) {
			if (_IsOptionString(str)) {
				// オプションを表している文字列
				std::vector<izanagi::izanagi_tk::CString> tvTmp;

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

	if (!optionFile.empty()) {
		optionFile.replace('/', '\\');
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
void COption::ConvetOptionToArgs(std::vector<izanagi::izanagi_tk::CString>& tvArgs)
{
	// コンパイルコマンド
	if (!compiler.empty()) {
		tvArgs.push_back(izanagi::izanagi_tk::CString("-c"));
		tvArgs.push_back(compiler);
	}

	// コンパイルオプション
	if (!compileOpt.empty()) {
		tvArgs.push_back(izanagi::izanagi_tk::CString("-co"));
		tvArgs.push_back(compileOpt);
	}

	// 中間ファイルディレクトリ
	if (!objDir.empty()) {
		tvArgs.push_back(izanagi::izanagi_tk::CString("-obj"));
		tvArgs.push_back(objDir);
	}

	// コンパイルするシェーダ
	if (!shader.empty()) {
		tvArgs.push_back(izanagi::izanagi_tk::CString("-src"));
		tvArgs.push_back(shader);
	}

	// 出力ファイル
	if (!outFile.empty()) {
		tvArgs.push_back(izanagi::izanagi_tk::CString("-o"));
		tvArgs.push_back(outFile);
	}

	// アセンブラ表示のためのコンパイルをするかどうか
	if (isCompileAsm) {
		tvArgs.push_back(izanagi::izanagi_tk::CString("-asm"));
	}
}
