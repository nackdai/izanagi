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

    void beginSSAOPass(izanagi::graph::CGraphicsDevice* device);
    void endSSAOPass(izanagi::graph::CGraphicsDevice* device);

    void bindForSSAOPass(izanagi::graph::CGraphicsDevice* device);
    void bindForBlurPass(izanagi::graph::CGraphicsDevice* device);

    void drawBuffers(izanagi::graph::CGraphicsDevice* device);

private:
    enum Type {
        Albedo = 0,
        Normal,
        Depth,
        SSAO,

        Num,
    };

    izanagi::engine::GBuffer* m_gbuffer{ nullptr };
};

#endif    // #if !defined(__GBUFFER_H__)
