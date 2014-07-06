#include "PhotoGalleryApp.h"
#include "Seat.h"
#include "PhotoItemManager.h"
#include "LoadTextureJob.h"
#include "StateManager.h"
#include "Configure.h"
#include "GestureListener.h"
#include "Utility.h"
#include "Environment.h"
#include "data/PhotoFiles.h"

PhotoGalleryApp::PhotoGalleryApp()
{
    m_Seat = IZ_NULL;
    m_Shader = IZ_NULL;
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

    {
        izanagi::CFileInputStream input;
        VRETURN(input.Open("data/SeatShader.shd"));

        m_Shader = izanagi::shader::CShaderBasic::CreateShader<izanagi::shader::CShaderBasic>(
            allocator,
            device,
            &input);
        VRETURN(m_Shader != IZ_NULL);
    }

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

    m_Timer.Begin();

__EXIT__:
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

    SAFE_RELEASE(m_Seat);

    SAFE_RELEASE(m_Shader);

    PhotoItemManager::Instance().Terminate();
}

// 更新.
void PhotoGalleryApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    IZ_FLOAT time = m_Timer.End();
    m_Timer.Begin();

    StateManager::Instance().Update(time, IZ_NULL);

    GetCamera().Update();

    izanagi::threadmodel::CJobQueue::UpdateQueues();
}

// 描画.
void PhotoGalleryApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    {
        const izanagi::CCamera& camera = GetCamera();

        izanagi::math::SMatrix mtx;
        izanagi::math::SMatrix::SetUnit(mtx);

        m_Shader->Begin(device, 0, IZ_FALSE);
        {
            if (m_Shader->BeginPass(0)) {
                Utility::SetShaderParam(
                    m_Shader,
                    "g_mW2C",
                    (void*)&camera.GetParam().mtxW2C,
                    sizeof(camera.GetParam().mtxW2C));

                Utility::SetShaderParam(
                    m_Shader,
                    "g_mL2W",
                    (void*)&mtx,
                    sizeof(mtx));

                Environment::Instance().SetPointLightParam(m_Shader);

                m_Shader->CommitChanges(device);

                m_Seat->Render(device);
            }
            m_Shader->EndPass();
        }
        m_Shader->End(device);
    }

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
