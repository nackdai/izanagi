#include "StatePostEffect.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "PostEffectSample.h"

CStatePostEffect::CStatePostEffect(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
}

CStatePostEffect::~CStatePostEffect()
{
}

// 初期化.
IZ_BOOL CStatePostEffect::Init()
{
    // Nothing is done...
    return IZ_TRUE;
}

// 更新
IZ_BOOL CStatePostEffect::Update()
{
    return IZ_TRUE;
}

// 描画.
IZ_BOOL CStatePostEffect::Render(izanagi::graph::CGraphicsDevice* device)
{
    CPostEffectSample::GetInstance()->BeginScene(device);
    {
        device->Clear(
            izanagi::graph::E_GRAPH_CLEAR_FLAG_COLOR, 
            IZ_COLOR_RGBA(0, 0, 0, 0xff), 
            1.0f, 0);
        CSceneRenderer::GetInstance()->Render(m_Camera, device);
    }
    CPostEffectSample::GetInstance()->Apply(device);

    return IZ_TRUE;
}

// 終了.
IZ_BOOL CStatePostEffect::Destroy()
{
    return Leave();
}

// ステートから抜ける（終了）.
IZ_BOOL CStatePostEffect::Leave()
{
    return IZ_TRUE;
}

// キー押下
IZ_BOOL CStatePostEffect::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    State cur = CStateManager::GetInstance().GetCurrentState();

    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        CStateManager::GetInstance().ChangeState(cur + 1);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        CStateManager::GetInstance().ChangeState(cur - 1);
    }

    return IZ_TRUE;
}
