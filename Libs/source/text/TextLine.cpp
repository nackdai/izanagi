#include "text/TextLine.h"
#include "text/FontHost.h"
#include "text/Paragraph.h"
#include "text/UString.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CLine::CLine()
    {
        m_ListItem.Init(this);
        m_FontHost = IZ_NULL;
        m_Texture = IZ_NULL;
        m_Height = 0;
    }

    CLine::~CLine()
    {
        SAFE_RELEASE(m_FontHost);
        SAFE_RELEASE(m_Texture);
    }

    IZ_UINT CLine::GetLineHeight()
    {
        return m_Height;
    }

    void CLine::Prepare(
        IZ_UINT height,
        IZ_UINT ascent,
        graph::CGraphicsDevice* device)
    {
        if (m_Texture == IZ_NULL)
        {
            m_Height = height;
            IZ_UINT width = GetLineWidth();

            if (width > 0)
            {
                m_Texture = device->CreateTexture(
                    width,
                    height,
                    1,
                    graph::E_GRAPH_PIXEL_FMT_A8,
                    graph::E_GRAPH_RSC_USAGE_STATIC);

                IZ_UINT8* data;
                IZ_UINT pitch = m_Texture->Lock(0, (void**)&data, IZ_FALSE);

                Prepare(
                    data,
                    pitch,
                    ascent,
                    m_FontHost);

                m_Texture->Unlock(0);
            }
        }
    }

    IZ_UINT CLine::CopyImage(
        IZ_UINT glyphID,
        IZ_UINT8* dst,
        IZ_UINT x,
        IZ_UINT pitch,
        IZ_UINT ascent)
    {
        IZ_UINT posX = x;
        IZ_UINT posY = ascent;

        text::SGlyphImage image;
        text::SGlyphMetrics metrics;
        m_FontHost->GetImageByID(
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
        IZ_INT x,
        IZ_INT y,
        graph::CGraphicsDevice* device)
    {
        if (m_Texture != IZ_NULL)
        {
            device->SetTexture(0, m_Texture);

            IZ_UINT width = m_Texture->GetWidth();
            IZ_UINT height = m_Texture->GetHeight();

            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(x, y, width, height));
        }
    }
}   // namespace text
}   // namespace izanagi
