#include "text/FontGlyphCache.h"
#include "text/UString.h"
#include "text/FontHostFT.h"
#include "izGraph.h"
#include "izMath.h"

namespace izanagi
{
namespace text
{
    SGlyphCacheItem* CGlyphCacheBase::Register(
        IZ_UINT code,
        const SGlyphMetrics& metrics,
        const SGlyphImage& image)
    {
        SGlyphCacheItem* item = FindCache(code);
        IZ_BOOL isRegistered = (item != IZ_NULL);

        if (isRegistered)
        {
            Regsiter(item, image);
        }
        else
        {
            item = CreateCacheItem(code);
            isRegistered = (item != IZ_NULL);

            if (isRegistered)
            {
                item->code = code;

                {
                    item->metrics.advance = metrics.advance;
                    item->metrics.bearingX = metrics.bearingX;
                    item->metrics.bearingY = metrics.bearingY;
                    item->metrics.width = metrics.width;
                    item->metrics.height = metrics.height;
                }

                isRegistered = Regsiter(item, image);
            }
        }

        return item;
    }

    IZ_BOOL CGlyphCacheBase::Register(
        CUString* string,
        IFontHost* host)
    {
        IZ_BOOL ret = IZ_TRUE;

        string->BeginIter();

        IZ_UINT code = 0;
        for (;;)
        {
            if (m_Encode == E_FONT_CHAR_ENCODE_UNICODE)
            {
                code = string->GetNextAsUnicode();
            }
            else
            {
                code = string->GetNext();
            }

            if (code == 0)
            {
                break;
            }

            SGlyphImage image;
            SGlyphMetrics metrics;

            if (!host->GetImage(code, image, metrics))
            {
                continue;
            }

            SGlyphCacheItem* item = Register(code, metrics, image);
            if (item == IZ_NULL)
            {
                ret = IZ_FALSE;
                break;
            }
        }

        string->EndIter();

        return ret;
    }

    IZ_BOOL CGlyphCacheBase::IsRegistered(IZ_UINT code)
    {
        SGlyphCacheItem* item = FindCache(code);
        return (item != IZ_NULL);
    }

    /////////////////////////////////////////////////

    CGlyphCache* CGlyphCache::CreateGlyphCache(
        IMemoryAllocator* allocator,
        graph::CGraphicsDevice* device,
        E_FONT_CHAR_ENCODE encode,
        IZ_UINT maxRegisterNum,
        IZ_UINT height,
        IZ_BOOL enableExchange)
    {
        size_t size = sizeof(CGlyphCache);
        size += sizeof(SGlyphCacheItemImpl) * maxRegisterNum;

        IZ_UINT8* buf = (IZ_UINT8*)ALLOC(allocator, size);

        CGlyphCache* instance = new (buf) CGlyphCache();
        {
            buf += sizeof(CGlyphCache);

            instance->AddRef();

            instance->m_Allocator = allocator;

            instance->m_Encode = encode;

            instance->m_MaxRegisterNum = maxRegisterNum;
            instance->m_FontHeight = height;
            instance->m_EnableExchange = enableExchange;

            instance->m_CacheBuffer = reinterpret_cast<SGlyphCacheItemImpl*>(buf);

            instance->CreateFontMap(
                allocator,
                device,
                maxRegisterNum,
                height);
        }

        return instance;
    }

    CGlyphCache::CGlyphCache()
    {
        m_RegisteredNum = 0;

        m_FontMapPitch = 0;
        m_FontMapData = IZ_NULL;
        m_PosX = CHAR_MARGIN;
        m_PosY = CHAR_MARGIN;

        m_CachePos = 0;
    }

    CGlyphCache::~CGlyphCache()
    {
        SAFE_RELEASE(m_FontMap);
    }

    IZ_BOOL CGlyphCache::CreateFontMap(
        IMemoryAllocator* allocator,
        graph::CGraphicsDevice* device,
        IZ_UINT maxRegisterNum,
        IZ_UINT height)
    {
        IZ_BOOL ret = IZ_TRUE;

        // 縦横のフォント数
        IZ_UINT fontNumH = static_cast<IZ_UINT>(sqrtf((IZ_FLOAT)maxRegisterNum) + 0.5f);
        IZ_UINT fontNumV = static_cast<IZ_UINT>(static_cast<IZ_FLOAT>(maxRegisterNum) / fontNumH + 0.5f);
        while (fontNumH * fontNumV < maxRegisterNum)
        {
            if (fontNumH < fontNumV)
            {
                fontNumH++;
            }
            else
            {
                fontNumV++;
            }
        }

        // テクスチャサイズ
        IZ_UINT texWidth = fontNumH * height;
        IZ_UINT texHeight = fontNumV * height;

        texWidth += fontNumH + CHAR_MARGIN * 2;
        texHeight += fontNumV + CHAR_MARGIN * 2;

#if 1
        texWidth = math::CMath::ComputeNextPow2(texWidth);
        texHeight = math::CMath::ComputeNextPow2(texHeight);
#endif

        // TODO
        // テクスチャサイズ

        m_FontMap = device->CreateTexture(
           texWidth,
           texHeight,
           1,
           izanagi::graph::E_GRAPH_PIXEL_FMT_A8,
           izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);

        IZ_ASSERT(m_FontMap != IZ_NULL);
    
        return (m_FontMap != IZ_NULL);
    }

