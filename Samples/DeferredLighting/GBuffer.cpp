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

    // Light.
    m_lightBuffer = device->CreateRenderTarget(
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

    SAFE_RELEASE(m_lightBuffer);
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

IZ_BOOL GBuffer::beginLightPass(izanagi::graph::CGraphicsDevice* device)
{
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
        IZ_FALSE);
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZENABLE,
        IZ_FALSE);

    auto ret = device->BeginScene(
        &m_lightBuffer,
        1,
        izanagi::graph::E_GRAPH_CLEAR_FLAG_COLOR,
        IZ_COLOR_RGBA(0x00, 0x00, 0x00, 0xff));

    return ret;
}

IZ_BOOL GBuffer::endLightPass(izanagi::graph::CGraphicsDevice* device)
{
    device->EndScene();

    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZWRITEENABLE,
        IZ_TRUE);
    device->SetRenderState(
        izanagi::graph::E_GRAPH_RS_ZENABLE,
        IZ_TRUE);

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

        device->SetTexture(0, m_lightBuffer);
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(x, y, 320, 180));

        device->End2D();
    }
}