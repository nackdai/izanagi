#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <string>
#include "izDefs.h"
#include "izToolKit.h"
#include "izModelLib.h"

struct SOption {
    izanagi::tool::CString in;  // 入力ファイル
    izanagi::tool::CString out; // 出力ファイル

    std::string base;   // ベースモデルファイル

    IZ_UINT idx;
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

#endif  // #if !defined(__OPTION_H__)
