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

    void clearBuffer(izanagi::graph::CGraphicsDevice* device);

    void beginGeometryPass(izanagi::graph::CGraphicsDevice* device);
    void endGeometryPass(izanagi::graph::CGraphicsDevice* device);

    void beginSSAOPass(izanagi::graph::CGraphicsDevice* device);
    void endSSAOPass(izanagi::graph::CGraphicsDevice* device);

    void beginBlurPass(izanagi::graph::CGraphicsDevice* device);
    void endBlurPass(izanagi::graph::CGraphicsDevice* device);

    void bindForSSAOPass(izanagi::graph::CGraphicsDevice* device);
    void bindForBlurPass(izanagi::graph::CGraphicsDevice* device);
    void bindForFinalPass(izanagi::graph::CGraphicsDevice* device);

    void toggle()
    {
        m_curIdx = 1 - m_curIdx;
    }

    void drawBuffers(izanagi::graph::CGraphicsDevice* device);

private:
    enum Type {
        Albedo = 0,
        Normal,
        Depth_0,
        Depth_1,
        SSAO_0,
        SSAO_1,
        Blur,

        Num,
    };

    izanagi::engine::GBuffer* m_gbuffer{ nullptr };

    IZ_UINT m_curIdx{ 0 };
};

#endif    // #if !defined(__GBUFFER_H__)
