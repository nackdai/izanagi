#include "text/ParagraphGroup.h"
#include "text/Paragraph.h"
#include "text/FontHost.h"
#include "izGraph.h"

namespace izanagi
{
namespace text
{
    CParagraphGroup::CParagraphGroup()
    {
        m_Width = 0;
        m_Height = 0;
        
        m_LineHeight = 0;
        m_Ascent = 0;
    }

    void CParagraphGroup::Layout(
        IZ_UINT width,
        IZ_UINT height)
    {
        m_Height = height;

        if (m_Width == width)
        {
            // Don't layout if width is not changed.
            return;
        }

        // 2 is margin.
        IZ_UINT lineWidth = width - 2;

        CStdList<CParagraph>::Item* paragraphListItem = m_Paragraphs.GetTop();
        while (paragraphListItem != IZ_NULL)
        {
            CParagraph* paragraph = paragraphListItem->GetData();

            paragraph->Layout(lineWidth);

            paragraphListItem = paragraphListItem->GetNext();
        }
    }

    void CParagraphGroup::Prepare(
        IFontHost* host,
        graph::CGraphicsDevice* device)
    {
        CStdList<CParagraph>::Item* paragraphListItem = m_Paragraphs.GetTop();
        while (paragraphListItem != IZ_NULL)
        {
            CParagraph* paragraph = paragraphListItem->GetData();

            paragraph->Prepare(m_LineHeight, m_Ascent, host, device);

            paragraphListItem = paragraphListItem->GetNext();
        }
    }

    void CParagraphGroup::Render(
        IZ_UINT x,
        IZ_UINT y,
        graph::CGraphicsDevice* device)
    {
        CStdList<CParagraph>::Item* listItem = m_Paragraphs.GetTop();
        while (listItem != IZ_NULL)
        {
            CParagraph* paragraph = listItem->GetData();

            paragraph->Render(x, y, m_LineHeight, device);

            listItem = listItem->GetNext();
        }
    }

    void CParagraphGroup::AddParagraph(CParagraph* paragraph)
    {
        m_Paragraphs.AddTail(paragraph->GetListItem());
    }
}   // namespace text
}   // namespace izanagi
