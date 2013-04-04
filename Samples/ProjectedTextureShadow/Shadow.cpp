#include "Shadow.h"

CShadow* CShadow::Create(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width,
    IZ_UINT height)
{
    IZ_BOOL result = IZ_FALSE;

    void* buf = ALLOC_ZERO(allocator, sizeof(CShadow));
    VRETURN_NULL(buf != IZ_NULL);

    CShadow* instance = new(buf) CShadow();
    {
        instance->m_Allocator = allocator;
        instance->AddRef();
        
        instance->m_RT = device->CreateRenderTarget(
                            width,
                            height,
                            fmtRT);
        result = (instance->m_RT != IZ_NULL);

        if (result) {
            instance->m_RT->SetAddress(
                izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP,
                izanagi::graph::E_GRAPH_TEX_ADDRESS_CLAMP);

            instance->m_DepthRT = device->CreateDepthStencilSurface(
                                    width, height,
                                    fmtDepth);
            result = (instance->m_DepthRT != IZ_NULL);
        }
    }

    if (!result) {
        SAFE_RELEASE(instance);
    }
    return instance;
}

CShadow::CShadow()
{
    m_Allocator = IZ_NULL;
    m_RT = IZ_NULL;
    m_DepthRT = IZ_NULL;
}

CShadow::~CShadow()
{
    SAFE_RELEASE(m_RT);
    SAFE_RELEASE(m_DepthRT);
}

IZ_BOOL CShadow::BeginShadowRender(
    izanagi::graph::CGraphicsDevice* device,
    const izanagi::math::SVector& lightPos,
    const izanagi::math::SVector& lightDir)
{
    IZ_BOOL ret = device->BeginScene(
                    &m_RT, 1,
                    m_DepthRT,
                    izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL);
    IZ_ASSERT(ret);

    if (ret) {
        IZ_FLOAT aspect = m_RT->GetHeight() / (IZ_FLOAT)m_RT->GetWidth();
        m_Light.Init(
            lightPos,
            izanagi::math::CVector(lightPos.x + lightDir.x, lightPos.y + lightDir.y, lightPos.z + lightDir.z),
            izanagi::math::CVector(0.0f, 1.0f, 0.0f),
            0.1f, 100.0f,
            IZ_MATH_PI / 2.5f,
            aspect);

        m_Light.Update();
    }

    return ret;
}

void CShadow::EndShadowRender(izanagi::graph::CGraphicsDevice* device)
{
    device->EndScene(izanagi::graph::E_GRAPH_END_SCENE_FLAG_RT_0 | izanagi::graph::E_GRAPH_END_SCENE_FLAG_DEPTH_STENCIL);
}

const izanagi::math::SMatrix& CShadow::GetLightViewProjMtx() const
{
    return m_Light.GetParam().mtxW2C;
}

const izanagi::math::SMatrix& CShadow::GetShadowTexMtx() const
{
    static izanagi::math::SMatrix mtx = {
        0.5f,  0.0f, 0.0f, 0.0f,
        0.0f, -0.5f, 0.0f, 0.0f,
        0.0f,  0.0f, 1.0f, 0.0f,
        0.5f,  0.5f, 0.0f, 1.0f,
    };

    return mtx;
}
