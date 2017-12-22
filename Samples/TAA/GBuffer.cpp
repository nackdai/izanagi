#include "GBuffer.h"

IZ_BOOL GBuffer::init(
	izanagi::IMemoryAllocator* allocator,
	izanagi::graph::CGraphicsDevice* device)
{
	m_gbuffer = izanagi::engine::GBuffer::crreate(allocator);
	VRETURN(m_gbuffer);

	auto width = device->GetBackBufferWidth();
	auto height = device->GetBackBufferHeight();

	// Normal.
	auto res = m_gbuffer->addBuffer(
		device,
		width, height,
		izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA8);
	VRETURN(std::get<1>(res) && std::get<0>(res) == Normal);

	// Depth.
	res = m_gbuffer->addBuffer(
		device,
		width, height,
		izanagi::graph::E_GRAPH_PIXEL_FMT_RGBA32F);
	VRETURN(std::get<1>(res) && std::get<0>(res) == Depth);

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

void GBuffer::bindForTAAPass(izanagi::graph::CGraphicsDevice* device)
{
	izanagi::engine::GBuffer::BindOp ops[] = {
		{ Normal, 0, "s0" },
		{ Depth, 1, "s1" },
	};

	m_gbuffer->bind(
		device,
		ops, COUNTOF(ops));
}

void GBuffer::drawBuffers(izanagi::graph::CGraphicsDevice* device)
{
	IZ_UINT targets[] = {
		Normal,
		Depth,
	};

	m_gbuffer->dumpBuffers(
		device,
		targets, COUNTOF(targets),
		0, 100,
		320, 180);
}