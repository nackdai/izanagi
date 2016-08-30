#include "SpringArmedCameraApp.h"

SpringArmedCameraApp::SpringArmedCameraApp()
{
}

SpringArmedCameraApp::~SpringArmedCameraApp()
{
}

// 初期化.
IZ_BOOL SpringArmedCameraApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    // カメラ
    camera.Init(
        izanagi::math::CVector4(0.0f, 80.0f, -160.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 80.0f, 0.0f, 1.0f),
        izanagi::math::CVector4(0.0f, 1.0f, 0.0f, 1.0f),
        1.0f,
        500.0f,
        izanagi::math::CMath::Deg2Rad(60.0f),
        (IZ_FLOAT)device->GetBackBufferWidth() / device->GetBackBufferHeight());
    camera.Update();

    IZ_BOOL result = m_player.init(
        allocator,
        device,
        camera);

__EXIT__:
    if (!result) {
        ReleaseInternal();
    }

    return IZ_TRUE;
}

// 解放.
void SpringArmedCameraApp::ReleaseInternal()
{
    m_player.release();
} 

// 更新.
void SpringArmedCameraApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    auto& camera = GetCamera();

    camera.Update();

    IZ_FLOAT elapsed = GetTimer(0).GetTime();
    //elapsed /= 1000.0f;
    elapsed = 16.67f / 1000.0f;

    m_player.update(device, camera, elapsed);
}

// 描画.
void SpringArmedCameraApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    auto& camera = GetCamera();

    m_player.render(device, camera);

    if (device->Begin2D()) {
        izanagi::CDebugFont* debugFont = GetDebugFont();

        debugFont->Begin(device, 0, izanagi::CDebugFont::FONT_SIZE * 2);

        auto speed = m_player.getSpeed();

        debugFont->DBPrint(
            device,
            "spped [%.2f]\n",
            speed);

        debugFont->End();

        device->End2D();
    }
}

IZ_BOOL SpringArmedCameraApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    m_player.OnKeyDown(key);
    return IZ_TRUE;
}

void SpringArmedCameraApp::OnKeyUp(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    m_player.OnKeyUp(key);
}