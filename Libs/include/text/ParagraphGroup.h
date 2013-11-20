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
    class CUString;

    /**
     */
    class CParagraphGroup : public CObject
    {
    public:
        template <typename _T>
        static CParagraphGroup* CreateParagraphGroup(
            IMemoryAllocator* allocator,
            IFontHost* host,
            CUString& text,
            void* userData)
        {
            IZ_BOOL result = IZ_TRUE;
            void* buf = ALLOC(allocator, sizeof(_T));

            CParagraphGroup* instance = new(buf) _T;
            {
                instance->AddRef();

                instance->m_Allocator = allocator;

                result = instance->Init(host, text, userData);
            }

            if (!result)
            {
                SAFE_RELEASE(instance);
            }

            return instance;
        }

    protected:
        CParagraphGroup();
        virtual ~CParagraphGroup();

        NO_COPIABLE(CParagraphGroup);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void Layout(
            IZ_UINT width,
            IZ_UINT height);

        void Prepare(graph::CGraphicsDevice* device);

        void Render(
            IZ_INT x,
            IZ_INT y,
            graph::CGraphicsDevice* device);

    protected:
        PURE_VIRTUAL(
            IZ_BOOL Init(
                IFontHost* host,
                CUString& str, 
                void* userData));

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
