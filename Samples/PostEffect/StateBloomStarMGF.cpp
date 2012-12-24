#include "StateBloomStarMGF.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "PostEffectSample.h"

CStateBloomStarMGF::CStateBloomStarMGF(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStatePostEffect(app, camera)
{
}

CStateBloomStarMGF::~CStateBloomStarMGF()
{
}

// 描画.
IZ_BOOL CStateBloomStarMGF::Render(izanagi::graph::CGraphicsDevice* device)
{
    CStatePostEffect::Render(device);
    RenderName(device, "BloomStarMGF");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateBloomStarMGF::Enter(
    izanagi::IMemoryAllocator* allocator,
    void* val)
{
    IZ_BOOL ret = CPostEffectSample::GetInstance()->Read("data/BloomStarMGF.pes");
    return ret;
}
