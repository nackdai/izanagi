#if !defined(__IZANAGI_OGL_DEFS_H__)
#define __IZANAGI_OGL_DEFS_H__

#include <GL/glew.h>

namespace izanagi {
    typedef GLuint TEX_HANDLE;
    typedef GLint SHADER_PARAM_HANDLE;

    typedef void* Display;
    typedef void* Surface;
    typedef void* Context;
}

#ifndef EGL_NO_DISPLAY
    #define EGL_NO_DISPLAY NULL
#endif

#ifndef EGL_NO_SURFACE
    #define EGL_NO_SURFACE NULL
#endif

#ifndef EGL_NO_CONTEXT
    #define EGL_NO_CONTEXT NULL
#endif

#define IS_VALID_SHADER_PARAM_HANDLE(handle)    ((handle) != 0)

#ifdef __IZ_DEBUG__
    #define CALL_GLES2_API(func)\
        func;\
        {\
            GLenum __gl_err__ = ::glGetError();\
            if (__gl_err__ != GL_NO_ERROR) { IZ_PRINTF("GLES2 ERROR [0x%x]\n", __gl_err__); IZ_ASSERT(IZ_FALSE); }\
        }
#else
    #define CALL_GLES2_API(func) func
#endif

namespace izanagi
{
namespace graph
{
    /** 初期化パラメータ
     */
    struct SGraphicsDeviceInitParams {
        GLuint screenWidth;
        GLuint screenHeight;

        GLubyte rgba[4];

        GLubyte depth;
        GLubyte stencil;

        GLboolean enableMultiSample;
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_OGL_DEFS_H__)
