#include "text/FontMap.h"

namespace izanagi
{
namespace text
{
    CFontMap* CFontMap::CreateFontMap(
        IMemoryAllocator* allocator,
        IZ_UINT maxMapNum)
    {
        size_t size = sizeof(CFontMap);
        size += sizeof(SMapItem) * maxMapNum;

        IZ_UINT8* buf = (IZ_UINT8*)ALLOC(allocator, size);
        VRETURN_NULL(buf);

        CFontMap* instance = new(buf) CFontMap();
        {
            buf += sizeof(CFontMap);

            instance->m_Allocator = allocator;
            instance->m_MapItemMax = maxMapNum;
            instance->m_MapItems = reinterpret_cast<SMapItem*>(buf);
        }

        return instance;
    }

    CFontMap::CFontMap()
    {
        m_Allocator = IZ_NULL;
        m_MapItemPos = 0;
        m_MapItems = IZ_NULL;
    }

    CFontMap::~CFontMap()
    {
        Clear();

        FREE(m_Allocator, m_MapItems);
    }

    IZ_UINT CFontMap::Register(
        const char* name,
        IFontHost* fontHost)
    {
        SMapItem* mapItem = GetMapItem();
        if (mapItem == IZ_NULL)
        {
            IZ_ASSERT(IZ_FALSE);
            return 0;
        }

        IZ_UINT key = CKey::GenerateKey(name);

        mapItem->name = name;
        SAFE_REPLACE(mapItem->fontHost, fontHost);
        mapItem->item.Init(key, mapItem);

        m_FontMapHash.Add(&mapItem->item);

        return key;
    }

    IZ_BOOL CFontMap::Register(
        IZ_UINT key,
        IFontHost* fontHost)
    {
        SMapItem* mapItem = GetMapItem();
        if (mapItem == IZ_NULL)
        {
            IZ_ASSERT(IZ_FALSE);
            return IZ_FALSE;
        }

        mapItem->name = IZ_NULL;
        SAFE_REPLACE(mapItem->fontHost, fontHost);
        mapItem->item.Init(key, mapItem);

        m_FontMapHash.Add(&mapItem->item);

        return IZ_TRUE;
    }

    IFontHost* CFontMap::Find(const char* name)
    {
        IZ_UINT key = CKey::GenerateKey(name);
        return Find(key);
    }

    IFontHost* CFontMap::Find(IZ_UINT key)
    {
        FontMapHash::Item* item = m_FontMapHash.Find(key);
        return item->GetData()->fontHost;
    }

    IZ_BOOL CFontMap::HasFontHost(const char* name)
    {
        IZ_UINT key = CKey::GenerateKey(name);
        return HasFontHost(key);
    }

    IZ_BOOL CFontMap::HasFontHost(IZ_UINT key)
    {
        FontMapHash::Item* item = m_FontMapHash.Find(key);
        return (item != IZ_NULL);
    }

    IZ_BOOL CFontMap::Remove(const char* name)
    {
        IZ_UINT key = CKey::GenerateKey(name);
        return Remove(key);
    }

    IZ_BOOL CFontMap::Remove(IZ_UINT key)
    {
        SMapItem* mapItem = m_FontMapHash.FindData(key);

        IZ_BOOL ret = (mapItem != IZ_NULL);

        if (ret)
        {
            SAFE_RELEASE(mapItem->fontHost);
        }

        return ret;
    }

    void CFontMap::Clear()
    {
        FontMapList::Item* item = m_FontMapHash.GetOrderTop();
        while (item != IZ_NULL)
        {
            FontMapHash::Item* hashItem = item->GetData();
            SMapItem* mapItem = hashItem->GetData();
            SAFE_RELEASE(mapItem->fontHost);

            item = item->GetNext();
        }

        m_MapItemPos = 0;
    }

    CFontMap::SMapItem* CFontMap::GetMapItem()
    {
        if (m_MapItemPos < m_MapItemMax)
        {
            return &m_MapItems[m_MapItemPos++];
        }
        else
        {
            for (IZ_UINT i = 0; i < m_MapItemPos; i++)
            {
                if (m_MapItems[i].fontHost == IZ_NULL)
                {
                    return &m_MapItems[i];
                }
            }
        }

        return IZ_NULL;
    }

    IZ_UINT CFontMap::GetPixelSize()
    {
        IZ_UINT ret = 0;

        FontMapList::Item* item = m_FontMapHash.GetOrderTop();
        while (item != IZ_NULL)
        {
            FontMapHash::Item* hashItem = item->GetData();
            SMapItem* mapItem = hashItem->GetData();

            IZ_UINT size = mapItem->fontHost->GetPixelSize();
            ret = IZ_MAX(ret, size);
        }

        return ret;
    }
}    // namespace text
}   // namespace izanagi
