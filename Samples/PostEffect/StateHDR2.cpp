#include "StateHDR2.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "PostEffectSample.h"

CStateHDR_2::CStateHDR_2(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStatePostEffect(app, camera)
{
}

CStateHDR_2::~CStateHDR_2()
{
}

// 描画.
IZ_BOOL CStateHDR_2::Render(izanagi::graph::CGraphicsDevice* device)
{
    CStatePostEffect::Render(device);
    RenderName(device, "HDR2");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateHDR_2::Enter(
    izanagi::IMemoryAllocator* allocator,
    void* val)
{
    IZ_BOOL ret = CPostEffectSample::GetInstance()->Read("data/HDR2.pes");
    return ret;
}
