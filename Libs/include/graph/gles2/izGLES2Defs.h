#if !defined(__IZANAGI_GLES2_DEFS_H__)
#define __IZANAGI_GLES2_DEFS_H__

#include <EGL/egl.h>
#include <GLES2/gl2.h>

typedef GLuint TEX_HANDLE;
typedef GLuint SHADER_PARAM_HANDLE;

namespace izanagi
{
namespace graph
{
    /** 初期化パラメータ
     */
    struct SGraphicsDeviceInitParams {
        EGLNativeWindowType window;
        EGLNativeDisplayType display;

        IZ_UINT screenWidth;
        IZ_UINT screenHeight;

        IZ_UINT8 rgba[4];

        IZ_UINT8 depth;
        IZ_UINT8 stencil;

        IZ_BOOL enableMultiSample;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GLES2_DEFS_H__)
