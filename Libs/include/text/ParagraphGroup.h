#if !defined(__IZANAGI_TEXT_PARAGRAPH_GROUP_H__)
#define __IZANAGI_TEXT_PARAGRAPH_GROUP_H__

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
    class CParagraph;

    /**
     */
    class CParagraphGroup : public CObject
    {
    protected:
        CParagraphGroup();
        virtual ~CParagraphGroup() {}

        NO_COPIABLE(CParagraphGroup);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void Layout(
            IZ_UINT width,
            IZ_UINT height);

        void Prepare(
            IFontHost* host,
            graph::CGraphicsDevice* device);

        void Render(
            IZ_UINT x,
            IZ_UINT y,
            graph::CGraphicsDevice* device);

    protected:
        void AddParagraph(CParagraph* paragraph);
       
    protected:
        IMemoryAllocator* m_Allocator;

        CStdList<CParagraph> m_Paragraphs;

        IZ_UINT m_Width;
        IZ_UINT m_Height;

        IZ_UINT m_LineHeight;
        IZ_UINT m_Ascent;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_GROUP_H__)
