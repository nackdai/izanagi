#include "StateMonotone.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "PostEffectSample.h"

CStateMonotone::CStateMonotone(
	izanagi::sample::CSampleApp* app,
	izanagi::SCameraParam& camera)
: CStatePostEffect(app, camera)
{
}

CStateMonotone::~CStateMonotone()
{
}

// 描画.
IZ_BOOL CStateMonotone::Render(izanagi::graph::CGraphicsDevice* device)
{
    CStatePostEffect::Render(device);
	RenderName(device, "Monotone");

	return IZ_TRUE;
}

// 開始
IZ_BOOL CStateMonotone::Enter(
	izanagi::IMemoryAllocator* allocator,
	void* val)
{
    IZ_BOOL ret = CPostEffectSample::GetInstance()->Read("data/Monotone.pes");
    return ret;
}
