#include "BG.h"
#include "Configure.h"
#include "Utility.h"

BG* BG::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device)
{
    void* buf = ALLOC(allocator, sizeof(BG));
    VRETURN_NULL(buf != IZ_NULL);

    IZ_BOOL result = IZ_TRUE;

    BG* instance = new (buf) BG;
    {
        instance->AddRef();
        instance->m_Allocator = allocator;
        result = instance->Init(allocator, device);
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }

    return instance;
}

BG::BG()
{
    m_BG = IZ_NULL;
}

BG::~BG()
{
    SAFE_RELEASE(m_BG);
}

IZ_BOOL BG::Init(
    izanagi::IMemoryAllocator* allocator, 
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    m_BG = izanagi::CDebugMeshRectangle::CreateDebugMeshRectangle(
        allocator,
        device,
        Configure::MeshFlags,
        Configure::BGColor,
        20, 20,
        400.0f,
        200.0f);
    VRETURN(m_BG != IZ_NULL);

    izanagi::math::SMatrix::GetRotByX(m_L2W, IZ_DEG2RAD(90.0f));
    izanagi::math::SMatrix::Trans(m_L2W, m_L2W, 0.0f, 50.0f, -200.0f);

    return IZ_TRUE;
}

void BG::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    IZ_ASSERT(m_BG != IZ_NULL);
    m_BG->Draw(device);
}

void BG::SetShaderParam(izanagi::shader::CShaderBasic* shader)
{
    Utility::SetShaderParam(
        shader,
        "g_mL2W",
        (void*)&m_L2W,
        sizeof(m_L2W));
}
