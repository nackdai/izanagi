#include "izSampleKit.h"
#include "izFont.h"

#include "FontUTF8.h"

class CFontRendererApp : public izanagi::sample::CSampleApp
{
public:
    CFontRendererApp();
    virtual ~CFontRendererApp();

protected:
    // 初期化.
    virtual IZ_BOOL InitInternal(
        izanagi::IMemoryAllocator* allocator,
        izanagi::graph::CGraphicsDevice* device,
        izanagi::sample::CSampleCamera& camera);

    // 解放.
    virtual void ReleaseInternal();

    // 更新.
    virtual void UpdateInternal(izanagi::graph::CGraphicsDevice* device);

    // 描画.
    virtual void RenderInternal(izanagi::graph::CGraphicsDevice* device);

private:
    izanagi::text::CFontRenderer* m_Renderer;
    izanagi::text::CGlyphCache* m_Cache;
};

CFontRendererApp::CFontRendererApp()
{
    m_Renderer = IZ_NULL;
    m_Cache = IZ_NULL;
}

CFontRendererApp::~CFontRendererApp()
{
}

// 初期化.
IZ_BOOL CFontRendererApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    {
        izanagi::CFileInputStream in;

        VRETURN(in.Open("./data/mikachan..ttf"));

        m_Renderer = izanagi::text::CFontRenderer::CreateFontRenderer<izanagi::text::CFontRendererFT>(
            allocator,
            &in,
            32);
        VRETURN(m_Renderer != IZ_NULL);
    }

    {
        m_Cache = izanagi::text::CGlyphCacheImpl::CreateGlyphCache(
            allocator,
            device,
            m_Renderer->GetFontHost()->GetEncodeType(),
            32,
            m_Renderer->GetFontHost()->GetPixelSize(),
            IZ_FALSE);
        VRETURN(m_Cache != IZ_NULL);
    }

    return IZ_TRUE;
}

// 解放.
void CFontRendererApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Renderer);
    SAFE_RELEASE(m_Cache);
}

// 更新.
void CFontRendererApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::text::CUtf8String str(fontUTF8);

    m_Cache->BeginRegister();

    m_Cache->Register(&str, m_Renderer->GetFontHost());

    m_Cache->EndRegister();
}

// 描画.
void CFontRendererApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        // フォント描画

        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE_ALPHA);

        izanagi::text::CUtf8String str(fontUTF8);

        izanagi::CIntPoint ptr(100, 100);

        m_Renderer->Render(
            device,
            str,
            ptr,
            28,
            *m_Cache);

        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 4 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CFontRendererApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "FontFreeType",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
