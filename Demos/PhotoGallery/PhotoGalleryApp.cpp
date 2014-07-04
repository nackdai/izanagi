#include "PhotoGalleryApp.h"
#include "Seat.h"
#include "PhotoItemManager.h"
#include "LoadTextureJob.h"
#include "StateManager.h"

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
        izanagi::math::CVector(0.0f, 5.0f, -50.0f, 1.0f),
        izanagi::math::CVector(0.0f, 5.0f, -100.0f, 1.0f),
        izanagi::math::CVector(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    TextureLoader::Instance().Init(allocator);

    m_Detector.Init(allocator, &m_Listener);

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
    m_Detector.Update();

    GetCamera().Update();

    StateManager::Instance().Update(IZ_NULL);

    izanagi::threadmodel::CJobQueue::UpdateQueues();
}

// 描画.
void PhotoGalleryApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    StateManager::Instance().Render(device);
}

IZ_BOOL PhotoGalleryApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    m_Detector.PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_DOWN,
            point.x, point.y));
    return IZ_TRUE;
}

IZ_BOOL PhotoGalleryApp::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    m_Detector.PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_UP,
            point.x, point.y));
    return IZ_TRUE;
}

void PhotoGalleryApp::OnMouseMove(const izanagi::CIntPoint& point)
{
    m_Detector.PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_MOVE,
            point.x, point.y));
}
