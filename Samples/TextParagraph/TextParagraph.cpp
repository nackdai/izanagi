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

    virtual IZ_BOOL OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key);

private:
    izanagi::text::IFontHost* m_FontHost;
    izanagi::text::CParagraphGroup* m_Paragraphs;

    IZ_UINT m_Width;
    IZ_UINT m_Height;
};

CTextParagraphApp::CTextParagraphApp()
{
    m_FontHost = IZ_NULL;
    m_Paragraphs = IZ_NULL;

    m_Width = 100;
    m_Height = 200;
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
        izanagi::text::CUtf8String str(textUTF8);

        m_Paragraphs = izanagi::text::CParagraphGroupImpl::CreateParagraphGroup(
            allocator,
            m_FontHost,
            str,
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
    m_Paragraphs->Layout(m_Width, m_Height);
}

// 描画.
void CTextParagraphApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        // フォント描画

        device->Set2DRenderOp(izanagi::graph::E_GRAPH_2D_RENDER_OP_MODULATE_ALPHA);

        izanagi::text::CUtf8String text(textUTF8);

        m_Paragraphs->Prepare(device);
        m_Paragraphs->Render(100, 100, device);

        izanagi::CDebugFont* debugFont = GetDebugFont();

        debugFont->Begin(m_Device, 0, 40);
        debugFont->DBPrint(
            m_Device,
            0xffffffff,
            "width[%d] height[%d]",
            m_Width, m_Height);
        debugFont->End();
        
        device->End2D();
    }
}

IZ_BOOL CTextParagraphApp::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_RIGHT)
    {
        if (m_Width < 1000)
        {
            m_Width += 10;
        }
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_LEFT)
    {
        if (m_Width > 50)
        {
            m_Width -= 10;
        }
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP)
    {
        if (m_Height < 500)
        {
            m_Height += 10;
        }
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN)
    {
        if (m_Height > 50)
        {
            m_Height -= 10;
        }
    }

    return IZ_TRUE;
}

static const IZ_UINT BUF_SIZE = 4 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

CTextParagraphApp app;

IzMain(SCREEN_WIDTH, SCREEN_HEIGHT)
{
    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "TextParagraph",
        IzGetScreenWidth(), IzGetScreenHeight(),
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
