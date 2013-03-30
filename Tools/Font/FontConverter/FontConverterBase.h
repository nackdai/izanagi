#if !defined(__FONT_CONVERTER_BASE_H__)
#define __FONT_CONVERTER_BASE_H__

#include <vector>
#include <set>
#include "izToolKit.h"
#include "izFont.h"

#define _TEX_FMT_A8_

struct SOption;

struct SGlyphMetrics
{
    IZ_UINT8 advance;
    IZ_UINT8 ascender;
    IZ_UINT8 descender;
    IZ_UINT8 height;
    IZ_UINT8 width;
    IZ_UINT8 bearingX;
    IZ_UINT8 bearingY;
    IZ_UINT8 padding;
};

struct SGlyphImage
{
    IZ_INT leftOffset;
    IZ_INT topOffset;
};

class CFontConverterBase
{
public:
    virtual ~CFontConverterBase();

protected:
    CFontConverterBase();

    NO_COPIABLE(CFontConverterBase);

public:
    // 初期化
    PURE_VIRTUAL(
        IZ_BOOL Init(
            void* initializeData,
            const SOption& sOption));

    // フォントイメージ作成
    IZ_BOOL CreateFontImage(
        const SOption& sOption,
        const std::set<IZ_UINT>& tsCharList);

    // 開放
    virtual void Release();

    // 出力
    BOOL ExportAsDDS();

    // 出力
    BOOL Export(
        izanagi::IOutputStream* pOut,
        const SOption& sOption,
        const std::set<IZ_UINT>& tsCharList);

protected:
    PURE_VIRTUAL(IZ_BOOL BeginCreateFontImage(const SOption& option));

    PURE_VIRTUAL(
        IZ_UINT GetGlyphMetrics(
            IZ_UINT code,
            const SOption& option,
            SGlyphMetrics&  metrics,
            SGlyphImage& image));

    PURE_VIRTUAL(
        IZ_BOOL GetGlyphImage(
            IZ_UINT code,
            const SOption& option,
            void* dst,
            IZ_UINT dstBytes));

    PURE_VIRTUAL(
        void WriteImage(
            const SOption& option,
            const SGlyphMetrics& metrics,
            void* src,
            void* dst,
            IZ_UINT pitch, IZ_UINT bpp,
            IZ_UINT x, IZ_UINT y,
            IZ_UINT leftOffset, IZ_UINT topOffset));

    PURE_VIRTUAL(IZ_UINT GetTextMetricsHeight());

    PURE_VIRTUAL(IZ_UINT GetTextAscender());
    PURE_VIRTUAL(IZ_UINT GetTextDescender());

    // ファイルフォーマットを指定してイメージ出力
    BOOL ExportAs(
        izanagi::tool::CTextureLite* pTex,
        LPCSTR pszName,
        D3DXIMAGE_FILEFORMAT nFileFmt);

    // イメージデータ部出力
    UINT ExportFontImageData(izanagi::IOutputStream* pOut);

protected:
#ifdef _TEX_FMT_A8_
    static const D3DFORMAT D3D_TEX_FMT = D3DFMT_A8;
    static const izanagi::graph::E_GRAPH_PIXEL_FMT IZ_TEX_FMT = izanagi::graph::E_GRAPH_PIXEL_FMT_A8;
    static const UINT TEX_BPP = 1;
#else
    static const D3DFORMAT D3D_TEX_FMT = D3DFMT_A8B8G8R8;
    static const izanagi::graph::E_GRAPH_PIXEL_FMT IZ_TEX_FMT = izanagi::graph::E_GRAPH_PIXEL_FMT_BGRA8
    static const UINT TEX_BPP = 4;
#endif
    static const IZ_INT MARGIN = 1;
        
protected:
    std::vector<izanagi::tool::CTextureLite*> m_TexList;

    // フォントイメージ情報リスト
    std::vector<izanagi::text::S_FNT_IMAGE> m_FontImgList;

    // フォントマップ情報リスト
    std::vector<izanagi::text::S_FNT_MAP> m_FontMapList;
};

#endif  // #if !defined(__FONT_CONVERTER_BASE_H__)
