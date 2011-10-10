#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <string>
#include "izDefs.h"
#include "izToolKit.h"

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
	izanagi::izanagi_tk::CString in;		// 入力ファイル
	izanagi::izanagi_tk::CString out;	// 出力ファイル

	izanagi::izanagi_tk::CString outMsh;
	izanagi::izanagi_tk::CString outSkl;

	IZ_UINT exportType;	// 出力タイプ
	FileType fileType;	// ファイルタイプ

	IZ_UINT maxJointMtxNum;
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
