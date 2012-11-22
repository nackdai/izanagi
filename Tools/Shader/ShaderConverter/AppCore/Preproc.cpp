#include "Preproc.h"
#include "Option.h"
#include "izToolKit.h"

// プリプロセッサエントリポイント
extern "C"
int preproc_main(int argc, char* argv[]);

/**
* プリプロセス処理を行う
*/
int Preproc(COption& cOption)
{
	// プリプロセッサに渡す引数を作成
	std::vector<char*> tvArgs;

	// 最初はダミー
	tvArgs.push_back("");

	// インクルードパス
	{
		std::vector<izanagi::tool::CString>::iterator it = cOption.includes.begin();
		while (it != cOption.includes.end()) {
			LPCSTR str = *it;
			tvArgs.push_back("-I");
			tvArgs.push_back(const_cast<char*>(str));
			it++;
		}
	}

	// プリプロセス定義
	{
		std::vector<izanagi::tool::CString>::iterator it = cOption.defines.begin();
		while (it != cOption.defines.end()) {
			LPCSTR str = *it;
			tvArgs.push_back("-D");
			tvArgs.push_back(const_cast<char*>(str));
			it++;
		}
	}

	// コメントは保持する
	//tvArgs.push_back("-C");

	// プリプロセッサ出力をファイルに書き込むようにする
	//tvArgs.push_back("-P");

	if (!cOption.isPreprocShader) {
		// without #line.
		tvArgs.push_back("-EP");
	}

	// 入力ファイル
	{
		LPCSTR tmp = cOption.in_file;
		tvArgs.push_back(const_cast<char*>(tmp));
	}

	// 出力ファイル
	{
		LPCSTR tmp = cOption.preproc_file;
		tvArgs.push_back(const_cast<char*>(tmp));
	}

	int ret = preproc_main((int)tvArgs.size(), &tvArgs[0]);

	if (ret > 0) {
		izanagi::tool::CString tmp;
		for (int i = 0; i < (int)tvArgs.size(); i++) {
			tmp += tvArgs[0];
			tmp += " ";
		}

		// TODO
	}

	return ret;
}

/**
* 自分自身をプリプロセス処理モードで呼び出す
*/
BOOL ExecWithPreprocMode(
	const char* lpszExe,
	COption& cOption)
{
	// コマンド作成
	izanagi::tool::CString strCmd;
	strCmd.format(
		"%s -E %s -out_tmp %s",
		lpszExe,
		cOption.in_file.c_str(),
		cOption.preproc_file.c_str());

	if (cOption.isPreprocShader) {
		strCmd += " -PreprocShd";
	}

	// インクルードパス
	{
		std::vector<izanagi::tool::CString>::iterator it = cOption.includes.begin();
		while (it != cOption.includes.end()) {
			strCmd += " -I ";
			strCmd += *it;
			it++;
		}
	}

	// プリプロセス定義
	{
		std::vector<izanagi::tool::CString>::iterator it = cOption.defines.begin();
		while (it != cOption.defines.end()) {
			strCmd += " -D ";
			strCmd += *it;
			it++;
		}
	}

	// 実行すっぞ
	FILE* fp = _popen(strCmd, "w");
	if (fp == NULL) {
		IZ_ASSERT(IZ_FALSE);

		// TODO

		return IZ_FALSE;
	}

	// ０で正常終了
	int result = _pclose(fp);
		
	return (result == 0);
}
