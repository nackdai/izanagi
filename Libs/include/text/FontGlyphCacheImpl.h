#if !defined(__IZANAGI_TEXT_FONT_GLYPH_CACHE_IMPL_H__)
#define __IZANAGI_TEXT_FONT_GLYPH_CACHE_IMPL_H__

#include "izDefs.h"
#include "izStd.h"
#include "FontDefs.h"

#include "FontGlyphCache.h"

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

    /**
     */
    class CGlyphCacheImpl : public CGlyphCache
    {
    public:
        static CGlyphCacheImpl* CreateGlyphCache(
            IMemoryAllocator* allocator,
            graph::CGraphicsDevice* device,
            E_FONT_CHAR_ENCODE encode,
            IZ_UINT maxRegisterNum,
            IZ_UINT height,
            IZ_BOOL enableExchange);

    protected:
        CGlyphCacheImpl();
        virtual ~CGlyphCacheImpl();

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

#endif  // #if !defined(__IZANAGI_TEXT_FONT_GLYPH_CACHE_IMPL_H__)
