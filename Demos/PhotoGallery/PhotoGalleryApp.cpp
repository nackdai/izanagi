#include "PhotoGalleryApp.h"
#include "PhotoItemManager.h"
#include "LoadTextureJob.h"
#include "StateManager.h"
#include "Configure.h"
#include "GestureListener.h"
#include "Utility.h"
#include "Environment.h"
#include "data/PhotoFiles.h"
#include "BGRenderer.h"
#include "PostEffect.h"

PhotoGalleryApp::PhotoGalleryApp()
{
}

PhotoGalleryApp::~PhotoGalleryApp()
{
}

// 初期化.
IZ_BOOL PhotoGalleryApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    //izanagi::math::CMathRand::Init((IZ_UINT)app->GetTimer(0).GetCurTime());
    srand((IZ_UINT)GetTimer(0).GetCurTime());

    PhotoItemManager::Instance().Init(
        allocator,
        device,
        Configure::PHOTOS);

    // カメラ
    camera.Init(
        izanagi::math::CVector(0.0f, 5.0f, -Configure::InnerRadius + Configure::CameraDistance, 1.0f),
        izanagi::math::CVector(0.0f, 5.0f, -Configure::InnerRadius, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    TextureLoader::Instance().Init(allocator);

    GestureDetector::Instance().Init(
        allocator,
        this,
        device->GetViewport());

    StateManager::Instance().Create(camera);
    StateManager::Instance().Init();

    IZ_UINT photoNum = (Configure::PHOTOS > COUNTOF(files) ? COUNTOF(files) : Configure::PHOTOS);
    for (IZ_UINT i = 0; i < photoNum; i++) {
        PhotoItemManager::Instance().EnqueueLoadingRequest(
            device,
            files[i]);
    }

    BGRenderer::Instance().Init(allocator, device);

    PostEffect::Instance().Init(allocator, device);
    PostEffect::Instance().Read("data/BloomStar.pes", device);

    m_Timer.Begin();

    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void PhotoGalleryApp::ReleaseInternal()
{
    TextureLoader::Instance().Terminate();
    izanagi::threadmodel::CJobQueue::TerminateJobQueue();

    PhotoItemManager::Instance().Terminate();

    BGRenderer::Instance().Terminate();

    PostEffect::Instance().Terminate();
}

// 更新.
void PhotoGalleryApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    IZ_FLOAT time = m_Timer.End();
    m_Timer.Begin();

    izanagi::CCamera& camera = GetCamera();

    PhotoItemManager::Instance().Update(time, device, camera);

    StateManager::Instance().Update(time, IZ_NULL);

    GetCamera().Update();

    izanagi::threadmodel::CJobQueue::UpdateQueues();
}

// 描画.
void PhotoGalleryApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    const izanagi::CCamera& camera = GetCamera();

    BGRenderer::Instance().PrepareToRender(device, camera);

    PostEffect::Instance().BeginScene(device);
    {
        BGRenderer::Instance().Render(device, camera);
        StateManager::Instance().Render(device);
    }
    PostEffect::Instance().Apply(device);

#ifdef __IZ_DEBUG__
    State state = StateManager::Instance().GetCurrentState();

    const char* name[] = {
        "Default",
        "RotateByFling",
        "ChnageView",
        "MoveToItem",
        "RotateByDrag",
    };

    izanagi::CDebugFont* font = GetDebugFont();

    if (device->Begin2D()) {
        font->Begin(device);

        font->DBPrint(
            device,
            0,
            100,
            0xffffffff,
            name[state]);

        const izanagi::math::SVector& dir = Environment::Instance().GetDir();

        font->DBPrint(
            device,
            0,
            120,
            0xffffffff,
            "[%f][%f][%f]",
            dir.x, dir.y, dir.z);

        font->End();

        device->End2D();
    }
#endif  // #ifdef __IZ_DEBUG__
}

IZ_BOOL PhotoGalleryApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
#ifdef __IZ_DEBUG__
    izanagi::math::SVector& dir = Environment::Instance().GetDir();

    switch (key) {
    case izanagi::sys::E_KEYBOARD_BUTTON_X:
        dir.x += 0.01f;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_Y:
        dir.y += 0.01f;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_Z:
        dir.z += 0.01f;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_0:
        dir.x -= 0.01f;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_1:
        dir.y -= 0.01f;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_2:
        dir.z -= 0.01f;
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_N:
        izanagi::math::SVector::Normalize(dir, dir);
        break;
    case izanagi::sys::E_KEYBOARD_BUTTON_D:
        dir.x = -0.679090f;
        dir.y = -0.701356f;
        dir.z = -0.216650f;
        break;
    }
#endif  // #ifdef __IZ_DEBUG__

    return StateManager::Instance().OnKeyDown(key);
}

IZ_BOOL PhotoGalleryApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    return StateManager::Instance().OnMouseLBtnDown(point);
}

IZ_BOOL PhotoGalleryApp::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    return StateManager::Instance().OnMouseLBtnUp(point);
}

void PhotoGalleryApp::OnMouseMove(const izanagi::CIntPoint& point)
{
    StateManager::Instance().OnMouseMove(point);
}
