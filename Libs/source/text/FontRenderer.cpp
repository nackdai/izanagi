#include "text/FontRenderer.h"
#include "text/FontHostFT.h"
#include "text/FontHostFNT.h"
#include "text/ParagraphGroup.h"
#include "text/FontGlyphCache.h"
#include "text/UString.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CFontRenderer::CFontRenderer()
    {
        m_Allocator = IZ_NULL;
        m_FontHost = IZ_NULL;
    }

    CFontRenderer::~CFontRenderer()
    {
        SAFE_RELEASE(m_FontHost);
    }

    void CFontRenderer::Render(
        graph::CGraphicsDevice* device,
        IZ_UINT x, IZ_UINT y,
        IZ_UINT width, IZ_UINT height,
        CParagraphGroup& paragraphGroup)
    {
        paragraphGroup.Layout(width, height);
        paragraphGroup.Prepare(device);
        paragraphGroup.Render(x, y, device);
    }

    void CFontRenderer::Render(
        graph::CGraphicsDevice* device,
        CUString& str,
        CIntPoint& pos,
        IZ_UINT baseline,
        CGlyphCache& glyphCache)
    {
        glyphCache.Prepare(device);

        IZ_FLOAT width = static_cast<IZ_FLOAT>(device->GetTexture(0)->GetWidth());
        IZ_FLOAT height = static_cast<IZ_FLOAT>(device->GetTexture(0)->GetHeight());

        IZ_UINT code = 0;

        IZ_UINT posX = pos.x;
        IZ_UINT posY = pos.y;

        E_FONT_CHAR_ENCODE encode = m_FontHost->GetEncodeType();

        str.BeginIter();

        for (;;)
        {
            if (encode == E_FONT_CHAR_ENCODE_UNICODE)
            {
                code = str.GetNextAsUnicode();
            }
            else if (encode == str.GetCharCode())
            {
                code = str.GetNext();
            }
            else
            {
                IZ_ASSERT(IZ_FALSE);
            }

            if (code == 0)
            {
                break;
            }

            // TODO
            // 改行コード

            SGlyphCacheItem* item = glyphCache.FindCache(code);

            if (item == IZ_NULL)
            {
                // TODO
            }
            else
            {
                IZ_FLOAT left = item->rect.left / width;
                IZ_FLOAT right = item->rect.right / width;
                IZ_FLOAT top = item->rect.top / height;
                IZ_FLOAT bottom = item->rect.bottom / height;

                IZ_UINT w = item->rect.Width();
                IZ_UINT h = item->rect.Height();

                IZ_UINT x = posX + item->leftOffset;
                IZ_UINT y = posY + baseline - item->metrics.bearingY;

                device->Draw2DSprite(
                    izanagi::CFloatRect(left, top, right, bottom),
                    izanagi::CIntRect(x, y, x + w, y + h));

                posX += item->metrics.advance;
            }
        }

        pos.x = posX;
        pos.y = posY;

        str.EndIter();
    }

    ///////////////////////////////////////////////

    IZ_BOOL CFontRendererFT::Init(
        IInputStream* in,
        IZ_UINT pixelSize)
    {
        m_FontHost = CFontHostFT::CreateFontHostFT(
            m_Allocator,
            in,
            pixelSize);

        return (m_FontHost != IZ_NULL);
    }

    IZ_UINT CFontRendererFT::GetNextChar(CUString& str)
    {
        return str.GetNextAsUnicode();
    }

    ///////////////////////////////////////////////

    IZ_BOOL CFontRendererFNT::Init(
        IInputStream* in,
        IZ_UINT pixelSize)
    {
        m_FontHost = CFontHostFNT::CreateFontHostFNT(
            m_Allocator,
            in);

        return (m_FontHost != IZ_NULL);
    }

    IZ_UINT CFontRendererFNT::GetNextChar(CUString& str)
    {
        return str.GetNext();
    }
}    // namespace text
}    // namespace izanagi
