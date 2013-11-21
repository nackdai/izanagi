#if !defined(__IZANAGI_TEXT_PARAGRAPH_GROUP_IMPL_H__)
#define __IZANAGI_TEXT_PARAGRAPH_GROUP_IMPL_H__

#include "izDefs.h"
#include "izStd.h"

#include "ParagraphGroup.h"

namespace izanagi
{
namespace text
{
    class IFontHost;
    class CParagraph;
    class CUString;

    /**
     */
    class CParagraphGroupImpl : public CParagraphGroup
    {
        friend class CParagraphGroup;

    public:
        static CParagraphGroup* CreateParagraphGroup(
            IMemoryAllocator* allocator,
            IFontHost* host,
            CUString& text,
            void* userData)
        {
            return CParagraphGroup::CreateParagraphGroup<CParagraphGroupImpl>(
                allocator,
                host,
                text,
                userData);
        }

    private:
        CParagraphGroupImpl();
        virtual ~CParagraphGroupImpl();

    private:
        virtual IZ_BOOL Init(
            IFontHost* host,
            CUString& str,
            void* userData);

    private:
        IFontHost* m_FontHost;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_PARAGRAPH_GROUP_IMPL_H__)
