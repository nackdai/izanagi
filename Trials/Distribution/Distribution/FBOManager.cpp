#include "FBOManager.h"

// NOTE
// Readback from FBO
// http://stackoverflow.com/questions/765434/glreadpixels-from-fbo-fails-with-multisampling

FBOManager::~FBOManager()
{
    terminate();
}

void FBOManager::init(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    IZ_UINT width, IZ_UINT height)
{
    m_allocator = allocator;

    m_screenWidth = width;
    m_screenHeight = height;

    for (IZ_UINT i = 0; i < COUNTOF(m_frames); i++) {
        m_frames[i].rt[0] = device->CreateRenderTarget(
            width,
            height,
            izanagi::graph::E_GRAPH_PIXEL_FMT::E_GRAPH_PIXEL_FMT_RGBA8);
        IZ_ASSERT(m_frames[i].rt[0]);

        m_frames[i].rt[1] = device->CreateRenderTarget(
            width,
            height,
            izanagi::graph::E_GRAPH_PIXEL_FMT::E_GRAPH_PIXEL_FMT_RGBA8);
        IZ_ASSERT(m_frames[i].rt[1]);
    }

    m_depthRT = device->CreateRenderTarget(
        width,
        height,
        izanagi::graph::E_GRAPH_PIXEL_FMT::E_GRAPH_PIXEL_FMT_D24S8);
    IZ_ASSERT(m_depthRT);

    m_pixels = ALLOC(m_allocator, width * height * 4);
}

void FBOManager::readback(izanagi::graph::CGraphicsDevice* device)
{
    while (m_RBTail != m_RBHead) {
        auto tmpTail = (m_RBTail + 1) % COUNTOF(m_frames);

#ifdef __IZ_OGL__
        auto texHandle = m_frames[tmpTail].rt[0]->GetTexHandle();

        CALL_GL_API(glBindTexture(GL_TEXTURE_2D, texHandle));

        CALL_GL_API(glGetTexImage(
            GL_TEXTURE_2D,
            0,
            GL_RGB,
            GL_UNSIGNED_BYTE,
            m_pixels));
#else
        auto d3d9device = (D3D_DEVICE*)device->GetPlatformInterface();

        // ロック可能なサーフェイスを作成.
        D3D_SURFACE* surface;
        auto hr = d3d9device->CreateOffscreenPlainSurface(
            m_screenWidth,
            m_screenHeight,
            D3DFMT_A8R8G8B8,
            D3DPOOL_SYSTEMMEM,
            &surface,
            NULL);
        IZ_ASSERT(SUCCEEDED(hr));

        auto d3d9surface = (D3D_SURFACE*)m_frames[tmpTail].rt[0]->GetPlatformInterface();
        
        d3d9device->GetRenderTargetData(
            d3d9surface,
            surface);

        surface->Release();
#endif

        m_RBTail = tmpTail;
    }
}

void FBOManager::begin(
    izanagi::graph::CGraphicsDevice* device,
    IZ_COLOR clrClear)
{
    auto tmpHead = (m_RBHead + 1) % COUNTOF(m_frames);

    if (tmpHead == m_RBTail) {
        IZ_PRINTF("Too fast\n");
    }
    else {
        device->BeginScene(
            m_frames[tmpHead].rt,
            COUNTOF(m_frames[tmpHead].rt),
            m_depthRT,
            izanagi::graph::E_GRAPH_CLEAR_FLAG_ALL,
            clrClear, 1.0f, 0);

        m_RBHead = tmpHead;
    }
}

void FBOManager::end(izanagi::graph::CGraphicsDevice* device)
{
    device->EndScene();
}

void FBOManager::terminate()
{
    for (IZ_UINT i = 0; i < COUNTOF(m_frames); i++) {
        for (IZ_UINT n = 0; n < COUNTOF(m_frames[i].rt); n++) {
            SAFE_RELEASE(m_frames[i].rt[n]);
        }
    }
    SAFE_RELEASE(m_depthRT);

    FREE(m_allocator, m_pixels);
}

void FBOManager::drawDebug(izanagi::graph::CGraphicsDevice* device)
{
#if 0
    auto rt0 = m_frames[m_curFrame].rt[0];
    auto rt1 = m_frames[m_curFrame].rt[1];

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
#endif
}