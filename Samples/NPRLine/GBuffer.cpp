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
    VRETURN(std::get<1>(res) && std::get<0>(res) == Type::Color);

    // Normal Depth.
	res = m_gbuffer->addBuffer(
		device,
		width, height,
		izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA32F);
	VRETURN(std::get<1>(res) && std::get<0>(res) == Type::NmlDepth);

	// Geom Id.
	res = m_gbuffer->addBuffer(
		device,
		width, height,
		izanagi::graph::E_GRAPH_PIXEL_FMT_R32F);
	VRETURN(std::get<1>(res) && std::get<0>(res) == Type::GeomId);

	m_gbuffer->getBuffer(Type::NmlDepth)->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	m_gbuffer->getBuffer(Type::GeomId)->setClearColor(0.0f, 0.0f, 0.0f, 0.0f);

    return IZ_TRUE;
}

void GBuffer::release()
{
    if (m_gbuffer) {
        m_gbuffer->removeAll();
        SAFE_RELEASE(m_gbuffer);
    }
}

void GBuffer::begin(
    izanagi::graph::CGraphicsDevice* device,
    IZ_COLOR bgColor)
{
    IZ_UINT targets[] = {
        Type::Color,
		Type::NmlDepth,
		Type::GeomId,
    };

    m_gbuffer->begin(
        device,
        targets, COUNTOF(targets),
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
        bgColor);
}

void GBuffer::end(izanagi::graph::CGraphicsDevice* device)
{
    m_gbuffer->end(device);
}

void GBuffer::bindForFinalPass(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::engine::GBuffer::BindOp ops[] = {
        { Type::Color, 0, "s0" },
        { Type::NmlDepth, 1, "s1" },
		{ Type::GeomId, 2, "s2" },
    };

    m_gbuffer->getBuffer(Type::Color)->SetFilter(
        izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR,
        izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR,
        izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR_MIPMAP_LINEAR);
    m_gbuffer->getBuffer(Type::NmlDepth)->SetFilter(
        izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR,
        izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR,
        izanagi::graph::E_GRAPH_TEX_FILTER_LINEAR_MIPMAP_LINEAR);
	m_gbuffer->getBuffer(Type::GeomId)->SetFilter(
		izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
		izanagi::graph::E_GRAPH_TEX_FILTER_POINT,
		izanagi::graph::E_GRAPH_TEX_FILTER_NEAREST_MIPMAP_NEAREST);

    m_gbuffer->bind(
        device,
        ops, COUNTOF(ops));
}

void GBuffer::drawBuffers(izanagi::graph::CGraphicsDevice* device)
{
    IZ_UINT targets[] = {
		Type::Color,
        Type::NmlDepth,
		Type::GeomId,
    };

    m_gbuffer->dumpBuffers(
        device,
        targets, COUNTOF(targets),
        0, 100,
        320, 180);
}