#if !defined(__OPTION_H__)
#define __OPTION_H__

#include "izToolKit.h"

struct SOption {
    izanagi::tool::CString in_dir;      // 入力ディレクトリ

    izanagi::tool::CString label;
    izanagi::tool::CString output;      // 出力
    izanagi::tool::CString file_id; // ファイルID
    izanagi::tool::CString obj_dir; // 中間データ出力ディレクトリ
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

#endif  // #if !defined(__OPTION_H__)
