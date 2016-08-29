#if !defined(__SPRING_ARMED_CAMERA_APP_H__)
#define __SPRING_ARMED_CAMERA_APP_H__

#include "izSampleKit.h"
#include "PlayerCharacter.h"

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

class SpringArmedCameraApp : public izanagi::sample::CSampleApp
{
    class CTimeOverHandler : public izanagi::animation::CTimeline::CTimeOverHandler
    {
    public:
        CTimeOverHandler(SpringArmedCameraApp* app) { m_App = app; }
        virtual ~CTimeOverHandler() {}

    public:
        void Handle(const izanagi::animation::CTimeline& timeline);

    private:
        SpringArmedCameraApp* m_App;
    };

public:
    SpringArmedCameraApp();
    virtual ~SpringArmedCameraApp();

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

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

private:
    PlayerCharacter m_player;
};

#endif    // #if !defined(__SPRING_ARMED_CAMERA_APP_H__)
