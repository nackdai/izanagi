#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/gles2/Texture_GLES2.h"
#include "graph/gles2/CubeTexture_GLES2.h"
#include "graph/gles2/VertexBuffer_GLES2.h"
#include "graph/gles2/IndexBuffer_GLES2.h"
#include "graph/gles2/ShaderProgram_GLES2.h"
#include "graph/gles2/VertexDeclaration_GLES2.h"
#include "graph/gles2/FrameBufferObject.h"
#include "graph/gles2/RenderTarget_GLES2.h"
#include "graph/2d/2DRenderer.h"

namespace izanagi
{
namespace graph
{
    CGraphicsDevice* CGraphicsDevice::s_Instance = IZ_NULL;

    /**
    * インスタンス作成
    */
    CGraphicsDevice* CGraphicsDevice::CreateGraphicsDevice(IMemoryAllocator* allocator)
    {
        if (s_Instance != IZ_NULL) {
            // 作成済みなので何もしない
            return s_Instance;
        }

        IZ_ASSERT(allocator != IZ_NULL);

        IZ_BOOL result = IZ_TRUE;
        IZ_UINT8* buf = IZ_NULL;
        CGraphicsDeviceGLES2* instance = IZ_NULL;

        // メモリ確保
        buf = (IZ_UINT8*)ALLOC_ZERO(allocator, sizeof(CGraphicsDeviceGLES2));
        VGOTO(buf != IZ_NULL, __EXIT__);

        // インスタンス作成
        instance = new(buf) CGraphicsDeviceGLES2;
        {
            instance->m_Allocator = allocator;
            instance->AddRef();
        }

        if (!result) {
            goto __EXIT__;
        }

        s_Instance = instance;
    
    __EXIT__:
        if (!result) {
            if (instance != IZ_NULL) {
                SAFE_RELEASE(instance);
            }
            else if (buf != IZ_NULL) {
                allocator->Free(buf);
            }
        }

        return instance;
    }

    // 本体作成
    IZ_BOOL CGraphicsDeviceGLES2::CreateBody(const SGraphicsDeviceInitParams& sParams)
    {
        // Get Display
        m_Display = ::eglGetDisplay(sParams.display);
        VRETURN(m_Display != EGL_NO_DISPLAY);

        // Initialize EGL
        EGLint major, minor;
        EGLBoolean result = ::eglInitialize(m_Display, &major, &minor);
        VRETURN(result);

        // Get configs
        EGLint numConfigs;
        result = ::eglGetConfigs(m_Display, IZ_NULL, 0, &numConfigs);
        VRETURN(result);

        EGLint configAttribList[] =
        {
            EGL_RED_SIZE,       sParams.rgba[0],
            EGL_GREEN_SIZE,     sParams.rgba[1],
            EGL_BLUE_SIZE,      sParams.rgba[2],
            EGL_ALPHA_SIZE,     (sParams.rgba[3] > 0 ? sParams.rgba[3] : EGL_DONT_CARE),
            EGL_DEPTH_SIZE,     (sParams.depth > 0 ? sParams.depth : EGL_DONT_CARE),
            EGL_STENCIL_SIZE,   (sParams.stencil > 0 ? sParams.stencil : EGL_DONT_CARE),
            EGL_SAMPLE_BUFFERS, (sParams.enableMultiSample ? 1 : 0),
            EGL_NONE,
        };

        // Choose config
        EGLConfig config;
        result = ::eglChooseConfig(
                m_Display,
                configAttribList,
                &config,
                1,
                &numConfigs);
        VRETURN(result);

        static const EGLint surfaceAttribList[] =
        {
            EGL_RENDER_BUFFER, EGL_BACK_BUFFER,
            EGL_NONE,
        };

        // Create a surface
        m_Surface = ::eglCreateWindowSurface(
            m_Display,
            config,
            sParams.window,
            surfaceAttribList);
        VRETURN(m_Surface != EGL_NO_SURFACE);

        static const EGLint contextAtrribs[] =
        {
            EGL_CONTEXT_CLIENT_VERSION, 2,
            EGL_NONE, EGL_NONE,
        };

        // Create a GL context
        m_Context = ::eglCreateContext(
            m_Display,
            config,
            EGL_NO_CONTEXT,
            contextAtrribs);
        VRETURN(m_Context != EGL_NO_CONTEXT);

        // Make the context current
        result = ::eglMakeCurrent(m_Display, m_Surface, m_Surface, m_Context);
        VRETURN(result);

        m_ScreenWidth = sParams.screenWidth;
        m_ScreenHeight = sParams.screenHeight;

        return IZ_TRUE;
    }

    void CGraphicsDeviceGLES2::OnTerminate()
    {
        if (m_Display != EGL_NO_DISPLAY) {
            ::eglMakeCurrent(m_Display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);

            if (m_Surface != EGL_NO_SURFACE) {
                ::eglDestroySurface(m_Display, m_Surface);
                m_Surface = EGL_NO_SURFACE;
            }

            if (m_Context != EGL_NO_CONTEXT) {
                ::eglDestroyContext(m_Display, m_Context);
                m_Context = EGL_NO_CONTEXT;
            }

            ::eglTerminate(m_Display);
            m_Display = EGL_NO_DISPLAY;
        }
    }

    /**
    * 同期
    */
    IZ_BOOL CGraphicsDeviceGLES2::Present()
    {
        ::eglSwapBuffers(m_Display, m_Surface);
        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
