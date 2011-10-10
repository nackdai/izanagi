#if !defined(__SHADER_CONVERTER_OPTION_H__)
#define __SHADER_CONVERTER_OPTION_H__

#include <vector>
#include "izToolKit.h"

/**
* オプション構造体
*/
struct SOption {
	izanagi::izanagi_tk::CString in_file;

	std::vector<izanagi::izanagi_tk::CString> includes;
	std::vector<izanagi::izanagi_tk::CString> defines;

	izanagi::izanagi_tk::CString preproc_file;	// プリプロセス済みファイル

	izanagi::izanagi_tk::CString out_file;		// 出力ファイル

	izanagi::izanagi_tk::CString obj_dir;		// 中間ファイルディレクトリ
	izanagi::izanagi_tk::CString export_dir;

	struct {
		UINT isPreproc			: 1;	// プリプロセス処理のみを行うかどうか
		UINT isPreprocShader	: 1;
	};
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

	BOOL PreprocInputFile();

protected:
	BOOL EndAnalysis();

public:
	// プリプロセス処理のみを行うかどうか
	BOOL IsPreproc() const { return isPreproc; }	
};

#endif	// #if !defined(__SHADER_CONVERTER_OPTION_H__)
