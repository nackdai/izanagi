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
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Color);

    // Depth.
    res = m_gbuffer->addBuffer(
        device,
        width, height,
        izanagi::graph::E_GRAPH_PIXEL_FMT_R32F);
    VRETURN(std::get<1>(res) && std::get<0>(res) == Depth);

	// Debug.
	res = m_gbuffer->addBuffer(
		device,
		width, height,
		izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA32F);
	VRETURN(std::get<1>(res) && std::get<0>(res) == Debug);

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
        Color,
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

void GBuffer::bindForSSRPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::engine::GBuffer::BindOp ops[] = {
        { Color, 0, "s0" },
        { Depth, 1, "s1" },
    };

    m_gbuffer->bind(
        device,
        ops, COUNTOF(ops));
}

void GBuffer::beginDebug(izanagi::graph::CGraphicsDevice* device)
{
	IZ_UINT targets[] = {
		Debug,
	};

	m_gbuffer->begin(
		device,
		targets, COUNTOF(targets));
}

void GBuffer::endDebug(izanagi::graph::CGraphicsDevice* device)
{
	m_gbuffer->end(device);
}

void GBuffer::drawBuffers(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
        Color,
        Depth,
		Debug,
    };

    m_gbuffer->dumpBuffers(
        device,
        targets, COUNTOF(targets),
        0, 100,
        320, 180);
}