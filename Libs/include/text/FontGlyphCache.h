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
        IZ_UINT16 leftOffset;
        IZ_UINT16 topOffset;
        CIntRect rect;
    };

    class CGlyphCacheBase : public CObject
    {   
    protected:
        CGlyphCacheBase() {}
        ~CGlyphCacheBase() {}

        NO_COPIABLE(CGlyphCacheBase);
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
    };

    class CGlyphCache : public CGlyphCacheBase
    {
    public:
        static CGlyphCache* CreateGlyphCache(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            IZ_UINT maxRegisterNum,
            IZ_UINT height,
            IZ_BOOL enableExchange);

    protected:
        CGlyphCache();
        virtual ~CGlyphCache();

    protected:
        IZ_BOOL CreateFontMap(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            IZ_UINT maxRegisterNum,
            IZ_UINT height);

        virtual void Unregister(IZ_UINT code);

        virtual IZ_BOOL Prepare(graph::CGraphicsDevice* device);

        virtual void BeginRegister();
        virtual void EndRegister();

        virtual void Clear();

        virtual SGlyphCacheItem* CreateCacheItem(IZ_UINT code);
        virtual SGlyphCacheItem* FindCache(IZ_UINT code);
        virtual void UnregisterCache(SGlyphCacheItem* item);
        virtual IZ_BOOL Regsiter(SGlyphCacheItem* item, const SGlyphImage& image);

    private:
        graph::CTexture* m_FontMap;

        IZ_UINT m_MaxRegisterNum;
        IZ_UINT m_FontHeight;
        IZ_BOOL m_EnableExchange;

        IZ_UINT m_RegisteredNum;

        IZ_UINT m_FontMapPitch;
        IZ_UINT8* m_FontMapData;
        IZ_UINT m_PosX;
        IZ_UINT m_PosY;

        static const IZ_UINT CHAR_MARGIN = 1;

        struct SGlyphCacheItemImpl;
        static const IZ_UINT HASH_NUM = 5;
        typedef CStdHash<IZ_UINT, SGlyphCacheItemImpl, HASH_NUM> Hash;

        struct SGlyphCacheItemImpl : public SGlyphCacheItem
        {
            IZ_UINT idx;
            Hash::Item hashItem;
        };

        IZ_UINT m_CachePos;
        SGlyphCacheItemImpl* m_CacheBuffer;

        Hash m_Hash;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_GLYPH_CACHE_H__)
