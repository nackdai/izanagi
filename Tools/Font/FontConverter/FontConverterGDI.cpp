#ifndef _WIN32_WINNT        // Windows XP 以降のバージョンに固有の機能の使用を許可します。                   
#define _WIN32_WINNT 0x0501 // これを Windows の他のバージョン向けに適切な値に変更してください。
#endif                      

#include "FontConverterGDI.h"
#include "CharList.h"
#include "Option.h"

// コンストラクタ
CFontConverterGDI::CFontConverterGDI()
{
    m_hFont = NULL;
    m_hOldFont = NULL;
    m_hDC = NULL;
}

// デストラクタ
CFontConverterGDI::~CFontConverterGDI()
{
    Release();
}

namespace {
    inline IZ_UINT _GetTextHeight(
        HDC hDC,
        izanagi::E_FONT_CHAR_ENCODE encode)
    {
        IZ_UINT ret = 0;

        if (encode == izanagi::E_FONT_CHAR_ENCODE_UTF16) {
            TEXTMETRICW tm;
            GetTextMetricsW(hDC, &tm);
            ret = tm.tmHeight;
        }
        else {
            TEXTMETRICA tm;
            GetTextMetricsA(hDC, &tm);
            ret = tm.tmHeight;
        }

        return ret;
    }
}   // namespace

// 初期化
IZ_BOOL CFontConverterGDI::Init(
    void* initializeData,
    const SOption& sOption)
{
    IZ_BOOL ret = IZ_FALSE;

    HDC hDC = (HDC)initializeData;

    // TODO
    // localeの設定
    //::setlocale(LC_ALL, _T("Japanese"));

    // ピッチ
    BYTE nFontPitch = (sOption.isFixedPitch ? FIXED_PITCH : DEFAULT_PITCH);

    // NOTE
    // フォントファミリは、フォントの見た目を定義します。
    // lpszFace で指定した フォント名のフォントが利用できないときに、フォントの選択に使われます。

    // フォントファミリ
    BYTE nFontFamily = FF_DONTCARE;

    IZ_UINT nFontHeight = sOption.fontSize;

    for (;;) {
        // TODO
        // フォント作成
        m_hFont = ::CreateFont(
                        nFontHeight,        // 高さ
                        0,                  // 幅
                        0,
                        0,
                        sOption.isBold ? FW_BOLD : FW_NORMAL,   // 太さ
                        sOption.isItalic,                       // 斜体
                        FALSE,                                  // 下線
                        FALSE,                                  // StrikeOut (水平線？）
                        DEFAULT_CHARSET,                        // キャラクタセット
                        OUT_TT_ONLY_PRECIS,                     // 出力精度
                        CLIP_DEFAULT_PRECIS,
                        CLEARTYPE_QUALITY,                      // 品質
                        nFontPitch | nFontFamily,               // ピッチ
                        sOption.fontFace);
        ret = (m_hFont != NULL);
        VRETURN(ret);

        m_hOldFont = (HFONT)::SelectObject(hDC, m_hFont);
        VRETURN(m_hOldFont);

        IZ_UINT nHeight = _GetTextHeight(hDC, sOption.charEncode);
        if (nHeight <= sOption.fontSize) {
            break;
        }

        nFontHeight--;
        IZ_ASSERT(nFontHeight != 0);

        ::SelectObject(hDC, m_hOldFont);
        ::DeleteObject(m_hFont);

        m_hOldFont = IZ_NULL;
        m_hFont = IZ_NULL;
    }

    if (ret)
    {
        m_hDC = hDC;
    }

    return ret;
}

void CFontConverterGDI::Release()
{
    CFontConverterBase::Release();

    if (m_hFont != NULL) {
        ::DeleteObject(m_hFont);
        m_hFont = NULL;
    }
}

IZ_BOOL CFontConverterGDI::BeginCreateFontImage(const SOption& option)
{
    IZ_ASSERT(m_hDC != IZ_NULL);

    if (m_hOldFont == IZ_NULL) {
        m_hOldFont = (HFONT)::SelectObject(m_hDC, m_hFont);
        VRETURN(m_hOldFont);
    }

    if (option.charEncode == izanagi::E_FONT_CHAR_ENCODE_UTF16)
    {
        TEXTMETRICW tm;
        GetTextMetricsW(m_hDC, &tm);

        m_TextMetrics.ascender = tm.tmAscent;
        m_TextMetrics.descender = tm.tmDescent;
        m_TextMetrics.height = tm.tmHeight;
    }
    else
    {
        TEXTMETRICA tm;
        GetTextMetricsA(m_hDC, &tm);

        m_TextMetrics.ascender = tm.tmAscent;
        m_TextMetrics.descender = tm.tmDescent;
        m_TextMetrics.height = tm.tmHeight;
    }

    // アンチエイリアスレベル
    m_AALevel = 1;
    switch (option.typeAA)
    {
    case GGO_GRAY2_BITMAP:
        m_AALevel = 4;
        break;
    case GGO_GRAY4_BITMAP:
        m_AALevel = 16;
        break;
    case GGO_GRAY8_BITMAP:
        m_AALevel = 64;
        break;
    }

    return IZ_TRUE;
}

static const MAT2 mat = 
{
    {0, 1},{0, 0},  // 1, 0
    {0, 0},{0, 1}   // 0, 1
};

