#if !defined(__IZANAGI_TEXT_FONT_RENDERER_FT_H__)
#define __IZANAGI_TEXT_FONT_RENDERER_FT_H__

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
    class CParagraphGroup;

    /**
     */
    class CFontRendererFT : public CObject
    {
    public:
        static CFontRendererFT* CreateFontRenderer(
            IMemoryAllocator* allocator,
            IInputStream* in,
            IZ_UINT pixelSize);

    private:
        CFontRendererFT();
        ~CFontRendererFT();

        NO_COPIABLE(CFontRendererFT);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void Render(
            IZ_UINT x, IZ_UINT y,
            IZ_UINT width, IZ_UINT height,
            CParagraphGroup& paragraphGroup,
            graph::CGraphicsDevice* device);

    private:
        IMemoryAllocator* m_Allocator;
        IFontHost* m_FontHost;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_RENDERER_FT_H__)
