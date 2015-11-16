#include "StateFbx.h"
#include "izGraph.h"
#include "izSystem.h"
#include "StateManager.h"

CStateFbx::CStateFbx(izanagi::sample::CSampleApp* app)
: CStateBase(app)
{
}

CStateFbx::~CStateFbx()
{
    Destroy();
}

// 開始
IZ_BOOL CStateFbx::Enter(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::CValue& arg)
{
    IZ_BOOL result = InitObject(
        allocator,
        device,
        30.0f,
        "data/ModelImage.img",
        "data/SeymourFbx.msh",
        "data/SeymourFbx.skl",
        "data/SkinShader.shd");

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateFbx::Leave()
{
    ReleaseObject();
    return IZ_TRUE;
}

IZ_BOOL CStateFbx::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_BOOL ret = CStateBase::Render(device);

    RenderName(device, "Fbx");

    return ret;
}
