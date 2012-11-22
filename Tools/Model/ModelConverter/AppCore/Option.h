#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <string>
#include "izDefs.h"
#include "izToolKit.h"
#include "izModelLib.h"

// 出力タイプ
enum ExportType {
	ExportTypeMdl = 1 << 0,	// MDLとして出力
	ExportTypeMsh = 1 << 1,	// MSHを出力
	ExportTypeSkl = 1 << 2,	// SKLを出力
};

struct SOption {
	izanagi::tool::CString in;	// 入力ファイル
	izanagi::tool::CString out;	// 出力ファイル

	izanagi::tool::CString outMsh;
	izanagi::tool::CString outSkl;

	IZ_UINT exportType;	// 出力タイプ

	IZ_UINT maxJointMtxNum;

	// トライアングルリストで出力するかどうか
	IZ_BOOL isExportTriList;
};

class COption : public CToolOption<SOption> {
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
