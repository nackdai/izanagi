#if !defined(__SHADER_CONVERTER_OPTION_H__)
#define __SHADER_CONVERTER_OPTION_H__

#include <vector>
#include "izToolKit.h"

#include "ShaderConfig.h"

/**
* オプション構造体
*/
struct SOption {
    izanagi::tool::CString in_file;

    std::vector<izanagi::tool::CString> includes;
    std::vector<izanagi::tool::CString> defines;

    izanagi::tool::CString preproc_file;    // プリプロセス済みファイル

    izanagi::tool::CString out_file;        // 出力ファイル

    izanagi::tool::CString obj_dir;         // 中間ファイルディレクトリ
    izanagi::tool::CString export_dir;

    izanagi::E_PLATFORM type;                // シェーダコンパイルタイプ

    struct {
        UINT isPreproc          : 1;        // プリプロセス処理のみを行うかどうか
        UINT isPreprocShader    : 1;
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

#endif  // #if !defined(__SHADER_CONVERTER_OPTION_H__)
