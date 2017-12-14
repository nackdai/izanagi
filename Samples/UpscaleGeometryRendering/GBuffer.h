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

    void release();

    void beginGeometryLowResPass(izanagi::graph::CGraphicsDevice* device);
	void endGeometryLowResPass(izanagi::graph::CGraphicsDevice* device);

	void beginGeometryHiResPass(izanagi::graph::CGraphicsDevice* device);
	void endGeometryHiResPass(izanagi::graph::CGraphicsDevice* device);

    void beginColorPass(
        izanagi::graph::CGraphicsDevice* device,
        IZ_COLOR bgColor);

    void endColorPass(izanagi::graph::CGraphicsDevice* device);

    void bindForFinalPass(izanagi::graph::CGraphicsDevice* device);

    void drawBuffers(izanagi::graph::CGraphicsDevice* device);

public:
    enum Type {
        Color = 0,
        NmlDepth_LowRes,
		NmlDepth_HiRes,

        Num,
    };

	izanagi::graph::CRenderTarget* getBuffer(Type type)
	{
		return m_gbuffer->getBuffer((IZ_UINT)type);
	}

private:
    izanagi::engine::GBuffer* m_gbuffer{ nullptr };
};

#endif    // #if !defined(__GBUFFER_H__)
