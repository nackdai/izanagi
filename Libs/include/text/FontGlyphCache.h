#if !defined(__IZANAGI_TEXT_FONT_GLYPH_CACHE_H__)
#define __IZANAGI_TEXT_FONT_GLYPH_CACHE_H__

#include "izDefs.h"
#include "izStd.h"
#include "FontDefs.h"

namespace izanagi
{
    class IMemoryAllocator;

    namespace graph
    {
        class CTexture;
        class CGraphicsDevice;
    }

namespace text
{
    class CUString;
    class IFontHost;

    struct SGlyphCacheItem
    {
        IZ_UINT code;
        SGlyphMetrics metrics;
        IZ_INT16 leftOffset;
        IZ_INT16 topOffset;
        CIntRect rect;
    };

    class CGlyphCache : public CObject
    {   
    protected:
        CGlyphCache()
        {
            m_Encode = E_FONT_CHAR_ENCODE_UNICODE;
        }
        ~CGlyphCache() {}

        NO_COPIABLE(CGlyphCache);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        SGlyphCacheItem* Register(
            IZ_UINT code,
            const SGlyphMetrics& metrics,
            const SGlyphImage& image);

        IZ_BOOL Register(
            CUString* string,
            IFontHost* host);

        PURE_VIRTUAL(void Unregister(IZ_UINT code));

        PURE_VIRTUAL(IZ_BOOL Prepare(graph::CGraphicsDevice* device));

        IZ_BOOL IsRegistered(IZ_UINT code);

        PURE_VIRTUAL(SGlyphCacheItem* FindCache(IZ_UINT code));

        PURE_VIRTUAL(void BeginRegister());
        PURE_VIRTUAL(void EndRegister());

        PURE_VIRTUAL(void Clear());

    protected:
        PURE_VIRTUAL(SGlyphCacheItem* CreateCacheItem(IZ_UINT code));
        PURE_VIRTUAL(void UnregisterCache(SGlyphCacheItem* item));
        PURE_VIRTUAL(IZ_BOOL Regsiter(SGlyphCacheItem* item, const SGlyphImage& image));

    protected:
        IMemoryAllocator* m_Allocator;
        E_FONT_CHAR_ENCODE m_Encode;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_GLYPH_CACHE_H__)
