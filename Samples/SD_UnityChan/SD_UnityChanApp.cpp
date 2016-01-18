#include "SD_UnityChanApp.h"
#include "StateManager.h"
#include "StateBase.h"

SDUnityChanApp::SDUnityChanApp()
{
}

SDUnityChanApp::~SDUnityChanApp()
{
}

// キーボード押下
IZ_BOOL SDUnityChanApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return CStateManager::GetInstance().OnKeyDown(key);
}

// 初期化.
IZ_BOOL SDUnityChanApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    CStateManager::GetInstance().Create(this);

    return CStateManager::GetInstance().Init();
}

// 解放.
void SDUnityChanApp::ReleaseInternal()
{
    CStateManager::GetInstance().Destroy();
}

// 更新.
void SDUnityChanApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    CStateManager::GetInstance().Update(
        0.0f,   // Not used.
        m_Allocator, m_Device);
}

// 描画.
void SDUnityChanApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    CStateManager::GetInstance().Render(device);
}

// 背景色取得.
IZ_COLOR SDUnityChanApp::GetBgColor() const
{
    izanagi::CSceneStateBase* state = CStateManager::GetInstance().GetState(
        CStateManager::GetInstance().GetCurrentState());

    IZ_BOOL enableBgColor = (reinterpret_cast<CStateBase*>(state))->EnableBgColor();
    if (enableBgColor)
    {
        return (reinterpret_cast<CStateBase*>(state))->GetBgColor();
    }

    return CSampleApp::GetBgColor();
}
