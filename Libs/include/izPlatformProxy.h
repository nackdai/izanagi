#if !defined(__IZANAGI_PLATFORM_PROXY_H__)
#define __IZANAGI_PLATFORM_PROXY_H__

#ifdef __IZ_DX9__
    #include "graph/dx9/izD3DDefs.h"
#elif __IZ_GLES2__
    #include "graph/gles2/izGLES2Defs.h"
#endif

#endif  // #if !defined(__IZANAGI_PLATFORM_PROXY_H__)
