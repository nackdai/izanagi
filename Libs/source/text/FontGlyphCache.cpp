#include "text/FontGlyphCache.h"
#include "text/UString.h"
#include "text/FontHostFT.h"
#include "izGraph.h"
#include "izMath.h"

namespace izanagi
{
namespace text
{
    SGlyphCacheItem* CGlyphCache::Register(
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

    IZ_BOOL CGlyphCache::Register(
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

    IZ_BOOL CGlyphCache::IsRegistered(IZ_UINT code)
    {
        SGlyphCacheItem* item = FindCache(code);
        return (item != IZ_NULL);
    }
}    // namespace text
}    // namespace izanagi
