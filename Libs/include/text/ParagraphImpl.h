#if !defined(__IZANAGI_TEXT_PARAGRAPH_IMPL_H__)
#define __IZANAGI_TEXT_PARAGRAPH_IMPL_H__

#include "izDefs.h"
#include "izStd.h"

#include "FontDefs.h"
#include "UString.h"
#include "Paragraph.h"

namespace izanagi
{
namespace text
{
    class IFontHost;
    class CLine;

    /**
     */
    class CParagraphImpl : public CParagraph
    {
        friend class CParagraph;
        friend class CLineImpl;

    public:
        static CParagraph* CreateParagraph(
            IMemoryAllocator* allocator,
            IFontHost* host,
            E_FONT_CHAR_ENCODE encode,
            const void* text,
            IZ_UINT bytes,
            void* userData)
        {
            CParagraph* ret = CParagraph::CreateParagraph<CParagraphImpl>(
                allocator,
                host,
                encode,
                text,
                bytes,
                userData);
            return ret;
        }

    private:
        CParagraphImpl();
        virtual ~CParagraphImpl();

    private:
        virtual IZ_BOOL Init(
            IFontHost* host,
            E_FONT_CHAR_ENCODE encode,
            const void* text,
            IZ_UINT bytes,
            void* userData);

        virtual void BeginCreateLine();
        virtual void EndCreateLine();

        virtual CLine* CreateLine(IZ_UINT width);

    private:
        struct SLineParam
        {
            E_FONT_CHAR_ENCODE encode;
            const void* text;
            IZ_UINT bytes;
        };

    private:
        IFontHost* m_FontHost;
        CUString m_String;
        IZ_UINT m_LastPos;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_IMPL_H__)
