#if !defined(__GBUFFER_H__)
#define __GBUFFER_H__

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

    void clear();

    void beginGeometryPass(izanagi::graph::CGraphicsDevice* device);
    void endGeometryPass(izanagi::graph::CGraphicsDevice* device);

    void bindForSSRPass(izanagi::graph::CGraphicsDevice* device);

    void drawBuffers(izanagi::graph::CGraphicsDevice* device);

	izanagi::graph::CRenderTarget* getBuffer(IZ_UINT idx)
	{
		return m_gbuffer->getBuffer(idx);
	}

private:
    enum Type {
        Color,
		Depth,

        Num,
    };

    izanagi::engine::GBuffer* m_gbuffer{ nullptr };
};

#endif    // #if !defined(__GBUFFER_H__)
