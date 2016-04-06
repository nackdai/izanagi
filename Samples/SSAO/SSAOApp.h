#if !defined(__SSAO_APP_H__)
#define __SSAO_APP_H__

#include "izSampleKit.h"
#include "izSceneGraph.h"
#include "izDebugUtil.h"
#include "GBuffer.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class SSAOApp : public izanagi::sample::CSampleApp {
public:
    SSAOApp();
    virtual ~SSAOApp();

public:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera) override;

    // 解放.
    virtual void ReleaseInternal() override;

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device) override;

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device) override;

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    IZ_BOOL initMeshes(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    void renderGeometryPass(izanagi::graph::CGraphicsDevice* device);
    void renderSSAOPass(izanagi::graph::CGraphicsDevice* device);
    void renderFinalPass(izanagi::graph::CGraphicsDevice* device);

protected:
    static const IZ_UINT MESH_NUM = 5;

    struct Mesh {
        izanagi::CDebugMesh* mesh{ nullptr };
        izanagi::math::CMatrix44 mtxL2W;
    } m_meshes[MESH_NUM];

    izanagi::shader::CShaderBasic* m_Shader{ nullptr };

    izanagi::CImage* m_Img{ nullptr };

    GBuffer m_gbuffer;

    izanagi::CDebugMesh* m_screenFillPlane{ nullptr };
};

#endif    // #if !defined(__SSAO_APP_H__)
