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
}   // namespace text
}   // namespace izanagi
