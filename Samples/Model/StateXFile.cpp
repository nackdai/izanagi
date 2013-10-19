#include "StateXFile.h"
#include "izGraph.h"
#include "izIo.h"
#include "StateManager.h"

CStateXFile::CStateXFile(izanagi::sample::CSampleApp* app)
: CStateBase(app)
{
}

CStateXFile::~CStateXFile()
{
    Destroy();
}

// 開始
IZ_BOOL CStateXFile::Enter(
    izanagi::IMemoryAllocator* allocator,
    void* val)
{
    izanagi::graph::CGraphicsDevice* device = reinterpret_cast<izanagi::graph::CGraphicsDevice*>(val);

    IZ_BOOL result = InitObject(
        allocator,
        device,
        300.0f,
        "data/ModelImage.img",
        "data/tiny.msh",
        "data/tiny.skl",
        "data/SkinShader.shd");

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateXFile::Leave()
{
    ReleaseObject();
    return IZ_TRUE;
}
