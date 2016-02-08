#include "DxtEncoder.h"

DxtEncoder::~DxtEncoder()
{
    terminate();
}

IZ_BOOL DxtEncoder::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width, IZ_UINT height,
    const char* vtxShader,
    const char* dxtShader,
    const char* pixelShader)
{
    m_width = width;
    m_height = height;

    char* buf = nullptr;
    IZ_UINT allocatedSize = 0;

    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(dxtShader));

        allocatedSize = in.GetSize();
        buf = (char*)ALLOC(allocator, allocatedSize);

        in.Read(buf, 0, allocatedSize);
        buf[allocatedSize] = 0;

        m_dxt = device->CreatePixelShader(buf);
        VRETURN(m_dxt);
    }

    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open(vtxShader));

        auto size = in.GetSize();
        IZ_ASSERT(allocatedSize >= size);

        in.Read(buf, 0, size);
        buf[size] = 0;

        m_vs = device->CreateVertexShader(buf);
        VRETURN(m_vs);
    }

    FREE(allocator, buf);

    {
        m_shd = device->CreateShaderProgram();
        VRETURN(m_shd);

        VRETURN(m_shd->AttachVertexShader(m_vs));
        VRETURN(m_shd->AttachPixelShader(m_dxt));

        m_hImage = m_shd->GetHandleByName("image");
        m_hMode = m_shd->GetHandleByName("mode");
    }

    {
        // NOTE
        // DXTは 4x4 ブロックで、128bit/block.
        // GL_RGBA32UI は 128bit/texel.
        // すると、1texel が DXTのブロックのサイズと同じになるので、fragment shaderの1pixel出力がそのままDXTの1ブロックになる.
        m_tex = device->CreateTexture(
            width / 4, height / 4,
            1,
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA32UI,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        VRETURN(m_tex);

        CALL_GL_API(glGenFramebuffers(1, &m_fbo));

        glGenBuffers(1, &m_pbo);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo);
        glBufferData(GL_PIXEL_PACK_BUFFER, width * height, 0, GL_STREAM_COPY);
        glBindBuffer(GL_PIXEL_PACK_BUFFER, 0);

        m_texDxt = device->CreateTexture(
            width, height,
            1,
            izanagi::graph::E_GRAPH_PIXEL_FMT_DXT5,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        VRETURN(m_tex);
    }

    return IZ_TRUE;
}

void DxtEncoder::encode(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::graph::CTexture* texture)
{
    izanagi::sys::CTimer timer;

    device->SaveRenderState();

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZENABLE,
        IZ_FALSE);
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_CULLMODE,
        izanagi::graph::E_GRAPH_CULL_NONE);

    auto texHandle = m_tex->GetTexHandle();

    // Set FBO.
    CALL_GL_API(glBindFramebuffer(GL_FRAMEBUFFER, m_fbo));
    CALL_GL_API(glFramebufferTexture2D(
        GL_FRAMEBUFFER,
        GL_COLOR_ATTACHMENT0,
        GL_TEXTURE_2D,
        texHandle,
        0));

    GLenum attachedColor[] = {
        GL_COLOR_ATTACHMENT0,
    };

    CALL_GL_API(::glDrawBuffers(1, attachedColor));

    CALL_GL_API(glDisable(GL_FRAMEBUFFER_SRGB));

    device->SetShaderProgram(m_shd);

    device->SetTexture(0, texture);

    CALL_GL_API(glUniform1i(m_hImage, 0));
    CALL_GL_API(glUniform1i(m_hMode, 3));

    device->SetViewport(
        izanagi::graph::SViewport(0, 0, m_width/4, m_height/4));

    // NOTE
    // 頂点バッファを使わず全画面に描画する頂点シェーダ.
    CALL_GL_API(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

#if 1
    // Readback.
    {
        timer.Begin();

        CALL_GL_API(glReadBuffer(GL_COLOR_ATTACHMENT0));
        CALL_GL_API(glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo));
        CALL_GL_API(glReadPixels(
            0, 0,
            m_width / 4, m_height / 4,
            GL_RGBA_INTEGER,
            GL_UNSIGNED_INT,
            0));
        CALL_GL_API(glBindBuffer(GL_PIXEL_PACK_BUFFER, 0));

        CALL_GL_API(glBindFramebuffer(GL_FRAMEBUFFER, 0));

        auto time = timer.End();
        IZ_PRINTF("ReadToPBO [%f]\n", time);
    }

    // Copy to dxt texture.
    {
        timer.Begin();

        auto texdxt = m_texDxt->GetTexHandle();

        CALL_GL_API(glBindTexture(GL_TEXTURE_2D, texdxt));
        CALL_GL_API(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, m_pbo));

        CALL_GL_API(glCompressedTexSubImage2D(
            GL_TEXTURE_2D,
            0,
            0, 0,
            m_width, m_height,
            GL_COMPRESSED_RGBA_S3TC_DXT5_EXT,
            m_width * m_height,
            0));

        CALL_GL_API(glBindTexture(GL_TEXTURE_2D, 0));
        CALL_GL_API(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));

        auto time = timer.End();
        IZ_PRINTF("CopyToDXT [%f]\n", time);
    }
#endif

    // 元に戻す.
    device->SetViewport(
        izanagi::graph::SViewport(0, 0, m_width, m_height));

    device->LoadRenderState();
}

void DxtEncoder::terminate()
{
    SAFE_RELEASE(m_vs);
    SAFE_RELEASE(m_dxt);
    SAFE_RELEASE(m_shd);

    SAFE_RELEASE(m_tex);
}

void DxtEncoder::drawDebug(izanagi::graph::CGraphicsDevice* device)
{
    device->SetTexture(0, m_texDxt);
    device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

    device->Draw2DSprite(
        izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
        izanagi::CIntRect(300, 500, 256, 128));
}
