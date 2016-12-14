#include "GBuffer.h"

IZ_BOOL GBuffer::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_gbuffer = izanagi::engine::GBuffer::crreate(allocator);
    VRETURN(m_gbuffer);

    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    // Albedo.
    auto res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Albedo);

    // Normal.
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Normal);

    // Depth.
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_R32F);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Depth_0);
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_R32F);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Depth_1);

    // SSAO
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == SSAO_0);
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == SSAO_1);

    // Blur
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Blur);

    return IZ_TRUE;
}

void GBuffer::release()
{
    if (m_gbuffer) {
        m_gbuffer->removeAll();
        SAFE_RELEASE(m_gbuffer);
    }
}

void GBuffer::clearBuffer(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        Depth_0,
        Depth_1,
        SSAO_0,
        SSAO_1,
    };

    m_gbuffer->begin(
        device,
        targets, COUNTOF(targets));

    m_gbuffer->end(device);
}

void GBuffer::beginGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        Albedo,
        Normal,
        m_curIdx == 0 ? Depth_0 : Depth_1,
    };

    m_gbuffer->begin(
        device,
        targets, COUNTOF(targets));
}

void GBuffer::endGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    m_gbuffer->end(device);
}

void GBuffer::beginSSAOPass(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        m_curIdx == 0 ? SSAO_0 : SSAO_1,
    };

    m_gbuffer->begin(
        device,
        targets, COUNTOF(targets));
}

void GBuffer::endSSAOPass(izanagi::graph::CGraphicsDevice* device)
{
    m_gbuffer->end(device);
}

void GBuffer::beginBlurPass(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        Blur,
    };

    m_gbuffer->begin(
        device,
        targets, COUNTOF(targets));
}

void GBuffer::endBlurPass(izanagi::graph::CGraphicsDevice* device)
{
    m_gbuffer->end(device);
}


void GBuffer::bindForSSAOPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::engine::GBuffer::BindOp ops[] = {
        { Albedo, 0, "s0" },
        { Normal, 1, "s1" },
        { m_curIdx == 0 ? Depth_0 : Depth_1, 2, "s2" },
        { m_curIdx == 0 ? Depth_1 : Depth_0, 3, "s3" },
        { m_curIdx == 0 ? SSAO_1 : SSAO_0, 4, "s4" },
    };

    m_gbuffer->bind(
        device,
        ops, COUNTOF(ops));
}

void GBuffer::bindForBlurPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::engine::GBuffer::BindOp ops[] = {
        { m_curIdx == 0 ? SSAO_0 : SSAO_1, 0, "s0" },
        { m_curIdx == 0 ? Depth_0 : Depth_1, 1, "s1" },
    };

    m_gbuffer->bind(
        device,
        ops, COUNTOF(ops));
}

void GBuffer::bindForFinalPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::engine::GBuffer::BindOp ops[] = {
        { Albedo, 0, "s0" },
        { Blur, 1, "s1" },
    };

    m_gbuffer->bind(
        device,
        ops, COUNTOF(ops));
}


void GBuffer::drawBuffers(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        Albedo,
        Normal,
        Depth_0,
        Depth_1,
        SSAO_0,
        SSAO_1,
        Blur,
    };

    m_gbuffer->dumpBuffers(
        device,
        targets, COUNTOF(targets),
        0, 100,
        320, 180);
}