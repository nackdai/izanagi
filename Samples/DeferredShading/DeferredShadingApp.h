#if !defined(__DEFERRED_SHADING_APP_H__)
#define __DEFERRED_SHADING_APP_H__

#include "izSampleKit.h"
#include "izShader.h"
#include "GBuffer.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class DeferredShadingApp : public izanagi::sample::CSampleApp {
public:
    DeferredShadingApp();
    virtual ~DeferredShadingApp();

protected:
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

private:
    IZ_BOOL initMeshes(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device);

    IZ_BOOL initLights();

    void renderGeometryPass(izanagi::graph::CGraphicsDevice* device);
    void renderLightPass(izanagi::graph::CGraphicsDevice* device);
    void renderFinalPass(izanagi::graph::CGraphicsDevice* device);

private:
    static const IZ_UINT MESH_NUM = 5;
    static const IZ_UINT POINT_LIGHT_NUM = 2;

    struct Cube {
        izanagi::CDebugMesh* mesh{ nullptr };
        izanagi::math::CMatrix44 mtxL2W;
    } m_meshes[MESH_NUM];

    izanagi::shader::CShaderBasic* m_Shader{ nullptr };

    izanagi::CImage* m_Img{ nullptr };

    GBuffer m_gbuffer;

    izanagi::SPointLightParam m_pointLights[POINT_LIGHT_NUM];
    izanagi::CDebugMesh* m_pointLitSphere{ nullptr };

    izanagi::CDebugMesh* m_screenFillPlane{ nullptr };
};

#endif    // #if !defined(__DEFERRED_SHADING_APP_H__)