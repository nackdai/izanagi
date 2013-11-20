#include "text/ParagraphGroup.h"
#include "text/Paragraph.h"
#include "text/FontHost.h"
#include "text/UString.h"
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

    CParagraphGroup::~CParagraphGroup()
    {
        CStdList<CParagraph>::Item* item = m_Paragraphs.GetTop();
        while (item != IZ_NULL)
        {
            CParagraph* paragraph = item->GetData();
            item = item->GetNext();
            SAFE_RELEASE(paragraph);
        }
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

        m_Width = width;
    }

    void CParagraphGroup::Prepare(graph::CGraphicsDevice* device)
    {
        IZ_UINT prevHeight = 0;

        CStdList<CParagraph>::Item* paragraphListItem = m_Paragraphs.GetTop();
        while (paragraphListItem != IZ_NULL)
        {
            if (m_Height < prevHeight)
            {
                break;
            }

            CParagraph* paragraph = paragraphListItem->GetData();

            paragraph->Prepare(
                m_Height - prevHeight,
                m_LineHeight, 
                m_Ascent, 
                device);

            prevHeight += paragraph->GetHeight();

            paragraphListItem = paragraphListItem->GetNext();
        }
    }

    void CParagraphGroup::Render(
        IZ_INT x,
        IZ_INT y,
        graph::CGraphicsDevice* device)
    {
        IZ_UINT prevHeight = 0;
        IZ_INT posY = y;

        CStdList<CParagraph>::Item* listItem = m_Paragraphs.GetTop();
        while (listItem != IZ_NULL)
        {
            if (m_Height < prevHeight)
            {
                break;
            }

            CParagraph* paragraph = listItem->GetData();

            posY = paragraph->Render(
                m_Height - prevHeight,
                x,
                posY,
                device);

            prevHeight += paragraph->GetHeight();

            listItem = listItem->GetNext();
        }
    }

    void CParagraphGroup::AddParagraph(CParagraph* paragraph)
    {
        m_Paragraphs.AddTail(paragraph->GetListItem());
    }
}   // namespace text
}   // namespace izanagi
