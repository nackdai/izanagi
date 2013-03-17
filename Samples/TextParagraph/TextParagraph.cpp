#include "izSampleKit.h"
#include "izFont.h"

#include "TextUTF8.h"

class CTextParagraphApp : public izanagi::sample::CSampleApp
{
public:
    CTextParagraphApp();
    virtual ~CTextParagraphApp();

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
    izanagi::text::IFontHost* m_FontHost;
    izanagi::text::CParagraphGroup* m_Paragraphs;
};

CTextParagraphApp::CTextParagraphApp()
{
    m_FontHost = IZ_NULL;
    m_Paragraphs = IZ_NULL;
}

CTextParagraphApp::~CTextParagraphApp()
{
}

// 初期化.
IZ_BOOL CTextParagraphApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    {
        izanagi::CFileInputStream in;

        VRETURN(in.Open("./data/mikachan..ttf"));

        m_FontHost = izanagi::text::CFontHostFT::CreateFontHostFT(
            allocator,
            &in,
            32);
        VRETURN(m_FontHost != IZ_NULL);
    }

    {
        m_Paragraphs = izanagi::text::CDefaultParagraphGroup::CreateParagraphGroup(
            allocator,
            m_FontHost,
            izanagi::text::CUtf8String(textUTF8),
            IZ_NULL);
        VRETURN(m_Paragraphs != IZ_NULL);
    }

    return IZ_TRUE;
}

// 解放.
void CTextParagraphApp::ReleaseInternal()
{
    SAFE_RELEASE(m_FontHost);
    SAFE_RELEASE(m_Paragraphs);
}

// 更新.
void CTextParagraphApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    m_Paragraphs->Layout(100, 200);
}

// 描画.
void CTextParagraphApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        // フォント描画

        izanagi::text::CUtf8String text(textUTF8);

        m_Paragraphs->Prepare(device);
        m_Paragraphs->Render(100, 100, device);
        
        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 4 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    CTextParagraphApp app;

    int ret = SampleMain(
        hInstance,
        &app,
        "TextParagraph",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
