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
        IFontHost* host,
        graph::CGraphicsDevice* device)
    {
        CStdList<CLine>::Item* lineListItem = m_LineList.GetTop();
        while (lineListItem != IZ_NULL)
        {
            CLine* line = lineListItem->GetData();

            line->Prepare(height, ascent, host, device);

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
}   // namespace text
}   // namespace izanagi
