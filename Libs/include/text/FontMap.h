#if !defined(__IZANAGI_TEXT_FONT_MAP_H__)
#define __IZANAGI_TEXT_FONT_MAP_H__

#include "izDefs.h"
#include "izStd.h"
#include "FontHost.h"

namespace izanagi
{
namespace text
{
    /**
     * フォントマップ
     */
    class CFontMap : public CObject
    {
        static const IZ_UINT HashNum = 5;

        struct SMapItem;
        typedef CStdHash<IZ_UINT, SMapItem, HashNum> FontMapHash;
        typedef CStdList<FontMapHash::Item> FontMapList;

        struct SMapItem
        {
            const char* name;
            IFontHost* fontHost;
            FontMapHash::Item item;
        };

    public:
        static CFontMap* CreateFontMap(
            IMemoryAllocator* allocator,
            IZ_UINT maxMapNum);

    private:
        CFontMap();
        virtual ~CFontMap();

        NO_COPIABLE(CFontMap);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_UINT Register(
            const char* name,
            IFontHost* fontHost);

        virtual IZ_BOOL Register(
            IZ_UINT key,
            IFontHost* fontHost);

        IFontHost* Find(const char* name);
        virtual IFontHost* Find(IZ_UINT key);

        IZ_BOOL HasFontHost(const char* name);
        virtual IZ_BOOL HasFontHost(IZ_UINT key);

        IZ_BOOL Remove(const char* name);
        virtual IZ_BOOL Remove(IZ_UINT key);

        void Clear();

    public:
        IZ_UINT GetPixelSize();

    private:
        SMapItem* GetMapItem();

    protected:
        IMemoryAllocator* m_Allocator;

        FontMapHash m_FontMapHash;

        IZ_UINT m_MapItemMax;
        IZ_UINT m_MapItemPos;
        SMapItem* m_MapItems;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_MAP_H__)
