#include "CaptureManager2.h"

CaptureManager2::~CaptureManager2()
{
    terminate();
}

void CaptureManager2::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width, IZ_UINT height)
{
    m_allocator = allocator;

    m_screenWidth = width;
    m_screenHeight = height;

    m_screenBufferSize = width * height * 4;

    m_color = device->CreateTexture(
        width, height,
        1,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8,
        izanagi::graph::E_GRAPH_RSC_USAGE_DYNAMIC);

    m_depth = device->CreateTexture(
        width, height,
        1,
        izanagi::graph::E_GRAPH_PIXEL_FMT_D24S8,
        izanagi::graph::E_GRAPH_RSC_USAGE_DYNAMIC);

    ::glGenFramebuffers(1, &m_fbo);

    glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

    auto color = m_color->GetTexHandle();
    auto depth = m_depth->GetTexHandle();

    CALL_GL_API(::glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        color,
        0));

    CALL_GL_API(::glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_DEPTH_ATTACHMENT,
        GL_TEXTURE_2D,
        depth,
        0));

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    m_pixels = ALLOC(m_allocator, width * height * 4);
}

void CaptureManager2::readback(izanagi::graph::CGraphicsDevice* device)
{
#if 0
    izanagi::sys::CTimer timer;

    {
        timer.Begin();
        
        auto color = m_color->GetTexHandle();

        CALL_GL_API(glBindTexture(GL_TEXTURE_2D, color));

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
#endif
}

void CaptureManager2::begin(
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

void CaptureManager2::end(izanagi::graph::CGraphicsDevice* device)
{
#ifdef __IZ_OGL__
    // NOTE
    // http://www.slideshare.net/Mark_Kilgard/opengl-45-update-for-nvidia-gpus
    // 38ページ
    ::glClipControl(
        GL_LOWER_LEFT,
        GL_ZERO_TO_ONE);
#endif

    blitFrameBuffer();
}

void CaptureManager2::blitFrameBuffer()
{
    izanagi::sys::CTimer timer;
    timer.Begin();

    // NOTE
    // If read frame buffer is 0, we can blit from defaul frame buffer.

    CALL_GL_API(glBlitNamedFramebuffer(
        0, m_fbo,
        0, 0,
        m_screenWidth, m_screenHeight,
        0, 0,
        m_screenWidth, m_screenHeight,
        GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT,
        GL_NEAREST));

    auto time = timer.End();
    IZ_PRINTF("Blit [%f]\n", time);
}

void CaptureManager2::terminate()
{
    SAFE_RELEASE(m_color);
    SAFE_RELEASE(m_depth);

    FREE(m_allocator, m_pixels);

    glDeleteFramebuffers(1, &m_fbo);
}

void CaptureManager2::drawDebug(izanagi::graph::CGraphicsDevice* device)
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
#elif 1
    if (m_color) {
        device->SetTexture(0, m_color);
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
