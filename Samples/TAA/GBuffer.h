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

	void beginColorPass(izanagi::graph::CGraphicsDevice* device);
	void endColorPass(izanagi::graph::CGraphicsDevice* device);

	void bindForTAAPass(izanagi::graph::CGraphicsDevice* device);

	void drawBuffers(izanagi::graph::CGraphicsDevice* device);

private:
	enum Type {
		Normal,
		Depth,
		Motion,
		Color,

		Num,
	};

	izanagi::engine::GBuffer* m_gbuffer{ nullptr };
};

#endif    // #if !defined(__GBUFFER_H__)
