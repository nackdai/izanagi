#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <vector>
#include "FontConverterDefs.h"
#include "FontConverterImpl.h"
#include "izFont.h"

struct SOption : public CFontConverter::SOption {
	// 入力リストファイル
	izanagi::tool::CString list_file;

	// 入力リストファイルを解析した入力ファイルのリスト
	std::vector<izanagi::tool::CString> in_file_list;

	// 出力ファイル
	izanagi::tool::CString out_file;
};

class COption : public SOption {
public:
	COption();
	~COption() {}

protected:
	COption(const COption& rhs);
	const COption& operator=(const COption& rhs);

public:
	// 解析
	BOOL Analysis(int argc, TCHAR* argv[]);

	// オプション正当性チェック
	BOOL IsValid();

	// リストファイル解析
	BOOL AnalysisListFile();

private:
	// 解析の後処理
	BOOL AfterAnalysis();
};

#endif	// #if !defined(__OPTION_H__)
