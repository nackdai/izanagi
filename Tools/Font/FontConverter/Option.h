#if !defined(__OPTION_H__)
#define __OPTION_H__

#include <vector>
#include "FontConverterDefs.h"
#include "izFont.h"

struct SOption
{
    IZ_UINT fontSize;       // フォントサイズ
    IZ_UINT texWidth;       // テクスチャ幅
    IZ_UINT texHeight;      // テクスチャ高さ

    // アンチエイリアス
    IZ_UINT typeAA;

    izanagi::tool::CString fontFace;

    izanagi::E_FONT_CHAR_ENCODE charEncode;

    struct {
        IZ_UINT isFixedPitch    : 1;    // 固定ピッチ
        IZ_UINT isBold          : 1;    // Bold
        IZ_UINT isItalic        : 1;    // Italic
        IZ_UINT reserved        : 29;
    };

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

#endif  // #if !defined(__OPTION_H__)
