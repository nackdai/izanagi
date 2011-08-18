#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <string>
#include "izDefs.h"

// 出力タイプ
enum ExportType {
	ExportTypeMdl = 1 << 0,	// MDLとして出力
	ExportTypeMsh = 1 << 1,	// MSHを出力
	ExportTypeSkl = 1 << 2,	// SKLを出力
};

// ファイルタイプ
enum FileType {
	FileTypeUnknown = 0,
	FileTypeCollada,
	FileTypeXFile,
	FileTypeFBX,

	FileTypeNum,
};

struct SOption {
	std::string in;		// 入力ファイル
	std::string out;	// 出力ファイル

	std::string outMsh;
	std::string outSkl;

	IZ_UINT exportType;	// 出力タイプ
	FileType fileType;	// ファイルタイプ
};

class COption : public SOption {
public:
	COption();
	~COption();

public:
	// 解析
	IZ_BOOL Analysis(int argc, char* argv[]);

	IZ_BOOL IsValid();

private:
	IZ_BOOL AnalysisInternal();
};

#endif	// #if !defined(__OPTION_H__)
