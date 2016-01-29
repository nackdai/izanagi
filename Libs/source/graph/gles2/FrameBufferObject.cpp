#include "graph/gles2/FrameBufferObject.h"
#include "graph/gles2/GraphicsDevice_GLES2.h"
#include "graph/gles2/RenderTarget_GLES2.h"

namespace izanagi
{
namespace graph
{
    // インスタンス作成
    CFrameBufferObject* CFrameBufferObject::CreateFBO(
        CGraphicsDeviceGLES2* device,
        IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(CFrameBufferObject));
        VRETURN_NULL(buf != IZ_NULL);

        CFrameBufferObject* instance = new(buf) CFrameBufferObject;
        {
            instance->AddRef();

            instance->m_Allocator = allocator;
        }

        IZ_BOOL result = instance->CreateInternal();

        if (!result) {
            SAFE_RELEASE(instance);
        }

        return instance;
    }

    CFrameBufferObject::CFrameBufferObject()
    {
        m_Allocator = IZ_NULL;
        m_FBO = 0;

        memset(m_Color, 0, sizeof(m_Color));
        m_Depth = IZ_NULL;

        m_IsOnOffScreen = IZ_FALSE;
    }

    CFrameBufferObject::~CFrameBufferObject()
    {
        if (m_FBO > 0) {
            CALL_GL_API(::glDeleteFramebuffers(1, &m_FBO));
        }

        ClearForcibly();
    }

    IZ_BOOL CFrameBufferObject::CreateInternal()
    {
        CALL_GL_API(::glGenFramebuffers(1, &m_FBO));

        return (m_FBO > 0);
    }

    IZ_BOOL CFrameBufferObject::SetRenderTarget(
        IZ_UINT idx,
        CRenderTarget* rt,
        IZ_BOOL isDepth)
    {
        IZ_ASSERT(!m_IsOnOffScreen);
        IZ_ASSERT(m_FBO > 0);
        IZ_ASSERT(idx < ClrBufNum);

        if (isDepth) {
            // Depth
            SAFE_REPLACE(m_Depth, rt);
        }
        else {
            // Color
            SAFE_REPLACE(m_Color[idx], rt);
        }

        return IZ_TRUE;
    }

    void CFrameBufferObject::ClearForcibly()
    {
        for (IZ_UINT i = 0; i < ClrBufNum; i++) {
            SAFE_RELEASE(m_Color[i]);
        }
        SAFE_RELEASE(m_Depth);
    }

    IZ_BOOL CFrameBufferObject::StartOffScreen()
    {
        IZ_ASSERT(!m_IsOnOffScreen);

        static const GLenum ClrAttachment[] = {
            GL_COLOR_ATTACHMENT0,
            GL_COLOR_ATTACHMENT1,
            GL_COLOR_ATTACHMENT2,
            GL_COLOR_ATTACHMENT3,
            GL_COLOR_ATTACHMENT4,
            GL_COLOR_ATTACHMENT5,
            GL_COLOR_ATTACHMENT6,
            GL_COLOR_ATTACHMENT7,
        };
        IZ_C_ASSERT(ClrBufNum == COUNTOF(ClrAttachment));

        if (m_Color != IZ_NULL
            || m_Depth != IZ_NULL)
        {
            m_IsOnOffScreen = IZ_TRUE;

            CALL_GL_API(
                ::glBindFramebuffer(
                    GL_FRAMEBUFFER,
                    m_FBO));

            for (IZ_UINT i = 0; i < ClrBufNum; i++) {
                GLuint colorTexHandle = (m_Color[i] != IZ_NULL
                    ? m_Color[i]->GetTexHandle()
                    : 0);

                CALL_GL_API(
                    ::glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    ClrAttachment[i],
                    GL_TEXTURE_2D,
                    colorTexHandle,
                    0));
            }

            GLuint depthHandle = (m_Depth != IZ_NULL
                ? m_Depth->GetTexHandle()
                : 0);

            CALL_GL_API(
                ::glFramebufferRenderbuffer(
                    GL_FRAMEBUFFER,
                    GL_DEPTH_ATTACHMENT,
                    GL_RENDERBUFFER,
                    depthHandle));
        }

        return IZ_TRUE;
    }

    IZ_BOOL CFrameBufferObject::EndOffScreen()
    {
        if (m_IsOnOffScreen) {
            CALL_GL_API(::glBindFramebuffer(GL_FRAMEBUFFER, 0));

            m_IsOnOffScreen = IZ_FALSE;
        }

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
