#include "CaptureManager.h"

CaptureManager::~CaptureManager()
{
    terminate();
}

void CaptureManager::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width, IZ_UINT height)
{
    m_allocator = allocator;

    m_screenWidth = width;
    m_screenHeight = height;

    m_screenBufferSize = width * height * 4;

    {
        m_depth = device->CreateTexture(
            width, height,
            1,
            izanagi::graph::E_GRAPH_PIXEL_FMT_D24S8,
            izanagi::graph::E_GRAPH_RSC_USAGE_DYNAMIC);

        ::glGenFramebuffers(1, &m_fbo);

        glBindFramebuffer(GL_FRAMEBUFFER, m_fbo);

        auto depth = m_depth->GetTexHandle();

        CALL_GL_API(::glFramebufferTexture2D(
            GL_FRAMEBUFFER,
            GL_DEPTH_ATTACHMENT,
            GL_TEXTURE_2D,
            depth,
            0));

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    for (IZ_UINT i = 0; i < COUNTOF(m_SD); i++) {
        auto& sd = m_SD[i];

        glGenBuffers(1, &sd.buffer);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, sd.buffer);
        glBufferData(GL_PIXEL_PACK_BUFFER, m_screenBufferSize, 0, GL_STREAM_READ);

        glGenBuffers(1, &sd.tmpBuffer);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, sd.tmpBuffer);
        glBufferData(GL_PIXEL_PACK_BUFFER, m_screenBufferSize, 0, GL_STREAM_COPY);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);
    }

    m_pixels = ALLOC(m_allocator, width * height * 4);


    m_tmpTex = device->CreateTexture(
        width, height,
        1,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8,
        izanagi::graph::E_GRAPH_RSC_USAGE_DYNAMIC);
    IZ_ASSERT(m_tmpTex);

}

void CaptureManager::readback(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::sys::CTimer timer;

    {
        timer.Begin();
        readbackColor();
        auto time = timer.End();
        IZ_PRINTF("Color [%f] ", time);
    }

    {
        timer.Begin();
        readbackDepth();
        auto time = timer.End();
        IZ_PRINTF("Depth [%f] ", time);
    }

    IZ_PRINTF("\n");
}

void CaptureManager::readbackDepth()
{
    auto depth = m_depth->GetTexHandle();

    CALL_GL_API(glBindTexture(GL_TEXTURE_2D, depth));

    CALL_GL_API(glGetTexImage(
        GL_TEXTURE_2D,
        0,
        GL_DEPTH_COMPONENT,
        GL_UNSIGNED_INT,
        m_pixels));
}

void CaptureManager::readbackColor()
{
    IZ_ASSERT(m_screenBufferSize > 0);
    IZ_ASSERT(m_screenWidth > 0);
    IZ_ASSERT(m_screenHeight > 0);

    while (m_tail != m_head) {
        auto tmpTail = (m_tail + 1) % COUNTOF(m_SD);

        auto& sd = m_SD[tmpTail];

        GLenum res = glClientWaitSync(sd.fence, 0, 0);
        if (res == GL_ALREADY_SIGNALED || res == GL_CONDITION_SATISFIED) {
            glBindBuffer(GL_COPY_READ_BUFFER, sd.buffer);

            IZ_BYTE* src = (IZ_BYTE*)glMapBufferRange(
                GL_COPY_READ_BUFFER,
                0,
                m_screenBufferSize,
                GL_MAP_READ_BIT);

            // TODO
            if (m_tmpTex) {
#if 0
                m_tmpTex->Write(
                    0,
                    src,
                    0, 0,
                    m_screenWidth, m_screenHeight);
#else
                IZ_UINT8* dst = nullptr;
                auto pitch = m_tmpTex->Lock(0, (void**)&dst, IZ_FALSE, IZ_TRUE);

                // NOTE
                // GLは2Dのy軸が逆転する

                for (IZ_UINT y = 0; y < m_screenHeight; y++) {
                    //auto tmpSrc = src + (m_screenHeight - 1 - y) * pitch;
                    auto tmpSrc = src + y * pitch;
                    auto tmpDst = dst + y * pitch;
                    memcpy(tmpDst, tmpSrc, pitch);
                }

                m_tmpTex->Unlock(0);
#endif

                glUnmapBuffer(GL_COPY_READ_BUFFER);
            }

            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

            m_tail = tmpTail;
        }
        else {
            break;
        }
    }
}

void CaptureManager::begin(
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

void CaptureManager::end(izanagi::graph::CGraphicsDevice* device)
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
    captureScreen();
}

void CaptureManager::blitFrameBuffer()
{
    // NOTE
    // If read frame buffer is 0, we can blit from defaul frame buffer.

    CALL_GL_API(glBlitNamedFramebuffer(
        0, m_fbo,
        0, 0,
        m_screenWidth, m_screenHeight,
        0, 0,
        m_screenWidth, m_screenHeight,
        GL_DEPTH_BUFFER_BIT,
        GL_NEAREST));
}

void CaptureManager::captureScreen()
{
    IZ_ASSERT(m_screenBufferSize > 0);
    IZ_ASSERT(m_screenWidth > 0);
    IZ_ASSERT(m_screenHeight > 0);

    auto tmpHead = (m_head + 1) % COUNTOF(m_SD);

    if (tmpHead == m_tail) {
        IZ_PRINTF("Too fast\n");
    }
    else {
        auto& sd = m_SD[tmpHead];

        izanagi::sys::CTimer timer;
        timer.Begin();

        glReadBuffer(GL_BACK);

        glBindBuffer(
            GL_PIXEL_PACK_BUFFER,
            sd.tmpBuffer);

        glReadPixels(
            0,
            0,
            m_screenWidth,
            m_screenHeight,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);

        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        glBindBuffer(GL_COPY_READ_BUFFER, sd.tmpBuffer);
        glBindBuffer(GL_COPY_WRITE_BUFFER, sd.buffer);
        glCopyBufferSubData(
            GL_COPY_READ_BUFFER,
            GL_COPY_WRITE_BUFFER,
            0,
            0,
            m_screenBufferSize);

        sd.fence = glFenceSync(GL_SYNC_GPU_COMMANDS_COMPLETE, 0);

        auto time = timer.End();
        IZ_PRINTF("Capture %f[ms]\n", time);

        m_head = tmpHead;
    }
}

void CaptureManager::terminate()
{
    SAFE_RELEASE(m_depth);

    FREE(m_allocator, m_pixels);

    glDeleteFramebuffers(1, &m_fbo);

    for (IZ_UINT i = 0; i < COUNTOF(m_SD); i++) {
        auto& sd = m_SD[i];

        glDeleteBuffers(1, &sd.buffer);
        glDeleteBuffers(1, &sd.tmpBuffer);
    }

    SAFE_RELEASE(m_tmpTex);
}

void CaptureManager::drawDebug(izanagi::graph::CGraphicsDevice* device)
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
    if (m_tmpTex) {
        device->SetTexture(0, m_tmpTex);
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
