// FontCoverter.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include <windows.h>

#include "CharList.h"
#include "Option.h"
#include "FontConverterFT.h"
#include "izToolKit.h"

using namespace std;

namespace {
    void _DispUsage()
    {
        printf(
            "FontConverter Ver.0.01\n"
            "\n"
            "フォントデータ作成ツール\n"
            "[USAGE]\n"
            " FontConverter.exe [options]\n"
            "\n"
            "[options]\n"
            " -i [in]      : 入力ファイルリスト\n"
            " -o [out]     : 出力ファイル\n"
            "\n"
            " -fs [size]   : フォントサイズ\n"
            "                デフォルトでは'16'が設定されている\n"
            " -w [size]    : 出力テクスチャ幅\n"
            "                デフォルトでは'256'が設定されている\n"
            " -h [size]    : 出力テクスチャ高さ\n"
            "                デフォルトでは'256'が設定されている\n"
            "\n"
            " -ft [type]   : フォント種別\n"
            "                デフォルトでは'ＭＳ 明朝'が設定されている\n"
            " -c [code]    : 文字コード\n"
            "                解析するファイルの文字コード\n"
            "                デフォルトではSJISとして扱う\n"
            "\n"
            " -aa [num]    : アンチエイリアスのサンプリング数\n"
            "                2、4、8 を設定することができる\n"
            "                オプションを指定しないとアンチアイリアス無しとなる\n"
            "\n"
            " -fixed_pitch : 固定ピッチとして出力する\n"
            " -bold        : 太字として出力する\n"
            " -italic      : イタリックとして出力する\n"
        );
    }
}   // namespace

#define _VGOTO(b, label)    if (!b) { nRetCode = 1; VGOTO(IZ_FALSE, label); }

int main(int argc, char* argv[])
{
    int nRetCode = 0;

    CFontConverterBase* fontConverter = IZ_NULL;

    COption cOption;
    izanagi::tool::CGraphicsDeviceLite* pDevice = IZ_NULL;
    izanagi::CFileOutputStream cOut;

    // オプション解析
    if (!cOption.Analysis(argc, argv))
    {
        _DispUsage();
        _VGOTO(IZ_FALSE, __EXIT__);
    }

    // オプション正当性チェック
    if (!cOption.IsValid())
    {
        _DispUsage();
        _VGOTO(IZ_FALSE, __EXIT__);
    }
    
    // リストファイルを解析
    _VGOTO(cOption.AnalysisListFile(), __EXIT__);

    // ウインドウハンドル取得
    HWND hWnd = ::GetConsoleWindow();
    if (hWnd == NULL)
    {
        hWnd = FindWindow(0, 0);
    }
    _VGOTO(hWnd != NULL, __EXIT__);

    // GraphicsDevice作成
    pDevice = izanagi::tool::CGraphicsDeviceLite::CreateGraphicsDeviceLight(hWnd);
    _VGOTO(pDevice != IZ_NULL, __EXIT__);

    {
        std::vector<izanagi::tool::CString>::iterator it = cOption.in_file_list.begin();
        while (it != cOption.in_file_list.end()) {
            izanagi::CFileInputStream cIn;
            _VGOTO(cIn.Open(*it), __EXIT__);

            // 文字列登録
            CCharList::GetInstance().Register(
                cOption.charEncode,
                &cIn);

            cIn.Close();
            it++;
        }
    }

    // デバイスコンテキスト取得
    HDC hDC = ::GetDC(hWnd);
    _VGOTO(hDC != NULL, __EXIT__);

    //fontConverter = new CFontConverterGDI();
    fontConverter = new CFontConverterFT();

    // 準備
    _VGOTO(
        fontConverter->Init(hDC, cOption),
        __EXIT__);

    // フォントイメージデータ作成
    _VGOTO(
        fontConverter->CreateFontImage(cOption, CCharList::GetInstance().GetCharList()),
        __EXIT__);

    // 出力
#if 0
    _VGOTO(
        CFontConverter::GetInstance().ExportAsDDS(),
        __EXIT__);
#else
    _VGOTO(
        cOut.Open(cOption.out_file),
        __EXIT__);

    _VGOTO(
        fontConverter->Export(&cOut, cOption, CCharList::GetInstance().GetCharList()),
        __EXIT__);

    cOut.Finalize();
#endif

__EXIT__:
    // 開放
    SAFE_DELETE(fontConverter);
    SAFE_RELEASE(pDevice);

    return nRetCode;
}
