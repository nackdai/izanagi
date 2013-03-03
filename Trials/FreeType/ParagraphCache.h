#if !defined(__IZANAGI_TEXT_PARAGRAPH_CACHE_H__)
#define __IZANAGI_TEXT_PARAGRAPH_CACHE_H__

#include "unicode/utypes.h"
#include "unicode/ubidi.h"

#include "layout/LEFontInstance.h"
#include "layout/ParagraphLayout.h"

#include "izDefs.h"
#include "izStd.h"
#include "izFont.h"
#include "izGraph.h"

namespace izanagi
{
    class IMemoryAllocator;

    namespace graph
    {
        class CTexture;
        class CGraphicsDevice;
    }

namespace text
{
    class CUString;
    class IFontHost;

    /**
     */
    class CLine : public CObject
    {
        friend class CParagraph;

    private:
        static CLine* CreateLine(
            IMemoryAllocator* allocator,
            const ParagraphLayout::Line* line);

    private:
        CLine();
        ~CLine();

        NO_COPIABLE(CLine);
        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        CStdList<CLine>::Item* GetListItem() { return &m_ListItem; }

        virtual void Prepare(
            IZ_UINT height,
            IZ_UINT ascent,
            IFontHost* host,
            graph::CGraphicsDevice* device);

        virtual void Render(
            IZ_UINT x,
            IZ_UINT y,
            graph::CGraphicsDevice* device);

    private:
        IMemoryAllocator* m_Allocator;

        const ParagraphLayout::Line* m_Line;

        CStdList<CLine>::Item m_ListItem;

        graph::CTexture* m_Texture;
    };

    class CParagraph : public CObject
    {
        friend class CParagraphGroup;

    private:
        static CParagraph* CreateParagraph(
            IMemoryAllocator* allocator,
            void* text,
            IZ_UINT bytes,
            ParagraphLayout* layout);

    private:
        CParagraph();
        ~CParagraph();

        NO_COPIABLE(CParagraph);
        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        CStdList<CParagraph>::Item* GetListItem()
        {
            return &m_ListItem;
        }

        virtual void Layout(IZ_UINT width);

        virtual void ReleaseLines();

        virtual void Prepare(
            IZ_UINT height,
            IZ_UINT ascent,
            IFontHost* host,
            graph::CGraphicsDevice* device);

        virtual void Render(
            IZ_UINT x,
            IZ_UINT y,
            IZ_UINT height,
            graph::CGraphicsDevice* device);

    private:
        IMemoryAllocator* m_Allocator;
        ParagraphLayout* m_Layout;
        void* m_Text;

        CStdList<CParagraph>::Item m_ListItem;
        CStdList<CLine> m_LineList;
    };

    /**
     */
    class CParagraphGroup : public CObject
    {
    public:
        static CParagraphGroup* CreateParagraphGroup(
            IMemoryAllocator* allocator,
            CUString& text,
            const LEFontInstance* font);

    protected:
        CParagraphGroup() {}
        ~CParagraphGroup() {}

        NO_COPIABLE(CParagraphGroup);
        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        virtual CParagraph* CreateParagraph(
            IMemoryAllocator* allocator,
            void* text,
            IZ_UINT bytes,
            void* data);

    public:
        virtual void Layout(
            IZ_UINT width,
            IZ_UINT height);

        virtual void Prepare(
            IFontHost* host,
            graph::CGraphicsDevice* device);

        virtual void Render(
            IZ_UINT x,
            IZ_UINT y,
            graph::CGraphicsDevice* device);

    private:
        void Init(CUString& text, const FontRuns* fontRuns);
       
    protected:
        IMemoryAllocator* m_Allocator;

        UBiDiLevel m_Level;

        CStdList<CParagraph> m_Paragraphs;

        IZ_UINT m_Width;
        IZ_UINT m_Height;

        IZ_UINT m_LineHeight;
        IZ_UINT m_Ascent;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_CACHE_H__)
