#include "StateCollada.h"
#include "izGraph.h"
#include "izIo.h"
#include "StateManager.h"

CStateCollada::CStateCollada(izanagi::sample::CSampleApp* app)
: CStateBase(app)
{
}

CStateCollada::~CStateCollada()
{
    Destroy();
}

// 開始
IZ_BOOL CStateCollada::Enter(
    izanagi::IMemoryAllocator* allocator,
    void* val)
{
    izanagi::graph::CGraphicsDevice* device = reinterpret_cast<izanagi::graph::CGraphicsDevice*>(val);

    IZ_BOOL result = InitObject(
        allocator,
        device,
        30.0f,
        "data/ModelImage.img",
        "data/Seymour.msh",
        "data/Seymour.skl",
        "data/SkinShader.shd");

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateCollada::Leave()
{
    ReleaseObject();
    return IZ_TRUE;
}
