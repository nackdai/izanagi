#pragma once

#include "izStd.h"
#include "izGraph.h"
#include "izEngine.h"

class GBuffer {
public:
    GBuffer() {}
    ~GBuffer() {}

public:
    IZ_BOOL init(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    void release();

	void begin(
		izanagi::graph::CGraphicsDevice* device,
		IZ_COLOR bgColor);
	void end(izanagi::graph::CGraphicsDevice* device);

    void bindForFinalPass(izanagi::graph::CGraphicsDevice* device);

    void drawBuffers(izanagi::graph::CGraphicsDevice* device);

public:
    enum Type {
        Color = 0,
        NmlDepth,
		GeomId,

        Num,
    };

	izanagi::graph::CRenderTarget* getBuffer(Type type)
	{
		return m_gbuffer->getBuffer((IZ_UINT)type);
	}

private:
    izanagi::engine::GBuffer* m_gbuffer{ nullptr };
};
