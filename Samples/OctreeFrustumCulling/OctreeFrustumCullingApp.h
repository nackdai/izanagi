#if !defined(__OCTREE_FRUSTUM_CULLING_APP_H__)
#define __OCTREE_FRUSTUM_CULLING_APP_H__

#include "izSampleKit.h"
#include "izCollision.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class OctreeFrustumCullingApp : public izanagi::sample::CSampleApp {
public:
    OctreeFrustumCullingApp();
    virtual ~OctreeFrustumCullingApp();

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

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;
    virtual void OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key) override;

    virtual void OnMouseMoveInternal(const izanagi::CIntPoint& point) override;

private:
    class Cube : public izanagi::col::OctreeNode {
    public:
        Cube() {}
        virtual ~Cube()
        {
            SAFE_RELEASE(mesh);
        }

    public:
        izanagi::CDebugMesh* mesh{ nullptr };
        izanagi::math::SMatrix44 mtxL2W;
    };

    izanagi::col::Octree<Cube> m_octree;

    izanagi::CDebugMeshAxis* m_Axis{ nullptr };
    izanagi::CDebugMeshGrid* m_Grid{ nullptr };
    izanagi::CDebugMeshFrustum* m_FrustumMesh{ nullptr };

    izanagi::shader::CShaderBasic* m_Shader;

    izanagi::col::Frustum m_frustum;

    IZ_BOOL m_isEnableInternalCamera;
    izanagi::sample::CSampleCamera m_internalCamera;
};

#endif    // #if !defined(__OCTREE_FRUSTUM_CULLING_APP_H__)