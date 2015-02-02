#include "izSampleKit.h"
#include "Render2D.h"

Callback g_Callback = IZ_NULL;

void SetCallback(Callback func)
{
    g_Callback = func;
}

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

    virtual void Present();

private:
    izanagi::CImage* m_Img;
    izanagi::graph::CGraphicsDevice* m_Dev;
};

CRender2DApp::CRender2DApp()
{
    m_Img = IZ_NULL;
    m_Dev = IZ_NULL;
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
#if 0
    izanagi::CFileInputStream in;
    VRETURN(in.Open("data/earth.img"));

    m_Img = izanagi::CImage::CreateImage(
                allocator,
                device,
                &in);

    return (m_Img != IZ_NULL);
#else
    char path[256];
    izanagi::sys::CSysUtil::GetExecuteFilePath(path, sizeof(path));

    printf("%s\n", path);

    return IZ_TRUE;
#endif
}

// 解放.
void CRender2DApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Img);
    SAFE_RELEASE(m_Dev);
}

// 更新.
void CRender2DApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (m_Dev == IZ_NULL) {
        SAFE_REPLACE(m_Dev, device);
    }
}

// 描画.
void CRender2DApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        // スプライト
#if 0
        device->SetTexture(0, m_Img->GetTexture(0));
        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE);

        device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(300, 100, 256, 128));
#endif
        
        // 塗りつぶし矩形
        device->Draw2DRect(
            izanagi::CIntRect(100, 100, 100, 100),
            IZ_COLOR_RGBA(0, 0xff, 0, 0xff));
        
        // ライン
        device->Draw2DLine(
            izanagi::CIntPoint(100, 100),    // 始点
            izanagi::CIntPoint(200, 200),    // 終点
            IZ_COLOR_RGBA(0xff, 0, 0, 0xff));

        device->End2D();
    }
}

IZ_UINT count = 0;
char name[512];

char root[256];

void SetRootPath(const char* path)
{
    sprintf(root, "%s\0", path);
    printf("%s\n", root);
}

void CRender2DApp::Present()
{
    CSampleApp::Present();

    if (m_Dev != IZ_NULL) {
        sprintf(name, "%s\\screenshot_%d.png\0", root, count);
        count++;
        m_Dev->SaveScreenShot(name, izanagi::graph::E_GRAPH_IMAGE_FILE_FMT_PNG);

        if (g_Callback != IZ_NULL) {
            (*g_Callback)();
        }
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CRender2DApp app;

//IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
int Hoge()
{
    int ret = SampleMain(
        //IzGetSystemDataForMainFunc(),
        IZ_NULL,
        &app,
        "Render2D",
        //IzGetScreenWidth(), IzGetScreenHeight(),
        SCREEN_WIDTH, SCREEN_HEIGHT,
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}

int Loop()
{
    app.Idle();
    return 0;
}
