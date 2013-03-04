#include "text/TextLine.h"
#include "text/FontHost.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CLine::CLine()
    {
        m_ListItem.Init(this);
        m_Texture = IZ_NULL;
    }

    CLine::~CLine()
    {
        SAFE_RELEASE(m_Texture);
    }

    void CLine::Prepare(
        IZ_UINT height,
        IZ_UINT ascent,
        IFontHost* host,
        graph::CGraphicsDevice* device)
    {
        if (m_Texture == IZ_NULL)
        {
            IZ_UINT width = GetLineWidth();

            m_Texture = device->CreateTexture(
                width,
                height,
                1,
                graph::E_GRAPH_PIXEL_FMT_A8,
                graph::E_GRAPH_RSC_TYPE_STATIC);

            IZ_UINT8* data;
            IZ_UINT pitch = m_Texture->Lock(0, (void**)&data, IZ_FALSE);

            Prepare(
                data,
                pitch,
                ascent,
                host);

            m_Texture->Unlock(0);
        }
    }

    IZ_UINT CLine::CopyImage(
        IZ_UINT glyphID,
        IZ_UINT8* dst,
        IZ_UINT x,
        IZ_UINT pitch,
        IZ_UINT ascent,
        IFontHost* host)
    {
        IZ_UINT posX = x;
        IZ_UINT posY = ascent;

        text::SGlyphImage image;
        text::SGlyphMetrics metrics;
        host->GetImageByID(
            glyphID,
            image,
            metrics);

        for (IZ_UINT y = 0; y < image.rows; y++)
        {
            memcpy(
                dst + posX + (posY + y) * pitch + image.leftOffset - image.topOffset * pitch,
                image.bmp + y * image.pitch,
                image.pitch);
        }

        posX += metrics.advance;

        return posX;
    }

    void CLine::Clear()
    {
        SAFE_RELEASE(m_Texture);
    }

    void CLine::Render(
        IZ_UINT x,
        IZ_UINT y,
        graph::CGraphicsDevice* device)
    {
        device->SetTexture(0, m_Texture);

        IZ_UINT width = m_Texture->GetWidth();
        IZ_UINT height = m_Texture->GetHeight();

        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(x, y, x + width, y + height));
    }
}   // namespace text
}   // namespace izanagi