#include "FBOManager.h"

FBOManager::~FBOManager()
{
    terminate();
}

void FBOManager::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width, IZ_UINT height)
{
    m_RT[0] = device->CreateRenderTarget(
        width,
        height,
        izanagi::graph::E_GRAPH_PIXEL_FMT::E_GRAPH_PIXEL_FMT_RGBA8);
    IZ_ASSERT(m_RT[0]);

    m_RT[1] = device->CreateRenderTarget(
        width,
        height,
        izanagi::graph::E_GRAPH_PIXEL_FMT::E_GRAPH_PIXEL_FMT_RGBA8);
    IZ_ASSERT(m_RT[1]);

    m_depthRT = device->CreateRenderTarget(
        width,
        height,
        izanagi::graph::E_GRAPH_PIXEL_FMT::E_GRAPH_PIXEL_FMT_D24S8);
    IZ_ASSERT(m_depthRT);
}

void FBOManager::begin(
    izanagi::graph::CGraphicsDevice* device,
    IZ_COLOR clrClear)
{
    device->BeginScene(
        m_RT, COUNTOF(m_RT),
        m_depthRT,
        izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
        clrClear, 1.0f, 0);
}

void FBOManager::end(izanagi::graph::CGraphicsDevice* device)
{
    device->EndScene();
}

void FBOManager::terminate()
{
    for (IZ_UINT i = 0; i < COUNTOF(m_RT); i++) {
        SAFE_RELEASE(m_RT[i]);
    }
    SAFE_RELEASE(m_depthRT);
}

void FBOManager::drawDebug(izanagi::graph::CGraphicsDevice* device)
{
    auto rt0 = m_RT[0];
    auto rt1 = m_RT[1];

    if (rt0) {
        device->SetTexture(0, rt0);
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

        device->Draw2DSprite(
            //izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CFloatRect(0.0f, 1.0f, 1.0f, 0.0f),
            izanagi::CIntRect(300, 100, 256, 128));
    }
    if (rt1) {
        device->SetTexture(0, rt1);
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

        device->Draw2DSprite(
            //izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CFloatRect(0.0f, 1.0f, 1.0f, 0.0f),
            izanagi::CIntRect(300, 300, 256, 128));
    }
}