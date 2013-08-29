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

        m_Color = IZ_NULL;
        m_Depth = IZ_NULL;

        m_IsOnOffScreen = IZ_FALSE;
    }

    CFrameBufferObject::~CFrameBufferObject()
    {
        if (m_FBO > 0) {
            CALL_GLES2_API(::glDeleteFramebuffers(1, &m_FBO));
        }

        SAFE_RELEASE(m_Color);
        SAFE_RELEASE(m_Depth);
    }

    IZ_BOOL CFrameBufferObject::CreateInternal()
    {
        CALL_GLES2_API(::glGenFramebuffers(1, &m_FBO));

        return (m_FBO > 0);
    }

    IZ_BOOL CFrameBufferObject::SetRenderTarget(CRenderTarget* rt, IZ_BOOL isDepth)
    {
        IZ_ASSERT(!m_IsOnOffScreen);
        IZ_ASSERT(m_FBO > 0);

        if (isDepth) {
            // Depth
            IZ_ASSERT(m_Depth == IZ_NULL);
            SAFE_REPLACE(m_Depth, rt);
        }
        else {
            // Color
            IZ_ASSERT(m_Color == IZ_NULL);
            SAFE_REPLACE(m_Color, rt);
        }

        return IZ_TRUE;
    }

    IZ_BOOL CFrameBufferObject::StartOffScreen()
    {
        IZ_ASSERT(!m_IsOnOffScreen);

        if (m_Color != IZ_NULL
            || m_Depth != IZ_NULL)
        {
            m_IsOnOffScreen = IZ_TRUE;

            ::glBindFramebuffer(
                GL_FRAMEBUFFER,
                m_FBO);

            if (m_Color != IZ_NULL)
            {
                ::glFramebufferTexture2D(
                    GL_FRAMEBUFFER,
                    GL_COLOR_ATTACHMENT0,
                    GL_TEXTURE_2D,
                    m_Color->GetTexHandle(),
                    0);
            }

            if (m_Depth != IZ_NULL)
            {
                ::glFramebufferRenderbuffer(
                    GL_FRAMEBUFFER,
                    GL_DEPTH_ATTACHMENT,
                    GL_RENDERBUFFER,
                    m_Depth->GetTexHandle());
            }
        }

        return IZ_TRUE;
    }

    IZ_BOOL CFrameBufferObject::EndOffScreen()
    {
        if (m_IsOnOffScreen) {
            ::glBindFramebuffer(GL_FRAMEBUFFER, 0);

            m_IsOnOffScreen = IZ_FALSE;

            if (m_Color != IZ_NULL)
            {
                SAFE_RELEASE(m_Color);
            }

            if (m_Depth != IZ_NULL)
            {
                SAFE_RELEASE(m_Depth);
            }
        }

        return IZ_TRUE;
    }
}   // namespace graph
}   // namespace izanagi
