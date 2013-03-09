#if !defined(__FONT_CONVERTER_GDI_H__)
#define __FONT_CONVERTER_GDI_H__

#include "FontConverterBase.h"

class CFontConverterGDI : public CFontConverterBase
{
public:
    CFontConverterGDI();
    virtual ~CFontConverterGDI();

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
    HDC m_hDC;

    HFONT m_hFont;
    HFONT m_hOldFont;

    IZ_UINT m_AALevel;

    struct {
        IZ_UINT height;
        IZ_UINT ascender;
        IZ_UINT descender;
    } m_TextMetrics;
};

#endif  // #if !defined(__FONT_CONVERTER_GDI_H__)