    void CGlyphCache::Unregister(IZ_UINT code)
    {
        Hash::Item* hashItem = m_Hash.Find(code);
        if (hashItem != IZ_NULL)
        {
            hashItem->Leave();
        }
    }

    IZ_BOOL CGlyphCache::Prepare(graph::CGraphicsDevice* device)
    {
        IZ_ASSERT(device != IZ_NULL);
        return device->SetTexture(0, m_FontMap);
    }

    void CGlyphCache::BeginRegister()
    {
        IZ_ASSERT(m_FontMap != IZ_NULL);
        IZ_ASSERT(m_FontMapData == IZ_NULL);

        m_FontMapPitch = m_FontMap->Lock(
            0,
            (void**)&m_FontMapData,
            IZ_FALSE);
    }

    void CGlyphCache::EndRegister()
    {
        IZ_ASSERT(m_FontMap != IZ_NULL);
        m_FontMap->Unlock(0);
        m_FontMapData = IZ_NULL;
    }

    void CGlyphCache::Clear()
    {
        m_PosX = CHAR_MARGIN;
        m_PosY = CHAR_MARGIN;

        m_CachePos = 0;

        m_RegisteredNum = 0;

        m_Hash.Clear();
    }

    SGlyphCacheItem* CGlyphCache::CreateCacheItem(IZ_UINT code)
    {
        SGlyphCacheItemImpl* item = (SGlyphCacheItemImpl*)FindCache(code);

        if (item == IZ_NULL)
        {
            if (m_RegisteredNum < m_MaxRegisterNum)
            {
                item = &m_CacheBuffer[m_CachePos];
                IZ_ASSERT(item != IZ_NULL);

                item->idx = m_CachePos;
                m_CachePos++;
            }
            else if (m_EnableExchange)
            {
                // 先頭にあるものが最も参照されていないもの
                Hash::Item* hashItem = m_Hash.GetOrderTopHashItem();
                hashItem->Leave();

                item = hashItem->GetData();
            }

            if (item != IZ_NULL)
            {
                item->hashItem.Init(
                    code,
                    item);
            }
        }

        return item;
    }

    SGlyphCacheItem* CGlyphCache::FindCache(IZ_UINT code)
    {
        SGlyphCacheItemImpl* item = m_Hash.FindData(code);
        return item;
    }

    void CGlyphCache::UnregisterCache(SGlyphCacheItem* item)
    {
        SGlyphCacheItemImpl* realItem = (SGlyphCacheItemImpl*)item;
        realItem->hashItem.Leave();
    }
    
    IZ_BOOL CGlyphCache::Regsiter(
        SGlyphCacheItem* item,
        const SGlyphImage& image)
    {
        SGlyphCacheItemImpl* realItem = (SGlyphCacheItemImpl*)item;

        IZ_BOOL isRegistered = realItem->hashItem.HasList();

        // 最後に参照されたものはリストの最後にくるようにする
        UnregisterCache(item);

        if (m_RegisteredNum < m_MaxRegisterNum)
        {
            if (isRegistered)
            {
                // 元々登録済みだったので、最後にくるようだけにする
                m_Hash.Add(&realItem->hashItem);
                return IZ_TRUE;
            }
        }

        IZ_ASSERT(m_FontMapData != IZ_NULL);

        if ((m_PosX + item->metrics.advance) >= m_FontMap->GetWidth())
        {
            IZ_ASSERT(m_PosY <= m_FontMap->GetHeight());

            m_PosX = CHAR_MARGIN;
            m_PosY += m_FontHeight + CHAR_MARGIN;
        }

        // TODO
        if (m_PosY + m_FontHeight - image.topOffset + image.rows > m_FontMap->GetHeight())
        {
            // はみ出る・・・
            IZ_ASSERT(IZ_FALSE);
            return IZ_FALSE;
        }

        for (IZ_UINT y = 0; y < image.rows; y++)
        {
#if 0
            IZ_UINT pos = m_PosX + (m_PosY + y + m_FontHeight) * m_FontMapPitch;
            //pos += image.leftOffset - image.rows * m_FontMapPitch;
            pos += image.leftOffset - image.topOffset * m_FontMapPitch;
            //pos += - image.rows * m_FontMapPitch;
#else
            IZ_UINT pos = m_PosX + (m_PosY + y) * m_FontMapPitch;
#endif

            memcpy(
                m_FontMapData + pos,
                image.bmp + y * image.pitch,
                image.rowBytes);
        }

        realItem->rect.left = m_PosX;
        realItem->rect.top = m_PosY;
        realItem->rect.SetWidth(item->metrics.advance);
        realItem->rect.SetHeight(m_FontHeight);

        realItem->leftOffset = image.leftOffset;
        realItem->topOffset = image.topOffset;

        m_PosX += item->metrics.advance + CHAR_MARGIN;

        m_Hash.Add(&realItem->hashItem);
        m_RegisteredNum++;

        return IZ_TRUE;
    }
}    // namespace text
}    // namespace izanagi
