#if !defined(__IZANAGI_TEXT_PARAGRAPH_H__)
#define __IZANAGI_TEXT_PARAGRAPH_H__

#include "izDefs.h"
#include "izStd.h"
#include "FontDefs.h"
#include "UString.h"

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
            IFontHost* host,
            E_FONT_CHAR_ENCODE encode,
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
                result = paragraph->Init(host, encode, text, bytes, userData);
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
        PURE_VIRTUAL(
            IZ_BOOL Init(
                IFontHost* host,
                E_FONT_CHAR_ENCODE encode,
                void* text,
                IZ_UINT bytes,
                void* userData));

        void Layout(IZ_UINT width);

        virtual void BeginCreateLine() {}
        virtual void EndCreateLine() {}

        PURE_VIRTUAL(CLine* CreateLine(IZ_UINT width));

        void ReleaseLines();
        void AddLine(CLine* line);

        void Prepare(
            IZ_UINT height,
            IZ_UINT ascent,
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

    /**
     */
    class CDefaultParagraph : public CParagraph
    {
        friend class CParagraph;
        friend class CDefaultLine;

    public:
        static CParagraph* CreateParagraph(
            IMemoryAllocator* allocator,
            IFontHost* host,
            E_FONT_CHAR_ENCODE encode,
            void* text,
            IZ_UINT bytes,
            void* userData)
        {
            CParagraph* ret = CParagraph::CreateParagraph<CDefaultParagraph>(
                allocator,
                host,
                encode,
                text,
                bytes,
                userData);
            return ret;
        }

    private:
        CDefaultParagraph();
        virtual ~CDefaultParagraph();

    private:
        virtual IZ_BOOL Init(
            IFontHost* host,
            E_FONT_CHAR_ENCODE encode,
            void* text,
            IZ_UINT bytes,
            void* userData);

        virtual void BeginCreateLine();
        virtual void EndCreateLine();

        virtual CLine* CreateLine(IZ_UINT width);

    private:
        struct SLineParam
        {
            const void* text;
            IZ_UINT bytes;
        };

    private:
        IFontHost* m_FontHost;
        CUString m_String;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_H__)
