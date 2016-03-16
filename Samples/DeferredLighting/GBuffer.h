#if !defined(__GBUFFER_H__)
#define __GBUFFER_H__

#include "izSampleKit.h"

class GBuffer {
public:
    GBuffer();
    virtual ~GBuffer();

public:
    // 初期化.
    IZ_BOOL initialize(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    // 解放.
    void release();

    IZ_BOOL begin(izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL end(izanagi::graph::CGraphicsDevice* device);

    void drawBuffers(izanagi::graph::CGraphicsDevice* device);

public:
    enum Type {
        Albedo,
        Normal,
        Depth,
        Position,

        Num,
    };

private:
    izanagi::graph::CRenderTarget* m_buffers[Type::Num];
};

#endif    // #if !defined(__GBUFFER_H__)