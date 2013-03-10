#if !defined(__FONT_CONVERTER_FT_H__)
#define __FONT_CONVERTER_FT_H__

#include "FontConverterBase.h"

#include <ft2build.h>
#include FT_FREETYPE_H

class CFontConverterFT : public CFontConverterBase
{
public:
    CFontConverterFT();
    virtual ~CFontConverterFT();

public:
    virtual IZ_BOOL Init(
        void* initializeData,
        const SOption& sOption);

    // 開放
    virtual void Release();

private:
    virtual IZ_BOOL BeginCreateFontImage(const SOption& option);

    virtual IZ_UINT GetGlyphMetrics(
        IZ_UINT code,
        const SOption& option,
        SGlyphMetrics&  metrics,
        SGlyphImage& image);

    virtual IZ_BOOL GetGlyphImage(
        IZ_UINT code,
        const SOption& option,
        void* dst,
        IZ_UINT dstBytes);

    virtual void WriteImage(
        const SOption& option,
        const SGlyphMetrics& metrics,
        void* src,
        void* dst,
        IZ_UINT pitch, IZ_UINT bpp,
        IZ_UINT x, IZ_UINT y,
        IZ_UINT leftOffset, IZ_UINT topOffset);

    virtual IZ_UINT GetTextMetricsHeight();

    virtual IZ_UINT GetTextAscender();
    virtual IZ_UINT GetTextDescender();

protected:
    FT_Library m_FTLibrary;
    FT_Face m_Face;
};

#endif  // #if !defined(__FONT_CONVERTER_FT_H__)
