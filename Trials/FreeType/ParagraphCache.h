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
        friend class izanagi::text::CLine;

    private:
        static CLine* CreateLine(
            IMemoryAllocator* allocator,
            izanagi::text::IFontHost* host,
            ParagraphLayout::Line* line);

    private:
        CLine();
        virtual ~CLine();

        NO_COPIABLE(CLine);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        virtual IZ_UINT GetLineWidth();

    protected:
        virtual IZ_BOOL Init(void* userData)
        {
            m_Line = (ParagraphLayout::Line*)userData;
            return IZ_TRUE;
        }

        virtual void Prepare(
            IZ_UINT8* dst,
            IZ_UINT pitch,
            IZ_UINT ascent,
            text::IFontHost* host);

    private:
        ParagraphLayout::Line* m_Line;
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
        virtual CLine* CreateLine(IZ_UINT width);

        virtual IZ_BOOL Init(
            izanagi::text::IFontHost* host,
            izanagi::text::E_FONT_CHAR_ENCODE encode,
            void* text,
            IZ_UINT bytes,
            void* userData);

    private:
        izanagi::text::IFontHost* m_Host;
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
            izanagi::text::IFontHost* host, 
            izanagi::text::E_FONT_CHAR_ENCODE encode,
            void* text,
            IZ_UINT bytes,
            void* data);

    private:
        virtual IZ_BOOL Init(
            izanagi::text::IFontHost* host,
            izanagi::text::CUString& str, 
            void* userData);
        void InitInternal(text::CUString& text, const FontRuns* fontRuns);
       
    protected:
        UBiDiLevel m_Level;
        izanagi::text::IFontHost* m_Host;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_CACHE_H__)
