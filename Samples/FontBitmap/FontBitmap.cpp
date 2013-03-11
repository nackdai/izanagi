#include "izSampleKit.h"
#include "izFont.h"

#include "FontUTF8.h"
#include "FontSJIS.h"

class CFontBitmapApp : public izanagi::sample::CSampleApp
{
public:
    CFontBitmapApp();
    virtual ~CFontBitmapApp();

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
    izanagi::text::IFontHost* m_FontUtf8;
    izanagi::text::IFontHost* m_FontSjis;

    izanagi::text::CGlyphCacheBase* m_Cache;
};

CFontBitmapApp::CFontBitmapApp()
{
    m_FontUtf8 = IZ_NULL;
    m_FontSjis = IZ_NULL;
    m_Cache = IZ_NULL;
}

CFontBitmapApp::~CFontBitmapApp()
{
}

// 初期化.
IZ_BOOL CFontBitmapApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    // UTF8
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("./data/FontSample_Utf8.fnt"));

        // FNT読み込み
        m_FontUtf8 = izanagi::text::CFontHostFNT::CreateFontHostFNT(
            allocator,
            &in);

        VRETURN(m_FontUtf8 != IZ_NULL);
    }

#if 0
    // SJIS
    {
        izanagi::CFileInputStream in;
        VRETURN(in.Open("./data/FontSample_Sjis.fnt"));

        // FNT読み込み
        m_FontSjis = izanagi::text::CFontHostFNT::CreateFontHostFNT(
            allocator,
            &in);
        VRETURN(m_FontSjis != IZ_NULL);
    }
#endif

    m_Cache = izanagi::text::CGlyphCache::CreateGlyphCache(
        allocator,
        device,
        izanagi::text::E_FONT_CHAR_ENCODE_UTF8,
        32,
        m_FontUtf8->GetPixelSize(),
        IZ_FALSE);

    return IZ_TRUE;
}

// 解放.
void CFontBitmapApp::ReleaseInternal()
{
    SAFE_RELEASE(m_FontUtf8);
    SAFE_RELEASE(m_FontSjis);
    SAFE_RELEASE(m_Cache);
}

// 更新.
void CFontBitmapApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    m_Cache->BeginRegister();

    izanagi::text::CUtf8String str(fontUTF8);
    m_Cache->Register(&str, m_FontUtf8);

    m_Cache->EndRegister();
}

// 描画.
void CFontBitmapApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    void* tmp = CONST_CAST(void*, char*, fontUTF8);

    izanagi::text::CUtf8String str(fontUTF8);

    m_Cache->Prepare(device);

    if (device->Begin2D()) {
        // フォント描画

        IZ_FLOAT width = device->GetTexture(0)->GetWidth();
        IZ_FLOAT height = device->GetTexture(0)->GetHeight();

        IZ_UINT posX = 100;
        IZ_UINT posY = 100;

        IZ_UINT code = 0;

        const IZ_UINT baseline = 16;

        str.BeginIter();

        while ((code = str.GetNext()) != 0)
        {
            izanagi::text::SGlyphCacheItem* item = m_Cache->FindCache(code);

            if (item == IZ_NULL)
            {
            }
            else
            {
                IZ_FLOAT left = item->rect.left / width;
                IZ_FLOAT right = item->rect.right / width;
                IZ_FLOAT top = item->rect.top / height;
                IZ_FLOAT bottom = item->rect.bottom / height;

                IZ_UINT w = item->rect.Width();
                IZ_UINT h = item->rect.Height();

                IZ_UINT x = posX + item->leftOffset;
                IZ_UINT y = posY + baseline - item->metrics.bearingY;

                device->Draw2DSprite(
                    izanagi::CFloatRect(left, top, right, bottom),
                    izanagi::CIntRect(x, y, x + w, y + h));

                posX += item->metrics.advance;
            }
        }

        str.EndIter();

        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
    CFontBitmapApp app;

    int ret = SampleMain(
        hInstance,
        &app,
        "FontBitmap",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
