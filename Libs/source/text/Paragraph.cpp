#include "text/Paragraph.h"
#include "text/FontHost.h"
#include "text/TextLine.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CParagraph::CParagraph()
    {
        m_ListItem.Init(this);
    }

    CParagraph::~CParagraph()
    {
        Clear();
    }

    void CParagraph::Layout(IZ_UINT width)
    {
        ReleaseLines();

        BeginCreateLine();

        CLine* line = IZ_NULL;
        while ((line = CreateLine(width)) != IZ_NULL)
        {
            AddLine(line);
        }

        EndCreateLine();
    }

    void CParagraph::ReleaseLines()
    {
        CStdList<CLine>::Item* lineListItem = m_LineList.GetTop();
        while (lineListItem != IZ_NULL)
        {
            CLine* line = lineListItem->GetData();
            lineListItem = lineListItem->GetNext();

            SAFE_RELEASE(line);
        }
    }

    void CParagraph::AddLine(CLine* line)
    {
        m_LineList.AddTail(line->GetListItem());
    }

    void CParagraph::Prepare(
        IZ_UINT height,
        IZ_UINT ascent,
        graph::CGraphicsDevice* device)
    {
        CStdList<CLine>::Item* lineListItem = m_LineList.GetTop();
        while (lineListItem != IZ_NULL)
        {
            CLine* line = lineListItem->GetData();

            line->Prepare(height, ascent, device);

            lineListItem = lineListItem->GetNext();
        }
    }

    void CParagraph::Render(
        IZ_UINT x,
        IZ_UINT y,
        IZ_UINT height,
        graph::CGraphicsDevice* device)
    {
        CStdList<CLine>::Item* listItem = this->m_LineList.GetTop();
        while (listItem != IZ_NULL)
        {
            CLine* line = listItem->GetData();

            line->Render(x, y, device);
            y += height;

            listItem = listItem->GetNext();
        }
    }

    void CParagraph::Clear()
    {
        CStdList<CLine>::Item* lineListItem = m_LineList.GetTop();
        while (lineListItem != IZ_NULL)
        {
            CLine* line = lineListItem->GetData();
            lineListItem = lineListItem->GetNext();
            SAFE_RELEASE(line);
        }
    }

    /////////////////////////////////////////////////////////

    CDefaultParagraph::CDefaultParagraph()
    {
        m_FontHost = IZ_NULL;
    }

    CDefaultParagraph::~CDefaultParagraph()
    {
        SAFE_RELEASE(m_FontHost);
    }

    IZ_BOOL CDefaultParagraph::Init(
        IFontHost* host,
        E_FONT_CHAR_ENCODE encode,
        void* text,
        IZ_UINT bytes,
        void* userData)
    {
        m_String.Init(text, bytes);
        return IZ_TRUE;
    }

    void CDefaultParagraph::BeginCreateLine()
    {
        m_String.BeginIter();
    }

    void CDefaultParagraph::EndCreateLine()
    {
        m_String.EndIter();
    }

    CLine* CDefaultParagraph::CreateLine(IZ_UINT width)
    {
        E_FONT_CHAR_ENCODE encode = m_String.GetCharCode();

        CLine* ret = IZ_NULL;

        IZ_UINT code = 0;
        IZ_UINT advanced = 0;

        const void* ptr = m_String.GetIterPtr();

        for (;;)
        {
            if (encode == E_FONT_CHAR_ENCODE_UNICODE)
            {
                code = m_String.GetNextAsUnicode();
            }
            else
            {
                code = m_String.GetNext();
            }

            if (code == 0)
            {
                break;
            }

            IZ_UINT id = m_FontHost->GetGlyphID(code);

            SGlyphMetrics metrics;

            if (m_FontHost->GetGlyphMetricsByID(id, metrics))
            {
                if (advanced + metrics.advance >= width)
                {
                    SLineParam param;
                    {
                        param.text = ptr;
                        param.bytes = static_cast<IZ_UINT>(m_String.GetIterDistance());
                    }

                    ret = CDefaultLine::CreateLine(
                        m_Allocator,
                        m_FontHost,
                        &param);

                    break;
                }
                advanced += metrics.advance;
            }
            else
            {
                // TODO
            }
        }

        return ret;
    }
}   // namespace text
}   // namespace izanagi
