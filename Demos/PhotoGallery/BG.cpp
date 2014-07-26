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
    m_EnvBox = IZ_NULL;
}

BG::~BG()
{
    SAFE_RELEASE(m_EnvBox);
}

IZ_BOOL BG::Init(
    izanagi::IMemoryAllocator* allocator, 
    izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    m_EnvBox = izanagi::sample::CSampleEnvBox::CreateSampleEnvBox(
            allocator,
            device);
    VRETURN(m_EnvBox != IZ_NULL);

    return IZ_TRUE;
}

void BG::Render(izanagi::graph::CGraphicsDevice* device)
{
    IZ_ASSERT(device != IZ_NULL);

    IZ_ASSERT(m_EnvBox != IZ_NULL);
    m_EnvBox->Render(device);
}

void BG::SetShaderParam(
    izanagi::shader::CShaderBasic* shader,
    const izanagi::CCamera& camera)
{
    izanagi::math::SMatrix44::SetScale(m_L2W, 150.0f, 150.0f, 150.0f);

    // ƒJƒƒ‰‚ÌˆÊ’u‚É‚ ‚í‚¹‚ÄˆÚ“®‚·‚é
    izanagi::math::SMatrix44::Trans(
        m_L2W,
        m_L2W,
        camera.GetParam().pos);

    Utility::SetShaderParam(
        shader,
        "g_mL2W",
        (void*)&m_L2W,
        sizeof(m_L2W));
}
