#if !defined(__TARGET_FOLLOW_CAMERA_APP_H__)
#define __TARGET_FOLLOW_CAMERA_APP_H__

#include "izSampleKit.h"
#include "PlayerCharacter.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class TargetFollowCameraApp : public izanagi::sample::CSampleApp
{
    class CTimeOverHandler : public izanagi::animation::CTimeline::CTimeOverHandler
    {
    public:
        CTimeOverHandler(TargetFollowCameraApp* app) { m_App = app; }
        virtual ~CTimeOverHandler() {}

    public:
        void Handle(const izanagi::animation::CTimeline& timeline);

    private:
        TargetFollowCameraApp* m_App;
    };

public:
    TargetFollowCameraApp();
    virtual ~TargetFollowCameraApp();

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

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key) override;
    virtual void OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key) override;

private:
    PlayerCharacter m_player;

    izanagi::CRenderGraph* m_RenderGraph{ nullptr };
    izanagi::CSceneRenderer* m_Renderer{ nullptr };

    izanagi::shader::CShaderBasic* m_shd{ nullptr };
    izanagi::CDebugMeshGrid* m_grid{ nullptr };

    izanagi::engine::TargetFollowCamera m_camera;
};

#endif    // #if !defined(__TARGET_FOLLOW_CAMERA_APP_H__)
