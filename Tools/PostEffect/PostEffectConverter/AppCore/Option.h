#if !defined(__POSTEFFECT_CONVERTER_OPTION_H__)
#define __POSTEFFECT_CONVERTER_OPTION_H__

#include <vector>
#include "izToolKit.h"

/**
* オプション構造体
*/
struct SOption {
	izanagi::tool::CString compiler;		// コンパイルコマンド
	izanagi::tool::CString compileOpt;	// コンパイルオプション

	izanagi::tool::CString objDir;		// 中間ファイルディレクトリ

	izanagi::tool::CString shader;			// コンパイルするシェーダ
	izanagi::tool::CString preprocFile;	// プリプロセス済みファイル
	izanagi::tool::CString outFile;		// 出力ファイル

	std::vector<izanagi::tool::CString> defines;	// -Dオプションで指定されたもの
	std::vector<izanagi::tool::CString> includes;	// -Iオプションで指定されたもの

	struct {
		UINT isPreproc		: 1;	// プリプロセス処理のみを行うかどうか
		UINT isCompileAsm	: 1;	// アセンブラ表示のためのコンパイルをするかどうか
		UINT reserved		: 31;
	};

	// オプション記述ファイル
	izanagi::tool::CString optionFile;

	// ベースディレクトリ
	std::string baseDir;
};

/**
* オプションクラス
*/
class COption : public SOption {
public:
	COption();
	~COption() {}

public:
	// 解析
	BOOL Analysis(int argc, char* argv[]);

	// 正当性チェック
	BOOL IsValid();

protected:
	// 解析処理
	BOOL AnalysisInternal();

	// コピー
	void Copy(const COption& rhs);

	// クリア
	void Clear();

	// オプションファイル解析
	BOOL AnalysisOptionFile(std::vector<izanagi::tool::CString>& tvArgs);

	// オプションを引数リストに戻す
	void ConvetOptionToArgs(std::vector<izanagi::tool::CString>& tvArgs);

public:
	// プリプロセス処理のみを行うかどうか
	BOOL IsPreproc() const { return isPreproc; }	
};

#endif	// #if !defined(__POSTEFFECT_CONVERTER_OPTION_H__)
