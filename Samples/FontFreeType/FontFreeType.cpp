#include "izSampleKit.h"
#include "izFont.h"

#include "FontUTF8.h"

class CFontFreeTypeApp : public izanagi::sample::CSampleApp
{
public:
    CFontFreeTypeApp();
    virtual ~CFontFreeTypeApp();

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
    izanagi::text::CGlyphCache* m_Cache;
};

CFontFreeTypeApp::CFontFreeTypeApp()
{
    m_FontHost = IZ_NULL;
    m_Cache = IZ_NULL;
}

CFontFreeTypeApp::~CFontFreeTypeApp()
{
}

// 初期化.
IZ_BOOL CFontFreeTypeApp::InitInternal(
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
        m_Cache = izanagi::text::CGlyphCacheImpl::CreateGlyphCache(
            allocator,
            device,
            m_FontHost->GetEncodeType(),
            32,
            m_FontHost->GetPixelSize(),
            IZ_FALSE);
        VRETURN(m_Cache != IZ_NULL);
    }

    return IZ_TRUE;
}

// 解放.
void CFontFreeTypeApp::ReleaseInternal()
{
    SAFE_RELEASE(m_FontHost);
    SAFE_RELEASE(m_Cache);
}

// 更新.
void CFontFreeTypeApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    izanagi::text::CUtf8String str(fontUTF8);

    m_Cache->BeginRegister();

    m_Cache->Register(&str, m_FontHost);

    m_Cache->EndRegister();
}

// 描画.
void CFontFreeTypeApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        // フォント描画

        izanagi::text::CUtf8String str(fontUTF8);

        m_Cache->Prepare(device);

        IZ_FLOAT width = device->GetTexture(0)->GetWidth();
        IZ_FLOAT height = device->GetTexture(0)->GetHeight();

        IZ_UINT posX = 100;
        IZ_UINT posY = 100;

        IZ_UINT code = 0;

        const IZ_UINT baseline = 16;

        str.BeginIter();

        while ((code = str.GetNextAsUnicode()) != 0)
        {
            izanagi::text::SGlyphCacheItem* item = m_Cache->FindCache(code);

            if (item != IZ_NULL)
            {
                IZ_FLOAT left = item->rect.left / width;
                IZ_FLOAT right = item->rect.right() / width;
                IZ_FLOAT top = item->rect.top / height;
                IZ_FLOAT bottom = item->rect.bottom() / height;

                IZ_UINT w = item->rect.width;
                IZ_UINT h = item->rect.height;

                IZ_UINT x = posX + item->leftOffset;
                IZ_UINT y = posY + baseline - item->metrics.bearingY;

                device->Draw2DSprite(
                    izanagi::CFloatRect(left, top, right, bottom),
                    izanagi::CIntRect(x, y, w, h));

                posX += item->metrics.advance;
            }
        }

        str.EndIter();

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
    CFontFreeTypeApp app;

    int ret = SampleMain(
        hInstance,
        &app,
        "FontFreeType",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
