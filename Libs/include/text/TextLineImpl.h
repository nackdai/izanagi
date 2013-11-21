#if !defined(__IZANAGI_TEXT_TEXT_LINE_IMPL_H__)
#define __IZANAGI_TEXT_TEXT_LINE_IMPL_H__

#include "izDefs.h"
#include "izStd.h"

#include "FontHost.h"
#include "TextLine.h"

namespace izanagi
{
namespace text
{
    class CUString;

    /**
     */
    class CLineImpl : public CLine
    {
        friend class CLine;

    public:
        static CLine* CreateLine(
            IMemoryAllocator* allocator,
            IFontHost* host,
            void* userData)
        {
            CLine* ret = CLine::CreateLine<CLineImpl>(allocator, host, userData);
            return ret;
        }

    private:
        CLineImpl();
        virtual ~CLineImpl() {}

    public:
        virtual IZ_UINT GetLineWidth();

    private:
        virtual IZ_BOOL Init(void* userData);

        virtual void Prepare(
            IZ_UINT8* dst,
            IZ_UINT pitch,
            IZ_UINT ascent,
            IFontHost* host);

    private:
        E_FONT_CHAR_ENCODE m_Encode;
        const void* m_Text;
        IZ_UINT m_Bytes;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_TEXT_LINE_IMPL_H__)
