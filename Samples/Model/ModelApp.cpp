#include "ModelApp.h"
#include "StateManager.h"
#include "StateBase.h"

CModelApp::CModelApp()
{
}

CModelApp::~CModelApp()
{
}

// キーボード押下
IZ_BOOL CModelApp::OnKeyDown(IZ_UINT nChar)
{
    return CStateManager::GetInstance().OnKeyDown(nChar);
}

// 初期化.
IZ_BOOL CModelApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    CStateManager::GetInstance().Create(this);

    return CStateManager::GetInstance().Init();
}

// 解放.
void CModelApp::ReleaseInternal()
{
    CStateManager::GetInstance().Destroy();
}

// 更新.
void CModelApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    GetCamera().Update();

    CStateManager::GetInstance().Update(m_Allocator, m_Device);
}

// 描画.
void CModelApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    CStateManager::GetInstance().Render(device);
}

// 背景色取得.
IZ_COLOR CModelApp::GetBgColor() const
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
