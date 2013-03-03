#if !defined(__IZANAGI_TEXT_PARAGRAPH_H__)
#define __IZANAGI_TEXT_PARAGRAPH_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
    namespace graph
    {
        class CGraphicsDevice;
    }

namespace text
{
    class IFontHost;
    class CLine;

    /**
     */
    class CParagraph : public CObject
    {
        friend class CParagraphGroup;

    protected:
        CParagraph();
        virtual ~CParagraph();

        NO_COPIABLE(CParagraph);
        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        PURE_VIRTUAL(void Layout(IZ_UINT width));

        void ReleaseLines();
        void AddLine(CLine* line);

        void Prepare(
            IZ_UINT height,
            IZ_UINT ascent,
            IFontHost* host,
            graph::CGraphicsDevice* device);

        void Render(
            IZ_UINT x,
            IZ_UINT y,
            IZ_UINT height,
            graph::CGraphicsDevice* device);

        void Clear();

    private:
        CStdList<CParagraph>::Item* GetListItem()
        {
            return &m_ListItem;
        }

    protected:
        IMemoryAllocator* m_Allocator;

        CStdList<CParagraph>::Item m_ListItem;
        CStdList<CLine> m_LineList;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_H__)
