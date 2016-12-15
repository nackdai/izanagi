#if !defined(__IZANAGI_GRAPH_GRAPH_UTIL_H__)
#define __IZANAGI_GRAPH_GRAPH_UTIL_H__

#include "izStd.h"
#include "GraphDefs.h"

namespace izanagi
{
namespace graph
{
    /**
    */
    class CGraphUtil {
    private:
        CGraphUtil();
        ~CGraphUtil();

        NO_COPIABLE(CGraphUtil);

    public:
        /** floatフォーマットかどうか
         */
        static IZ_BOOL IsFloatPixelFormat(E_GRAPH_PIXEL_FMT fmt)
        {
            IZ_BOOL ret = ((fmt == E_GRAPH_PIXEL_FMT_R32F)
                            || (fmt == E_GRAPH_PIXEL_FMT_RGBA32F)
                            || (fmt == E_GRAPH_PIXEL_FMT_RGBA16F));
            return ret;
        }

        /** RGBA8フォーマットかどうか
         */
        static IZ_BOOL IsRGBA8PixelFormat(E_GRAPH_PIXEL_FMT fmt)
        {
            IZ_BOOL ret = ((fmt == E_GRAPH_PIXEL_FMT_RGBA8)
                            || (fmt == E_GRAPH_PIXEL_FMT_BGRA8));
            return ret;
        }

        /** DXTフォーマットかどうか
         */
        static IZ_BOOL IsCompressedPixelFormat(E_GRAPH_PIXEL_FMT fmt)
        {
            IZ_BOOL ret = ((fmt == E_GRAPH_PIXEL_FMT_DXT1)
                            || (fmt == E_GRAPH_PIXEL_FMT_DXT3)
                            || (fmt == E_GRAPH_PIXEL_FMT_DXT5));
            return ret;
        }

        /** Bit Per Pixelを取得.
         */
        static IZ_UINT GetBPP(E_GRAPH_PIXEL_FMT fmt)
        {
            static const IZ_UINT8 bpp[] = {
                4,  // E_GRAPH_PIXEL_FMT_RGBA8
                4,  // E_GRAPH_PIXEL_FMT_BGRA8,

                4,  // E_GRAPH_PIXEL_FMT_RGBA4,
                4,  // E_GRAPH_PIXEL_FMT_RGB10A2,
                1,  // E_GRAPH_PIXEL_FMT_A8,
                
                4,  // E_GRAPH_PIXEL_FMT_R32F,
                8,  // E_GRAPH_PIXEL_FMT_RGBA16F,
                16, // E_GRAPH_PIXEL_FMT_RGBA32F,

                0,  // E_GRAPH_PIXEL_FMT_DXT1,
                0,  // E_GRAPH_PIXEL_FMT_DXT3,
                0,  // E_GRAPH_PIXEL_FMT_DXT5,

                4,  // E_GRAPH_PIXEL_FMT_RGBX8,
                4,  // E_GRAPH_PIXEL_FMT_D24S8,

                // Only for OpenGL...
                16, // E_GRAPH_PIXEL_FMT_RGBA32UI,
                12, // E_GRAPH_PIXEL_FMT_RGB32UI,
                4,  // E_GRAPH_PIXEL_FMT_R32UI
            };
            IZ_C_ASSERT(COUNTOF(bpp) == E_GRAPH_PIXEL_FMT_NUM);

            IZ_UINT ret = bpp[fmt];
            IZ_ASSERT(ret > 0);

            return ret;
        }
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRAPH_GRAPH_UTIL_H__)
