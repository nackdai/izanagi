#if !defined(__FRUSTUM_CULLING_APP_H__)
#define __FRUSTUM_CULLING_APP_H__

#include "izSampleKit.h"
#include "izCollision.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class FrustumCullingApp : public izanagi::sample::CSampleApp {
public:
    FrustumCullingApp();
    virtual ~FrustumCullingApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
    static const IZ_UINT CUBE_NUM = 5;

    struct Cube {
        izanagi::CDebugMesh* mesh{ nullptr };
        izanagi::math::SMatrix44 mtxL2W;
        izanagi::col::AABB aabb;
    } m_cubes[CUBE_NUM];

    izanagi::CDebugMeshAxis* m_Axis{ nullptr };
    izanagi::CDebugMeshGrid* m_Grid{ nullptr };
    izanagi::CDebugMeshFrustum* m_FrustumMesh{ nullptr };

    izanagi::shader::CShaderBasic* m_Shader;

    izanagi::col::Frustum m_frustum;

    izanagi::math::SMatrix44 m_mtxW2V;
    izanagi::math::SMatrix44 m_mtxW2C;
    izanagi::math::SMatrix44 m_mtxL2W_Frustum;
};

#endif    // #if !defined(__FRUSTUM_CULLING_APP_H__)