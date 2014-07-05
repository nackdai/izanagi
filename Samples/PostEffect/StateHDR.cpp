#include "StateHDR.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "PostEffectSample.h"

CStateHDR::CStateHDR(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStatePostEffect(app, camera)
{
}

CStateHDR::~CStateHDR()
{
}

// 描画.
IZ_BOOL CStateHDR::Render(izanagi::graph::CGraphicsDevice* device)
{
    CStatePostEffect::Render(device);
    RenderName(device, "HDR");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateHDR::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    IZ_BOOL ret = CPostEffectSample::GetInstance()->Read("data/HDR.pes");
    return ret;
}
