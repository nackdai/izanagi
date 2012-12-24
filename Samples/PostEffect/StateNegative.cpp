#include "StateNegative.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "PostEffectSample.h"

CStateNegative::CStateNegative(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStatePostEffect(app, camera)
{
}

CStateNegative::~CStateNegative()
{
}

// 描画.
IZ_BOOL CStateNegative::Render(izanagi::graph::CGraphicsDevice* device)
{
    CStatePostEffect::Render(device);
    RenderName(device, "Negative");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateNegative::Enter(
    izanagi::IMemoryAllocator* allocator,
    void* val)
{
    IZ_BOOL ret = CPostEffectSample::GetInstance()->Read("data/Negative.pes");
    return ret;
}
