#if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_OGL_H__)
#define __IZANAGI_GRPAH_GRAPHICS_DEVICE_OGL_H__

#include "graph/gles2/GraphicsDevice_GLES2.h"

namespace izanagi {
namespace graph {
    class CGraphicsDeviceOGL : public CGraphicsDeviceGLES2 {
    public:
        CGraphicsDeviceOGL();
        virtual ~CGraphicsDeviceOGL();

    public:
        virtual IZ_BOOL Reset(const void* initialParam);

        virtual IZ_BOOL CheckRenderTargetCount(IZ_UINT cnt) override
        {
            // MRT‚Í‚È‚µ.
            VRETURN(0 < cnt);
            return IZ_TRUE;
        }
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GRPAH_GRAPHICS_DEVICE_OGL_H__)
