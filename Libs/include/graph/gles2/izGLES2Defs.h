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

        GLuint screenWidth;
        GLuint screenHeight;

        GLubyte rgba[4];

        GLubyte depth;
        GLubyte stencil;

        GLboolean enableMultiSample;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GLES2_DEFS_H__)