IZ_UINT CFontConverterGDI::GetGlyphMetrics(
    IZ_UINT code,
    const SOption& option,
    SGlyphMetrics&  metrics,
    SGlyphImage& image)
{
    // NOTE
    // わたってくる文字コードは必ずUnicode

    GLYPHMETRICS gdiMetrics;

    IZ_UINT ret = ::GetGlyphOutlineW(
        m_hDC,
        code,
        option.typeAA,
        &gdiMetrics,
        0,
        NULL,
        &mat);

    if (ret > 0)
    {
        metrics.advance = gdiMetrics.gmCellIncX;
        metrics.width = gdiMetrics.gmBlackBoxX;
        metrics.height = gdiMetrics.gmBlackBoxY;
        metrics.bearingX = gdiMetrics.gmptGlyphOrigin.x;
        metrics.bearingY = gdiMetrics.gmptGlyphOrigin.y;
        metrics.ascender = m_TextMetrics.ascender;
        metrics.descender = m_TextMetrics.descender;
    }

    return ret;
}

IZ_BOOL CFontConverterGDI::GetGlyphImage(
    IZ_UINT code,
    const SOption& option,
    void* dst,
    IZ_UINT dstBytes)
{
    // NOTE
    // わたってくる文字コードは必ずUnicode

    GLYPHMETRICS gdiMetrics;

    IZ_UINT ret = ::GetGlyphOutlineW(
        m_hDC,
        code,
        option.typeAA,
        &gdiMetrics,
        dstBytes,
        dst,
        &mat);

    return (ret > 0);
}

namespace
{
    void _WriteColor(
        const SGlyphMetrics& metrics,
        IZ_UINT textMetricsHeight,
        BYTE* pSrc,
        BYTE* pDst,
        UINT nDstPitch, UINT nDstBpp,
        UINT nDstX, UINT nDstY,
        UINT nDstLeft, UINT nDstTop)
    {
        UINT nPitch = (1 + (metrics.advance / 32)) * 4;

        UINT nHeight = metrics.height;
        UINT nWidth = metrics.advance;

        for (UINT y = 0; y < nHeight; y++) {
            BYTE* dst = pDst + nDstPitch * (nDstY + nDstTop + y) + (nDstX + nDstLeft) * nDstBpp;

            for (UINT x = 0; x < nWidth; x++) {
                DWORD num = x / 8;      // 現在のxが1行の何BYTE目かを算出
                BYTE bit = x % 8;       // 現在のxが1バイト内の何ビット目かを算出
                BYTE mask = ((BYTE)1) << (7 - bit); // 現在のビット数のマスク作成

                // 現在のバイト位置にあるビットマップをを取得
                BYTE cur = *(pSrc + nPitch * y + num);

                // 作成したマスクと現在のバイト位置とで論理積演算
                cur &= mask;

                // curに立ったビットフラグを最下位ビットまでシフトしてアルファ値に変換
                BYTE alpha = (cur >> (7 - bit)) * 255;
                
#ifdef _TEX_FMT_A8_
                *dst++ = alpha;
#else
                *dst++ = 0xff;
                *dst++ = 0xff;
                *dst++ = 0xff;
                *dst++ = alpha;
#endif
            }
        }
    }

    void _WriteColorWithAA(
        const SGlyphMetrics& metrics,
        IZ_UINT textMetricsHeight,
        BYTE* pSrc,
        BYTE* pDst,
        UINT nDstPitch, UINT nDstBpp,
        UINT nDstX, UINT nDstY,
        UINT nDstLeft, UINT nDstTop,
        UINT nAALevel)
    {
        UINT nPitch = (metrics.width + 3) & 0xfffc;

        UINT nHeight = metrics.height;
        UINT nWidth = metrics.advance;

        for (UINT y = 0; y < nHeight; y++) {
            BYTE* src = pSrc + nPitch * y;
            BYTE* dst = pDst + nDstPitch * (nDstY + nDstTop + y) + (nDstX + nDstLeft) * nDstBpp;

            for (UINT x = 0; x < nWidth; x++) {
#if 0
                BYTE alpha = (*src++) * 255 / nAALevel; // よく分からんがこういうもんらしい・・・
#else
                UINT tmp = *src++;
                BYTE alpha = static_cast<BYTE>(CLAMP((tmp * 256 / 16), 0, 255));
#endif

#ifdef _TEX_FMT_A8_
                *dst++ = alpha;
#else
                *dst++ = 0xff;
                *dst++ = 0xff;
                *dst++ = 0xff;
                *dst++ = alpha;
#endif
            }
        }
    }
}

void CFontConverterGDI::WriteImage(
    const SOption& option,
    const SGlyphMetrics& metrics,
    void* src,
    void* dst,
    IZ_UINT pitch, IZ_UINT bpp,
    IZ_UINT x, IZ_UINT y,
    IZ_UINT leftOffset, IZ_UINT topOffset)
{
    BYTE* srcPtr = (BYTE*)src;

    if (option.typeAA == GGO_BITMAP)
    {
        _WriteColor(
            metrics,
            m_TextMetrics.ascender,
            srcPtr,
            (BYTE*)dst,
            pitch, bpp,
            x, y,
            leftOffset, topOffset);
    }
    else
    {
        _WriteColorWithAA(
            metrics,
            m_TextMetrics.ascender,
            srcPtr,
            (BYTE*)dst,
            pitch, bpp,
            x, y,
            leftOffset, topOffset,
            m_AALevel);
    }
}

IZ_UINT CFontConverterGDI::GetTextMetricsHeight()
{
    return m_TextMetrics.height;
}

IZ_UINT CFontConverterGDI::GetTextAscender()
{
    return m_TextMetrics.ascender;
}

IZ_UINT CFontConverterGDI::GetTextDescender()
{
    return m_TextMetrics.descender;
}
