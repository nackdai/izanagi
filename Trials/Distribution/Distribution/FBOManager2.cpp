#include "FBOManager2.h"

// NOTE
// Readback from FBO
// http://stackoverflow.com/questions/765434/glreadpixels-from-fbo-fails-with-multisampling

FBOManager2::~FBOManager2()
{
    terminate();
}

void FBOManager2::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width, IZ_UINT height)
{
    m_allocator = allocator;

    m_screenWidth = width;
    m_screenHeight = height;

    ::glGenFramebuffers(1, &m_fbo);

    {
        m_clr = device->CreateTexture(
            width, height,
            1,
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8,
            izanagi::graph::E_GRAPH_RSC_USAGE_DYNAMIC);

        m_depth = device->CreateTexture(
            width, height,
            1,
            izanagi::graph::E_GRAPH_PIXEL_FMT_D24S8,
            izanagi::graph::E_GRAPH_RSC_USAGE_DYNAMIC);
    }

    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        auto clr = m_clr->GetTexHandle();
        auto depth = m_depth->GetTexHandle();

        CALL_GL_API(::glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_COLOR_ATTACHMENT0,
            GL_TEXTURE_2D,
            clr,
            0));

        GLenum attachedColor[] = {
            GL_COLOR_ATTACHMENT0,
        };
        CALL_GL_API(::glDrawBuffers(1, attachedColor));

        CALL_GL_API(::glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            depth,
            0));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    m_pixels = ALLOC(m_allocator, width * height * 4);
}

void FBOManager2::readback(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sys::CTimer timer;
    
    {
        timer.Begin();

        auto clr = m_clr->GetTexHandle();

        CALL_GL_API(glBindTexture(GL_TEXTURE_2D, clr));

        CALL_GL_API(glGetTexImage(
            GL_TEXTURE_2D,
            0,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            m_pixels));

        auto time = timer.End();
        IZ_PRINTF("Color [%f] ", time);
    }

    {
        timer.Begin();

        auto depth = m_depth->GetTexHandle();

        CALL_GL_API(glBindTexture(GL_TEXTURE_2D, depth));

        CALL_GL_API(glGetTexImage(
            GL_TEXTURE_2D,
            0,
            GL_DEPTH_COMPONENT,
            GL_UNSIGNED_INT,
            m_pixels));

        auto time = timer.End();
        IZ_PRINTF("Depth [%f] ", time);
    }

    IZ_PRINTF("\n");
}

void FBOManager2::begin(
    izanagi::graph::CGraphicsDevice* device,
    IZ_COLOR clrClear)
{
#ifdef __IZ_OGL__
    // NOTE
    // http://www.slideshare.net/Mark_Kilgard/opengl-45-update-for-nvidia-gpus
    // 38ページ
    ::glClipControl(
        GL_UPPER_LEFT,
        GL_ZERO_TO_ONE);
#endif
}

void FBOManager2::end(izanagi::graph::CGraphicsDevice* device)
{
#ifdef __IZ_OGL__
    // NOTE
    // http://www.slideshare.net/Mark_Kilgard/opengl-45-update-for-nvidia-gpus
    // 38ページ
    ::glClipControl(
        GL_LOWER_LEFT,
        GL_ZERO_TO_ONE);
#endif

    // NOTE
    // If read frame buffer is 0, we can blit from defaul frame buffer.

    CALL_GL_API(glBlitNamedFramebuffer(
        0, m_fbo,
        0, 0,
        m_screenWidth, m_screenHeight,
        0, 0,
        m_screenWidth, m_screenHeight,
        GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT,
        GL_NEAREST));
}

void FBOManager2::terminate()
{
    SAFE_RELEASE(m_clr);
    SAFE_RELEASE(m_depth);

    FREE(m_allocator, m_pixels);

    glDeleteFramebuffers(1, &m_fbo);
}

void FBOManager2::drawDebug(izanagi::graph::CGraphicsDevice* device)
{
#if 0
    auto rt0 = m_frames[m_curFrame].rt[0];
    auto rt1 = m_frames[m_curFrame].rt[1];

    if (rt0) {
        device->SetTexture(0, rt0);
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

        device->Draw2DSprite(
            //izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CFloatRect(0.0f, 1.0f, 1.0f, 0.0f),
            izanagi::CIntRect(300, 100, 256, 128));
    }
    if (rt1) {
        device->SetTexture(0, rt1);
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

        device->Draw2DSprite(
            //izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CFloatRect(0.0f, 1.0f, 1.0f, 0.0f),
            izanagi::CIntRect(300, 300, 256, 128));
    }
#else
    if (m_clr) {
        device->SetTexture(0, m_clr);
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(300, 100, 256, 128));
    }
    if (m_depth) {
        device->SetTexture(0, m_depth);
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_TEX_AS_DETPH);

        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(300, 300, 256, 128));
    }
#endif
}