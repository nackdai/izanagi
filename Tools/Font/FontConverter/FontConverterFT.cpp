#include "FontConverterFT.h"
#include "Option.h"

CFontConverterFT::CFontConverterFT()
{
}

CFontConverterFT::~CFontConverterFT()
{
    Release();
}

IZ_BOOL CFontConverterFT::Init(
    void* initializeData,
    const SOption& sOption)
{
    FT_Init_FreeType(&m_FTLibrary);

    FT_Error err = FT_New_Face(
        m_FTLibrary,
        sOption.fontFace,
        0,
        &m_Face);
    VRETURN(err == 0);

    err = FT_Set_Pixel_Sizes(
       m_Face,
       0,
       sOption.fontSize);
    VRETURN(err == 0);

    return IZ_TRUE;
}

void CFontConverterFT::Release()
{
    CFontConverterBase::Release();

    // TODO
}

IZ_BOOL CFontConverterFT::BeginCreateFontImage(const SOption& option)
{
    // Nothing is done.
    return IZ_TRUE;
}

IZ_UINT CFontConverterFT::GetGlyphMetrics(
    IZ_UINT code,
    const SOption& option,
    SGlyphMetrics&  metrics,
    SGlyphImage& image)
{
    FT_UInt id = FT_Get_Char_Index(m_Face, code);
    
    FT_Error err = FT_Load_Glyph(
        m_Face,
        id,
        FT_LOAD_DEFAULT);
    VRETURN(err == 0);

    metrics.advance = static_cast<IZ_UINT8>(m_Face->glyph->advance.x >> 6);
    metrics.ascender = static_cast<IZ_UINT8>(m_Face->size->metrics.ascender >> 6);
    metrics.descender = static_cast<IZ_UINT8>((m_Face->size->metrics.descender * -1) >> 6);
    metrics.bearingX = static_cast<IZ_UINT8>(m_Face->glyph->metrics.horiBearingX >> 6);
    metrics.bearingY = static_cast<IZ_UINT8>(m_Face->glyph->metrics.horiBearingY >> 6);
    metrics.width = static_cast<IZ_UINT8>(m_Face->glyph->metrics.width >> 6);
    metrics.height = static_cast<IZ_UINT8>(m_Face->glyph->metrics.height >> 6);

    err = FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL);
    VRETURN(err == 0);

    image.leftOffset = m_Face->glyph->bitmap_left;
    image.topOffset = m_Face->glyph->bitmap_top;

    IZ_ASSERT(metrics.width == m_Face->glyph->bitmap.pitch);
    IZ_ASSERT(metrics.height == m_Face->glyph->bitmap.rows);

    IZ_UINT ret = m_Face->glyph->bitmap.pitch * m_Face->glyph->bitmap.rows;
    return ret;
}

IZ_BOOL CFontConverterFT::GetGlyphImage(
    IZ_UINT code,
    const SOption& option,
    void* dst,
    IZ_UINT dstBytes)
{
    FT_UInt id = FT_Get_Char_Index(m_Face, code);

    FT_Error err = FT_Load_Glyph(
        m_Face,
        id,
        FT_LOAD_DEFAULT);
    VRETURN(err == 0);

    err = FT_Render_Glyph(m_Face->glyph, FT_RENDER_MODE_NORMAL);
    VRETURN(err == 0);

    IZ_UINT size = m_Face->glyph->bitmap.pitch * m_Face->glyph->bitmap.rows;
    IZ_ASSERT(size == dstBytes);

    memcpy(dst, m_Face->glyph->bitmap.buffer, size);

    return IZ_TRUE;
}

void CFontConverterFT::WriteImage(
    const SOption& option,
    const SGlyphMetrics& metrics,
    void* src,
    void* dst,
    IZ_UINT pitch, IZ_UINT bpp,
    IZ_UINT x, IZ_UINT y,
    IZ_UINT leftOffset, IZ_UINT topOffset)
{
    IZ_UINT8* pSrc = reinterpret_cast<IZ_UINT8*>(src);

    IZ_UINT8* pDst = reinterpret_cast<IZ_UINT8*>(dst);
    IZ_UINT dstPitch = pitch * bpp;

    for (IZ_UINT dstY = 0; dstY < metrics.height; dstY++)
    {
        IZ_UINT8* ptrDst = pDst + (y + dstY + topOffset) * dstPitch;
        ptrDst += (x + leftOffset) * bpp;

        IZ_UINT8* ptrSrc = pSrc + dstY * metrics.width;
        
        memcpy(ptrDst, ptrSrc, metrics.width);
    }
}

IZ_UINT CFontConverterFT::GetTextMetricsHeight()
{
    return (m_Face->height >> 6);
}

IZ_UINT CFontConverterFT::GetTextAscender()
{
    return (m_Face->ascender >> 6);
}

IZ_UINT CFontConverterFT::GetTextDescender()
{
    return (m_Face->descender >> 6);
}
