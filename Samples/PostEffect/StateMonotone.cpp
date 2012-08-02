#include "StateMonotone.h"
#include "StateManager.h"
#include "SceneRenderer.h"

CStateMonotone::CStateMonotone(
	izanagi::sample::CSampleApp* app,
	izanagi::SCameraParam& camera)
: CStateBase(app, camera)
{
}

CStateMonotone::~CStateMonotone()
{
	Destroy();
}

// 初期化.
IZ_BOOL CStateMonotone::Init()
{
	// Nothing is done...
	return IZ_TRUE;
}

// 更新
IZ_BOOL CStateMonotone::Update()
{
	return IZ_TRUE;
}

// 描画.
IZ_BOOL CStateMonotone::Render(izanagi::CGraphicsDevice* device)
{
    CSceneRenderer::GetInstance()->Render(m_Camera, device);
	RenderName(device, "Monotone");

	return IZ_TRUE;
}

// 開始
IZ_BOOL CStateMonotone::Enter(
	izanagi::IMemoryAllocator* allocator,
	void* val)
{
    return IZ_TRUE;
}

// 終了.
IZ_BOOL CStateMonotone::Destroy()
{
	return Leave();
}

// ステートから抜ける（終了）.
IZ_BOOL CStateMonotone::Leave()
{
	return IZ_TRUE;
}

// キー押下
IZ_BOOL CStateMonotone::OnKeyDown(IZ_UINT nChar)
{
	if (nChar == VK_UP) {
		CStateManager::GetInstance().ChangeState(State_Monotone + 1);
	}
	else if (nChar == VK_DOWN) {
		CStateManager::GetInstance().ChangeState(State_Monotone - 1);
	}

	return IZ_TRUE;
}
