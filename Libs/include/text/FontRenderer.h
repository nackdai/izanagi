#if !defined(__IZANAGI_TEXT_FONT_RENDERER_H__)
#define __IZANAGI_TEXT_FONT_RENDERER_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
    class IInputStream;

    namespace graph
    {
        class CGraphicsDevice;
    }

namespace text
{
    class IFontHost;
    class CUString;
    class CParagraphGroup;
    class CGlyphCache;

    /**
     */
    class CFontRenderer : public CObject
    {
    public:
        template <typename _T>
        static CFontRenderer* CreateFontRenderer(
            IMemoryAllocator* allocator,
            IInputStream* in,
            IZ_UINT pixelSize)
        {
            IZ_BOOL result = IZ_TRUE;

            void* buf = ALLOC(allocator, sizeof(_T));
            _T* instance = new(buf) _T;
            {
                instance->AddRef();
                instance->m_Allocator = allocator;
                result = instance->Init(in, pixelSize);
            }

            if (!result)
            {
                SAFE_RELEASE(instance);
            }
            return instance;
        }

    protected:
        CFontRenderer();
        virtual ~CFontRenderer();

        NO_COPIABLE(CFontRenderer);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void Render(
            graph::CGraphicsDevice* device,
            IZ_UINT x, IZ_UINT y,
            IZ_UINT width, IZ_UINT height,
            CParagraphGroup& paragraphGroup);

        void Render(
            graph::CGraphicsDevice* device,
            CUString& str,
            CIntPoint& pos,
            IZ_UINT baseline,
            CGlyphCache& glyphCache);

        IFontHost* GetFontHost()
        {
            return m_FontHost;
        }

    protected:
        PURE_VIRTUAL(
            IZ_BOOL Init(
                IInputStream* in,
                IZ_UINT pixelSize));

        PURE_VIRTUAL(IZ_UINT GetNextChar(CUString& string));

    protected:
        IMemoryAllocator* m_Allocator;
        IFontHost* m_FontHost;
    };

    /**
     */
    class CFontRendererFT : public CFontRenderer
    {
        friend class CFontRenderer;

    private:
        CFontRendererFT() {}
        virtual ~CFontRendererFT() {}

    private:
        IZ_BOOL Init(
            IInputStream* in,
            IZ_UINT pixelSize);

        virtual IZ_UINT GetNextChar(CUString& string);
    };

    /**
     */
    class CFontRendererFNT : public CFontRenderer
    {
        friend class CFontRenderer;

    private:
        CFontRendererFNT() {}
        virtual ~CFontRendererFNT() {}

    private:
        IZ_BOOL Init(
            IInputStream* in,
            IZ_UINT pixelSize);

        virtual IZ_UINT GetNextChar(CUString& string);
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_RENDERER_H__)
