#if !defined(__OPTION_H__)
#define __OPTION_H__

#include "izToolKit.h"

struct SOption {
	izanagi::izanagi_tk::CString in_dir;		// 入力ディレクトリ

	izanagi::izanagi_tk::CString label;
	izanagi::izanagi_tk::CString output;		// 出力
	izanagi::izanagi_tk::CString file_id;	// ファイルID
	izanagi::izanagi_tk::CString obj_dir;	// 中間データ出力ディレクトリ
};

class COption : public SOption {
public:
	COption();
	~COption() {}

public:
	// 解析
	BOOL Analysis(int argc, char* argv[]);

	// 正当性チェック
	BOOL IsValid();

private:
	// 解析の後処理
	BOOL AfterAnalysis();
};

#endif	// #if !defined(__OPTION_H__)
