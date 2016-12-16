#include "GBuffer.h"

IZ_BOOL GBuffer::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    m_gbuffer = izanagi::engine::GBuffer::crreate(allocator);
    VRETURN(m_gbuffer);

    auto width = device->GetBackBufferWidth();
    auto height = device->GetBackBufferHeight();

    // Color.
    auto res = m_gbuffer->addBuffer(
        device,
        width / 2, height / 2,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Color);

    // Id.
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_R32UI);
        //izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Id);

    return IZ_TRUE;
}

void GBuffer::release()
{
    if (m_gbuffer) {
        m_gbuffer->removeAll();
        SAFE_RELEASE(m_gbuffer);
    }
}

void GBuffer::beginGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        Id,
    };

    m_gbuffer->begin(
        device,
        targets, COUNTOF(targets));
}

void GBuffer::endGeometryPass(izanagi::graph::CGraphicsDevice* device)
{
    m_gbuffer->end(device);
}

void GBuffer::beginColorPass(
    izanagi::graph::CGraphicsDevice* device,
    IZ_COLOR bgColor)
{
    IZ_UINT targets[] = {
        Color,
    };

    m_gbuffer->begin(
        device,
        targets, COUNTOF(targets),
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
        bgColor);
}

void GBuffer::endColorPass(izanagi::graph::CGraphicsDevice* device)
{
    m_gbuffer->end(device);
}

void GBuffer::bindForFinalPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::engine::GBuffer::BindOp ops[] = {
        { Color, 0, "s0" },
        { Id, 1, "s1" },
    };

    m_gbuffer->getBuffer(Color)->SetFilter(
        izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
        izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
        izanagi::graph::E_GRAPH_TEX_FILTER_POINT);
    m_gbuffer->getBuffer(Id)->SetFilter(
        izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
        izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
        izanagi::graph::E_GRAPH_TEX_FILTER_POINT);

    m_gbuffer->bind(
        device,
        ops, COUNTOF(ops));
}

void GBuffer::drawBuffers(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        Color,
        Id,
    };

    m_gbuffer->dumpBuffers(
        device,
        targets, COUNTOF(targets),
        0, 100,
        320, 180);
}