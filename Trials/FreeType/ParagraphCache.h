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

    /**
     */
    class CLine : public text::CLine
    {
        friend class CParagraph;

    private:
        static CLine* CreateLine(
            IMemoryAllocator* allocator,
            const ParagraphLayout::Line* line);

    private:
        CLine();
        virtual ~CLine();

        NO_COPIABLE(CLine);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        virtual IZ_UINT GetLineWidth();

    protected:
        virtual void Prepare(
            IZ_UINT8* dst,
            IZ_UINT pitch,
            IZ_UINT ascent,
            text::IFontHost* host);

    private:
        const ParagraphLayout::Line* m_Line;
    };

    /**
     */
    class CParagraph : public text::CParagraph
    {
        friend class text::CParagraph;
        friend class CParagraphGroup;

    private:
        CParagraph();
        ~CParagraph();

        NO_COPIABLE(CParagraph);
        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        virtual IZ_BOOL Init(void* userData);
        virtual CLine* CreateLine(IZ_UINT width);

    private:
        ParagraphLayout* m_Layout;
        IZ_BOOL m_IsReflowed;
    };

    /**
     */
    class CParagraphGroup : public text::CParagraphGroup
    {
        friend class izanagi::text::CParagraphGroup;
    protected:
        CParagraphGroup() {}
        ~CParagraphGroup() {}

        NO_COPIABLE(CParagraphGroup);
        IZ_DEFINE_INTERNAL_RELEASE();

    protected:
        CParagraph* CreateParagraph(
            IMemoryAllocator* allocator,
            void* text,
            IZ_UINT bytes,
            void* data);

    private:
        virtual IZ_BOOL Init(text::CUString& str, const void* userData);
        void InitInternal(text::CUString& text, const FontRuns* fontRuns);
       
    protected:
        UBiDiLevel m_Level;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_CACHE_H__)
