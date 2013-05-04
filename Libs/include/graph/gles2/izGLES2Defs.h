#if !defined(__IZANAGI_GLES2_DEFS_H__)
#define __IZANAGI_GLES2_DEFS_H__

#include <EGL/egl.h>
#include <GLES2/gl2.h>

typedef GLuint TEX_HANDLE_GLES2;
typedef GLuint SHADER_PARAM_HANDLE_GLES2;

namespace izanagi
{
namespace graph
{
    /** 初期化パラメータ
     */
    struct SGraphicsDeviceInitParams {
    };
}   // namespace graph
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_GLES2_DEFS_H__)
