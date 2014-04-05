#include "izSampleKit.h"

class CRender2DApp : public izanagi::sample::CSampleApp {
public:
    CRender2DApp();
    virtual ~CRender2DApp();

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
    izanagi::CImage* m_Img;
};

CRender2DApp::CRender2DApp()
{
    m_Img = IZ_NULL;
}

CRender2DApp::~CRender2DApp()
{
}

// 初期化.
IZ_BOOL CRender2DApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    izanagi::CFileInputStream in;
    VRETURN(in.Open("data/earth.img"));

    m_Img = izanagi::CImage::CreateImage(
                allocator,
                device,
                &in);

    return (m_Img != IZ_NULL);
}

// 解放.
void CRender2DApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Img);
}

// 更新.
void CRender2DApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    // Nothing is done...
}

// 描画.
void CRender2DApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    IZ_PRINTF("RenderInternal\n");

    if (device->Begin2D()) {
        // スプライト
        IZ_PRINTF("SetTexture\n");
        device->SetTexture(0, m_Img->GetTexture(0));
        IZ_PRINTF("Set2DRenderOp\n");
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);
        IZ_PRINTF("Draw2DSprite\n");
        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(300, 100, 256, 128));
        IZ_PRINTF("Draw2DRect\n");
        // 塗りつぶし矩形
        device->Draw2DRect(
            izanagi::CIntRect(100, 100, 100, 100),
            IZ_COLOR_RGBA(0, 0xff, 0, 0xff));
        IZ_PRINTF("Draw2DLine\n");
        // ライン
        device->Draw2DLine(
            izanagi::CIntPoint(100, 100),    // 始点
            izanagi::CIntPoint(200, 200),    // 終点
            IZ_COLOR_RGBA(0xff, 0, 0, 0xff));

        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CRender2DApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "Render2D",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
