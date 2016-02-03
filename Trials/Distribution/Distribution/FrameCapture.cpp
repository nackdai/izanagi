#include "FrameCapture.h"

// NOTE
// https://www.seas.upenn.edu/~pcozzi/OpenGLInsights/OpenGLInsights-AsynchronousBufferTransfers.pdf
// 28.4 Download
// 28.5 Copy

FrameCapture::~FrameCapture()
{
    terminate();
}

void FrameCapture::initScreenCapture(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width, IZ_UINT height)
{
    m_screenWidth = width;
    m_screenHeight = height;
    m_screenBufferSize = width * height * 4;

#ifdef __IZ_OGL__
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

    m_tmpTex = device->CreateTexture(
        width, height,
        1,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8,
        izanagi::graph::E_GRAPH_RSC_USAGE_DYNAMIC);
    IZ_ASSERT(m_tmpTex);
#endif  // #ifdef __IZ_OGL__
}

void FrameCapture::procScreenCapture()
{
    IZ_ASSERT(m_screenBufferSize > 0);
    IZ_ASSERT(m_screenWidth > 0);
    IZ_ASSERT(m_screenHeight > 0);

#ifdef __IZ_OGL__
    while (m_RBTail != m_RBHead) {
        auto tmpTail = (m_RBTail + 1) % COUNTOF(m_SD);

        auto& sd = m_SD[tmpTail];

        GLenum res = glClientWaitSync(sd.fence, 0, 0);
        if (res == GL_ALREADY_SIGNALED || res == GL_CONDITION_SATISFIED) {
            glBindBuffer(GL_COPY_READ_BUFFER, sd.buffer);

            izanagi::sys::CTimer timer;
            timer.Begin();

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

            auto time = timer.End();
            IZ_PRINTF("Readback %f[ms]\n", time);

            glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0);

            m_RBTail = tmpTail;
        }
        else {
            break;
        }
    }
#endif  // #ifdef __IZ_OGL__
}

void FrameCapture::captureScreen()
{
    IZ_ASSERT(m_screenBufferSize > 0);
    IZ_ASSERT(m_screenWidth > 0);
    IZ_ASSERT(m_screenHeight > 0);

#ifdef __IZ_OGL__
    auto tmpHead = (m_RBHead + 1) % COUNTOF(m_SD);

    if (tmpHead == m_RBTail) {
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

#if 1
        glReadPixels(
            0,
            0,
            m_screenWidth,
            m_screenHeight,
            GL_RGBA,
            GL_UNSIGNED_BYTE,
            0);
#else
        glReadPixels(
            0,
            0,
            m_screenWidth,
            m_screenHeight,
            GL_DEPTH_COMPONENT,
            //GL_DEPTH_STENCIL,
            GL_UNSIGNED_INT,
            //GL_UNSIGNED_INT_24_8,
            0);
#endif

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

        m_RBHead = tmpHead;
    }
#endif  // #ifdef __IZ_OGL__
}

void FrameCapture::terminate()
{
#ifdef __IZ_OGL__
    for (IZ_UINT i = 0; i < COUNTOF(m_SD); i++) {
        auto& sd = m_SD[i];

        glDeleteBuffers(1, &sd.buffer);
        glDeleteBuffers(1, &sd.tmpBuffer);
    }

    SAFE_RELEASE(m_tmpTex);
#endif  // #ifdef __IZ_OGL__
}

void FrameCapture::drawDebug(izanagi::graph::CGraphicsDevice* device)
{
#ifdef __IZ_OGL__
    if (m_tmpTex) {
        device->SetTexture(0, m_tmpTex);
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(300, 100, 256, 128));
    }
#endif  // #ifdef __IZ_OGL__
}