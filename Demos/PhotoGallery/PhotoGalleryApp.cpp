#include "PhotoGalleryApp.h"
#include "Seat.h"
#include "PhotoItemManager.h"
#include "LoadTextureJob.h"
#include "StateManager.h"
#include "Configure.h"
#include "GestureListener.h"

PhotoGalleryApp::PhotoGalleryApp()
{
    m_Seat = IZ_NULL;
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

    m_Seat = Seat::Create(allocator, device);
    VGOTO(result = (m_Seat != IZ_NULL), __EXIT__);

    PhotoItemManager::Instance().Init(
        allocator,
        device,
        60);

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

    
    camera.Update();

    TextureLoader::Instance().Init(allocator);

    GestureDetector::Instance().Init(
        allocator,
        this,
        device->GetViewport());

    StateManager::Instance().Create(camera);
    StateManager::Instance().Init();

#if 0
    PhotoItemManager::Instance().EnqueueLoadingRequest(
        device,
        "data/test.jpg");
#endif

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void PhotoGalleryApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Seat);

    TextureLoader::Instance().Terminate();
    izanagi::threadmodel::CJobQueue::TerminateJobQueue();

    PhotoItemManager::Instance().Terminate();
}

// 更新.
void PhotoGalleryApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    StateManager::Instance().Update(IZ_NULL);

    GetCamera().Update();

    izanagi::threadmodel::CJobQueue::UpdateQueues();
}

// 描画.
void PhotoGalleryApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    StateManager::Instance().Render(device);

    State state = StateManager::Instance().GetCurrentState();

    const char* name[] = {
        "Default",
        "RotateByFling",
        "ChnageView",
        "MoveToItem",
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

        font->End();

        device->End2D();
    }
}

IZ_BOOL PhotoGalleryApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
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
