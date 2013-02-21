#if !defined(__IZANAGI_TEXT_FONT_RENDERER_FT_H__)
#define __IZANAGI_TEXT_FONT_RENDERER_FT_H__

#include "izDefs.h"
#include "izStd.h"
#include "text/FontHostFT.h"

namespace izanagi
{
    class IMemoryAllocator;

    namespace graph
    {
        class CTexture;
        class CGraphicsDevice;
    }

namespace text
{
    /**
     */
    class CFontRendererFT
    {
    public:
        CFontRendererFT(){}
        ~CFontRendererFT(){}

    public:
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_RENDERER_FT_H__)
