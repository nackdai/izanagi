#if !defined(__DEFERRED_LIGHTING_APP_H__)
#define __DEFERRED_LIGHTING_APP_H__

#include "izSampleKit.h"
#include "izShader.h"
#include "GBuffer.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class DeferredLightingApp : public izanagi::sample::CSampleApp {
public:
    DeferredLightingApp();
    virtual ~DeferredLightingApp();

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
    static const IZ_UINT MESH_NUM = 5;

    struct Cube {
        izanagi::CDebugMesh* mesh{ nullptr };
        izanagi::math::SMatrix44 mtxL2W;
    } m_spheres[MESH_NUM];

    izanagi::shader::CShaderBasic* m_Shader{ nullptr };

    izanagi::CImage* m_Img{ nullptr };

    GBuffer m_gbuffer;
};

#endif    // #if !defined(__DEFERRED_LIGHTING_APP_H__)