#if !defined(__RAY_HIT_APP_H__)
#define __RAY_HIT_APP_H__

#include "izSampleKit.h"
#include "izCollision.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

struct Hitable {
    izanagi::col::BoundingVolume* bound{ nullptr };
    izanagi::CDebugMesh* mesh{ nullptr };

    IZ_BOOL isHit{ IZ_FALSE };

    void release()
    {
        SAFE_RELEASE(mesh);
    }
};

class RayHitApp : public izanagi::sample::CSampleApp {
public:
    RayHitApp();
    virtual ~RayHitApp();

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

    virtual IZ_BOOL OnMouseLBtnDown(const izanagi::CIntPoint& point) override;

private:
    static const IZ_UINT HitableNum = 3;

    izanagi::shader::CShaderBasic* m_Shader;

    Hitable m_hitable[HitableNum];

    izanagi::col::Rectangle m_rect;
    izanagi::col::Sphere m_sphere;
    izanagi::col::AABB m_cube;

    IZ_BOOL m_isPressedKey{ IZ_FALSE };

    izanagi::CIntPoint m_mousePoint;
};

#endif    // #if !defined(__RAY_HIT_APP_H__)