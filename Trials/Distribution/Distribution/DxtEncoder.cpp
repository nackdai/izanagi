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
        m_tex = device->CreateTexture(
            width/4, height/4,
            1,
            izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8,
            izanagi::graph::E_GRAPH_RSC_USAGE_STATIC);
        VRETURN(m_tex);

        CALL_GL_API(glGenFramebuffers(1, &m_fbo));

        glGenBuffers(1, &m_pbo);
        glBindBuffer(GL_ARRAY_BUFFER, m_pbo);
        glBufferData(GL_ARRAY_BUFFER, width * height, 0, GL_STREAM_COPY);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }

    return IZ_TRUE;
}

void DxtEncoder::encode(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::graph::CTexture* texture)
{
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
    CALL_GL_API(glUniform1i(m_hMode, 0));

    // TODO
    // Why 1/4 ?
    device->SetViewport(
        izanagi::graph::SViewport(0, 0, m_width/4, m_height/4));

    // NOTE
    // 頂点バッファを使わず全画面に描画する頂点シェーダ.
    CALL_GL_API(glDrawArrays(GL_TRIANGLE_STRIP, 0, 4));

    // Readback.
    CALL_GL_API(glReadBuffer(GL_COLOR_ATTACHMENT0));
    CALL_GL_API(glBindBuffer(GL_PIXEL_PACK_BUFFER, m_pbo));
    CALL_GL_API(glReadPixels(
        0, 0,
        m_width/4, m_height/4,
        GL_RGBA_INTEGER,
        GL_UNSIGNED_INT,
        0));
    CALL_GL_API(glBindBuffer(GL_PIXEL_PACK_BUFFER, 0));

    CALL_GL_API(glBindFramebuffer(GL_FRAMEBUFFER, 0));

    device->LoadRenderState();
}

void DxtEncoder::terminate()
{
    SAFE_RELEASE(m_vs);
    SAFE_RELEASE(m_dxt);
    SAFE_RELEASE(m_shd);

    SAFE_RELEASE(m_tex);
}
