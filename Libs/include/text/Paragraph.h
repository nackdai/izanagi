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

    public:
        template <typename _T>
        static CParagraph* CreateParagraph(
            IMemoryAllocator* allocator,
            void* text,
            IZ_UINT bytes,
            void* userData)
        {
            IZ_BOOL result = IZ_TRUE;
            IZ_UINT8* buf = (IZ_UINT8*)ALLOC(allocator, sizeof(_T) + bytes + 1);

            CParagraph* paragraph = new(buf) _T;
            {
                buf += sizeof(_T);

                paragraph->AddRef();

                paragraph->m_Allocator = allocator;

                paragraph->m_Text = buf;
                buf = (IZ_UINT8*)paragraph->m_Text;
                buf[bytes] = 0;
                memcpy(
                    paragraph->m_Text,
                    text,
                    bytes);

                result = paragraph->Init(userData);
            }

            if (!result)
            {
                SAFE_RELEASE(paragraph);
            }

            return paragraph;
        }

    protected:
        CParagraph();
        virtual ~CParagraph();

        NO_COPIABLE(CParagraph);
        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        PURE_VIRTUAL(IZ_BOOL Init(void* userData));

        void Layout(IZ_UINT width);

        PURE_VIRTUAL(CLine* CreateLine(IZ_UINT width));

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

        void* m_Text;

        CStdList<CParagraph>::Item m_ListItem;
        CStdList<CLine> m_LineList;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_H__)
