#if !defined(__BG_RENDERER_H__)
#define __BG_RENDERER_H__

#include "izSampleKit.h"

class Seat;
class BG;

class BGRenderer {
    static BGRenderer instance;

public:
    static BGRenderer& Instance();

private:
    BGRenderer();
    virtual ~BGRenderer();

public:
    // Initialization.
    IZ_BOOL Init(
        izanagi::IMemoryAllocator* allocator, 
        izanagi::graph::CGraphicsDevice* device);

    void Terminate();

    void PrepareToRender(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    void Render(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

private:
    void RenderToMRT(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    void RenderSSAO(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    void RenderDebug(izanagi::graph::CGraphicsDevice* device);
    void RenderDebugPointLight(izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::graph::CRenderTarget* m_Light;
    izanagi::graph::CRenderTarget* m_Depth;
    izanagi::graph::CRenderTarget* m_Position;
    izanagi::graph::CRenderTarget* m_Normal;

    izanagi::graph::CRenderTarget* m_RT[4];

    izanagi::graph::CVertexBuffer* m_VB;
    izanagi::graph::CVertexDeclaration* m_VD;

    izanagi::shader::CShaderBasic* m_Shader;

    Seat* m_Seat;
    BG* m_BG;
};

#endif    // #if !defined(__BG_RENDERER_H__)