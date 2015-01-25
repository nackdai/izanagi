#include "AppGalleryApp.h"
#include "LoadTextureJob.h"
#include "StateManager.h"
#include "GestureListener.h"
#include "Scene.h"
#include "ItemManager.h"
#include "Configure.h"
#include "LoadTextureJob.h"
#include "data/ImageFiles.h"

AppGalleryApp::AppGalleryApp()
{
}

AppGalleryApp::~AppGalleryApp()
{
}

// 初期化.
IZ_BOOL AppGalleryApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    //izanagi::math::CMathRand::Init((IZ_UINT)app->GetTimer(0).GetCurTime());
    srand((IZ_UINT)GetTimer(0).GetCurTime());

    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 0.0f, -45.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 0.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
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

    ItemManager::Instance().Init(
        allocator,
        device);

    IZ_UINT imageNum = (Configure::ItemNum > COUNTOF(files) ? COUNTOF(files) : Configure::ItemNum);
    for (IZ_UINT i = 0; i < imageNum; i++) {
        ItemManager::Instance().EnqueueLoadingRequest(
            device,
            files[i]);
    }

    Scene::Instance().Init(
        allocator,
        device);

    m_Timer.Begin();

    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void AppGalleryApp::ReleaseInternal()
{
    TextureLoader::Instance().Terminate();
    izanagi::threadmodel::CJobQueue::TerminateJobQueue();

    ItemManager::Instance().Terminate();
    Scene::Instance().Terminate();
}

// 更新.
void AppGalleryApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    IZ_FLOAT time = m_Timer.End();
    m_Timer.Begin();

    izanagi::CCamera& camera = GetCamera();

    StateManager::Instance().Update(time, IZ_NULL);

    GetCamera().Update();

    Scene::Instance().Update(time, device, camera);

    izanagi::threadmodel::CJobQueue::UpdateQueues();
}

// 描画.
void AppGalleryApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    const izanagi::CCamera& camera = GetCamera();

    Scene::Instance().Render(
        camera,
        device);
}

IZ_BOOL AppGalleryApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return StateManager::Instance().OnKeyDown(key);
}

IZ_BOOL AppGalleryApp::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    return StateManager::Instance().OnMouseLBtnDown(point);
}

IZ_BOOL AppGalleryApp::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    return StateManager::Instance().OnMouseLBtnUp(point);
}

void AppGalleryApp::OnMouseMove(const izanagi::CIntPoint& point)
{
    StateManager::Instance().OnMouseMove(point);
}
