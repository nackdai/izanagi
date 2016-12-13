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
    VRETURN(std::get<1>(res) && std::get<0>(res) == Depth);

    // SSAO
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == SSAO);

    // Blur
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Blur);

    return IZ_TRUE;
}

void GBuffer::clear()
{
    if (m_gbuffer) {
        m_gbuffer->removeAll();
        SAFE_RELEASE(m_gbuffer);
    }
}

void GBuffer::beginGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        Albedo,
        Normal,
        Depth,
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
        SSAO,
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
        { Depth, 2, "s2" },
    };

    m_gbuffer->bind(
        device,
        ops, COUNTOF(ops));
}

void GBuffer::bindForBlurPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::engine::GBuffer::BindOp ops[] = {
        { SSAO, 0, "s0" },
        { Depth, 1, "s1" },
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
        Depth,
        SSAO,
        Blur,
    };

    m_gbuffer->dumpBuffers(
        device,
        targets, COUNTOF(targets),
        0, 100,
        320, 180);
}