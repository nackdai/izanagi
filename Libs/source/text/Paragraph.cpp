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
        m_Width = 0;
        m_ListItem.Init(this);
    }

    CParagraph::~CParagraph()
    {
        Clear();
    }

    void CParagraph::Layout(IZ_UINT width)
    {
        if (m_Width != width)
        {
            ReleaseLines();

            BeginCreateLine();

            CLine* line = IZ_NULL;
            while ((line = CreateLine(width)) != IZ_NULL)
            {
                AddLine(line);
            }

            EndCreateLine();

            m_Width = width;
        }
    }

    IZ_UINT CParagraph::GetHeight()
    {
        IZ_UINT height = 0;

        CStdList<CLine>::Item* lineListItem = m_LineList.GetTop();
        while (lineListItem != IZ_NULL)
        {
            CLine* line = lineListItem->GetData();

            height += line->GetLineHeight();

            lineListItem = lineListItem->GetNext();
        }

        return height;
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
        IZ_UINT lineHeight,
        IZ_UINT ascent,
        graph::CGraphicsDevice* device)
    {
        IZ_UINT allHeight = 0;

        CStdList<CLine>::Item* lineListItem = m_LineList.GetTop();
        while (lineListItem != IZ_NULL)
        {
            if (allHeight + lineHeight > height)
            {
                break;
            }

            CLine* line = lineListItem->GetData();

            line->Prepare(lineHeight, ascent, device);
            allHeight += lineHeight;

            lineListItem = lineListItem->GetNext();
        }
    }

    IZ_INT CParagraph::Render(
        IZ_UINT height,
        IZ_INT x,
        IZ_INT y,
        graph::CGraphicsDevice* device)
    {
        IZ_UINT allHeight = 0;

        CStdList<CLine>::Item* listItem = this->m_LineList.GetTop();
        while (listItem != IZ_NULL)
        {
            CLine* line = listItem->GetData();
            IZ_UINT lineHeight = line->GetLineHeight();

            if (allHeight + lineHeight > height)
            {
                break;
            }

            line->Render(x, y, device);

            y += lineHeight;
            allHeight += lineHeight;

            listItem = listItem->GetNext();
        }

        return y;
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
        const void* text,
        IZ_UINT bytes,
        void* userData)
    {
        SAFE_REPLACE(m_FontHost, host);
        m_String.Init(encode, text, bytes);
        return IZ_TRUE;
    }

    void CDefaultParagraph::BeginCreateLine()
    {
        m_LastPos = 0;
        m_String.BeginIter();
    }

    void CDefaultParagraph::EndCreateLine()
    {
        m_String.EndIter();
    }

    CLine* CDefaultParagraph::CreateLine(IZ_UINT width)
    {
        E_FONT_CHAR_ENCODE encode = m_FontHost->GetEncodeType();

        CLine* ret = IZ_NULL;

        IZ_UINT code = 0;
        IZ_UINT advanced = 0;

        const void* ptr = m_String.GetIterPtr();

        for (IZ_UINT count = 0; ; count++)
        {
            if (encode == E_FONT_CHAR_ENCODE_UNICODE)
            {
                code = m_String.GetCurrentAsUnicode();
            }
            else
            {
                code = m_String.GetCurrent();
            }

            IZ_BOOL isFinishLine = IZ_FALSE;

            if (code == 0)
            {
                isFinishLine = IZ_TRUE;
            }
            else
            {
                IZ_UINT id = m_FontHost->GetGlyphID(code);

                SGlyphMetrics metrics;

                if (m_FontHost->GetGlyphMetricsByID(id, metrics))
                {
                    if (advanced + metrics.advance >= width)
                    {
                        isFinishLine = IZ_TRUE;;
                    }
                    else
                    {
                        advanced += metrics.advance;
                        m_String.GetNext();
                    }
                }
                else
                {
                    // TODO
                }
            }

            if (isFinishLine)
            {
                if (count > 0)
                {
                    IZ_UINT pos = static_cast<IZ_UINT>(m_String.GetIterDistance());
                    IZ_UINT bytes = pos - m_LastPos;
                    m_LastPos = pos;

                    SLineParam param;
                    {
                        param.encode = m_String.GetCharCode();
                        param.text = ptr;
                        param.bytes = bytes;
                    }

                    ret = CDefaultLine::CreateLine(
                        m_Allocator,
                        m_FontHost,
                        &param);
                }

                break;
            }
        }

        return ret;
    }
}   // namespace text
}   // namespace izanagi
