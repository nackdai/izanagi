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
    /** Render scene to MRT for SSAO.
     */
    void RenderToMRT(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    /** Render SSAO scene.
     */
    void RenderSSAO(
        izanagi::graph::CGraphicsDevice* device,
        const izanagi::CCamera& camera);

    void RenderDebug(izanagi::graph::CGraphicsDevice* device);
    void RenderDebugPointLight(izanagi::graph::CGraphicsDevice* device);

private:
    // For SSAO
    izanagi::graph::CRenderTarget* m_Light;     // ライティング
    izanagi::graph::CRenderTarget* m_Depth;     // デプス
    izanagi::graph::CRenderTarget* m_Position;  // 位置
    izanagi::graph::CRenderTarget* m_Normal;    // 法線

    // 上のレンダーターゲットをまとめて扱いやすくする用
    izanagi::graph::CRenderTarget* m_RT[4];

    // 全面に描画する用
    izanagi::graph::CVertexBuffer* m_VB;
    izanagi::graph::CVertexDeclaration* m_VD;

    izanagi::shader::CShaderBasic* m_Shader;

    Seat* m_Seat;
    BG* m_BG;
};

#endif    // #if !defined(__BG_RENDERER_H__)