#include "StateXFile.h"
#include "izGraph.h"
#include "izSystem.h"
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
    izanagi::CValue& arg)
{
    izanagi::graph::CGraphicsDevice* device = reinterpret_cast<izanagi::graph::CGraphicsDevice*>(arg.GetValueAsPtr());

    IZ_BOOL result = InitObject(
        allocator,
        device,
        300.0f,
        "data/ModelImage.img",
        "data/tiny.msh",
        "data/tiny.skl",
        "data/tiny.anm",
        "data/SkinShader.shd");

    return result;
}

// ステートから抜ける（終了）.
IZ_BOOL CStateXFile::Leave()
{
    ReleaseObject();
    return IZ_TRUE;
}

IZ_FLOAT CStateXFile::GetElapesedTime()
{
    return 33.333f;
}
