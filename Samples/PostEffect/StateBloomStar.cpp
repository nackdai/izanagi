#include "StateBloomStar.h"
#include "StateManager.h"
#include "SceneRenderer.h"
#include "PostEffectSample.h"

CStateBloomStar::CStateBloomStar(
    izanagi::sample::CSampleApp* app,
    izanagi::SCameraParam& camera)
: CStatePostEffect(app, camera)
{
}

CStateBloomStar::~CStateBloomStar()
{
}

// 描画.
IZ_BOOL CStateBloomStar::Render(izanagi::graph::CGraphicsDevice* device)
{
    CStatePostEffect::Render(device);
    RenderName(device, "BloomStar");

    return IZ_TRUE;
}

// 開始
IZ_BOOL CStateBloomStar::Enter(
    izanagi::IMemoryAllocator* allocator,
    void* val)
{
    IZ_BOOL ret = CPostEffectSample::GetInstance()->Read("data/BloomStar.pes");
    return ret;
}
