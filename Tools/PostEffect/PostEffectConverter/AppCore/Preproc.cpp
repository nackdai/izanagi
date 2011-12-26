#include "Preproc.h"
#include "Option.h"

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
			izanagi::tool::CString& str = *it;
			tvArgs.push_back("-I");
			tvArgs.push_back(const_cast<char*>(str.c_str()));
			it++;
		}
	}

	// プリプロセス定義
	{
		std::vector<izanagi::tool::CString>::iterator it = cOption.defines.begin();
		while (it != cOption.defines.end()) {
			izanagi::tool::CString& str = *it;
			tvArgs.push_back("-D");
			tvArgs.push_back(const_cast<char*>(str.c_str()));
			it++;
		}
	}

	// コメントは保持する
	//tvArgs.push_back("-C");

	// プリプロセッサ出力をファイルに書き込むようにする
	//tvArgs.push_back("-P");

	// 入力ファイル
	{
		izanagi::tool::CString& tmp = cOption.shader;
		tvArgs.push_back(const_cast<char*>(tmp.c_str()));
	}

	// 出力ファイル
	{
		izanagi::tool::CString& tmp = cOption.outFile;
		tvArgs.push_back(const_cast<char*>(tmp.c_str()));
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
		"%s -E %s -o %s",
		lpszExe,
		cOption.shader.c_str(),
		cOption.preprocFile.c_str());

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
	FILE* fp = _popen(strCmd.c_str(), "w");
	if (fp == NULL) {
		IZ_ASSERT(FALSE);

		// TODO

		return FALSE;
	}

	// ０で正常終了
	int result = _pclose(fp);
		
	return (result == 0);
}
