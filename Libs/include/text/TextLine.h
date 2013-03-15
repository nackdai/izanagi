#if !defined(__IZANAGI_TEXT_TEXT_LINE_H__)
#define __IZANAGI_TEXT_TEXT_LINE_H__

#include "izDefs.h"
#include "izStd.h"
#include "FontHost.h"

namespace izanagi
{
    namespace graph
    {
        class CGraphicsDevice;
        class CTexture;
    }

namespace text
{
    class CUString;

    /**
     */
    class CLine : public CObject
    {
        friend class CParagraph;

    public:
        template <typename _T>
        static CLine* CreateLine(
            IMemoryAllocator* allocator,
            IFontHost* host,
            void* userData)
        {
            IZ_BOOL result = IZ_FALSE;
            void* buf = ALLOC(allocator, sizeof(_T));

            _T* instance = new(buf) _T;
            {
                instance->AddRef();

                instance->m_Allocator = allocator;
                SAFE_REPLACE(instance->m_FontHost, host);
                result = instance->Init(userData);
            }

            if (!result)
            {
                SAFE_RELEASE(instance);
            }

            return instance;
        }

    protected:
        CLine();
        virtual ~CLine();

        NO_COPIABLE(CLine);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        PURE_VIRTUAL(IZ_UINT GetLineWidth());

    protected:
        PURE_VIRTUAL(IZ_BOOL Init(void* userData));

        void Prepare(
            IZ_UINT height,
            IZ_UINT ascent,
            graph::CGraphicsDevice* device);

        PURE_VIRTUAL(
            void Prepare(
                IZ_UINT8* dst,
                IZ_UINT pitch,
                IZ_UINT ascent,
                IFontHost* host));

        IZ_UINT CopyImage(
            IZ_UINT glyphID,
            IZ_UINT8* dst,
            IZ_UINT x,
            IZ_UINT pitch,
            IZ_UINT ascent);

        void Clear();

        void Render(
            IZ_UINT x,
            IZ_UINT y,
            graph::CGraphicsDevice* device);

    private:
        CStdList<CLine>::Item* GetListItem() { return &m_ListItem; }

    protected:
        IMemoryAllocator* m_Allocator;

        IFontHost* m_FontHost;
        graph::CTexture* m_Texture;

        CStdList<CLine>::Item m_ListItem;
    };

    /**
     */
    class CDefaultLine : public CLine
    {
        friend class CLine;

    public:
        static CLine* CreateLine(
            IMemoryAllocator* allocator,
            IFontHost* host,
            void* userData)
        {
            CLine* ret = CLine::CreateLine<CDefaultLine>(allocator, host, userData);
            return ret;
        }

    private:
        CDefaultLine();
        virtual ~CDefaultLine() {}

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
        const void* m_Text;
        IZ_UINT m_Bytes;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_TEXT_LINE_H__)
