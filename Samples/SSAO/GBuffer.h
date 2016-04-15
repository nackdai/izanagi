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

    IZ_BOOL beginGeometryPass(izanagi::graph::CGraphicsDevice* device);
    IZ_BOOL endGeometryPass(izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL beginSSAOPass(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader);
    IZ_BOOL endSSAOPass(
        izanagi::graph::CGraphicsDevice* device,
        izanagi::shader::CShaderBasic* shader);

    void drawBuffers(izanagi::graph::CGraphicsDevice* device);

public:
    enum Type {
        Albedo,
        Normal,
        Depth,
        Position,

        Num,
    };

    izanagi::graph::CRenderTarget* getBuffer(Type type)
    {
        IZ_ASSERT(type < Type::Num);
        return m_buffers[type];
    }

    izanagi::graph::CRenderTarget* getSSAOBuffer()
    {
        return m_SSAOBuffer;
    }

private:
    izanagi::graph::CRenderTarget* m_buffers[Type::Num];

    izanagi::graph::CRenderTarget* m_SSAOBuffer{ nullptr };
};

#endif    // #if !defined(__GBUFFER_H__)