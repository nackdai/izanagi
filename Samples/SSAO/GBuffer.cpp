#include "GBuffer.h"

GBuffer::GBuffer()
{
    FILL_ZERO(m_buffers, sizeof(m_buffers));
}

GBuffer::~GBuffer()
{
}

// 初期化.
IZ_BOOL GBuffer::initialize(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    // Albedo.
    m_buffers[Type::Albedo] = device->CreateRenderTarget(
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);

    // Normal.
    m_buffers[Type::Normal] = device->CreateRenderTarget(
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);

    // Depth.
    m_buffers[Type::Depth] = device->CreateRenderTarget(
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_R32F);

    // SSAO.
    m_SSAOBuffer = device->CreateRenderTarget(
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);

    return IZ_TRUE;
}

// 解放.
void GBuffer::release()
{
    for (IZ_UINT i = 0; i < COUNTOF(m_buffers); i++) {
        SAFE_RELEASE(m_buffers[i]);
    }

    SAFE_RELEASE(m_SSAOBuffer);
}

IZ_BOOL GBuffer::beginGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    auto ret = device->BeginScene(
        m_buffers,
        COUNTOF(m_buffers),
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
        IZ_COLOR_RGBA(0xff, 0xff, 0xff, 0x00));

    return ret;
}

IZ_BOOL GBuffer::endGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    device->EndScene();
    return IZ_TRUE;
}

IZ_BOOL GBuffer::beginSSAOPass(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::shader::CShaderBasic* shader)
{
    shader->EnableToUpdateRenderState(
        izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
        IZ_FALSE);
    shader->EnableToUpdateRenderState(
        izanagi::graph::E_GRAPH_RS_ZENABLE,
        IZ_FALSE);
    shader->EnableToUpdateRenderState(
        izanagi::graph::E_GRAPH_RS_ALPHABLENDENABLE,
        IZ_FALSE);
    shader->EnableToUpdateRenderState(
        izanagi::graph::E_GRAPH_RS_BLENDMETHOD,
        IZ_FALSE);

    device->SaveRenderState();

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
        IZ_FALSE);
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZENABLE,
        IZ_FALSE);

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ALPHABLENDENABLE,
        IZ_TRUE);
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_BLENDMETHOD,
        izanagi::graph::E_GRAPH_ALPHA_BLEND_Cs_Cd);

    auto ret = device->BeginScene(
        &m_SSAOBuffer,
        1,
        izanagi::graph::E_GRAPH_CLEAR_FLAG_COLOR,
        IZ_COLOR_RGBA(0x00, 0x00, 0x00, 0xff));

    return ret;
}

IZ_BOOL GBuffer::endSSAOPass(
    izanagi::graph::CGraphicsDevice* device,
    izanagi::shader::CShaderBasic* shader)
{
    device->EndScene();

    device->LoadRenderState();

    return IZ_TRUE;
}

void GBuffer::drawBuffers(izanagi::graph::CGraphicsDevice* device)
{
    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    IZ_UINT x = 0;
    IZ_UINT y = 100;

    if (device->Begin2D()) {
        for (IZ_UINT i = 0; i < COUNTOF(m_buffers); i++) {
            device->SetTexture(0, m_buffers[i]);

            if (y + 180 > height) {
                y = 100;
                x += 320;
            }

            device->Draw2DSprite(
                izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
                izanagi::CIntRect(x, y, 320, 180));

            y += 180;
        }

        if (y + 180 > height) {
            y = 100;
            x += 320;
        }

#if 0
        device->SetTexture(0, m_SSAOBuffer);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(x, y, 320, 180));
#endif

        device->End2D();
    }
}