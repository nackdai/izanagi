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
    izanagi::text::IFontHost* m_FontHost[2];

    izanagi::text::CGlyphCache* m_Cache[2];
};

struct SFontParameter
{
    const void* text;
    izanagi::text::E_FONT_CHAR_ENCODE charcode;
    const char* file;
} parameters[2] =
{
    {
        fontUTF8,
        izanagi::text::E_FONT_CHAR_ENCODE_UTF8,
        "./data/FontSample_Utf8.fnt",
    },
    {
        fontSJIS,
        izanagi::text::E_FONT_CHAR_ENCODE_SJIS,
        "./data/FontSample_Sjis.fnt"
    },
};

CFontBitmapApp::CFontBitmapApp()
{
    m_FontHost[0] = IZ_NULL;
    m_FontHost[1] = IZ_NULL;

    m_Cache[0] = IZ_NULL;
    m_Cache[1] = IZ_NULL;
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
    for (IZ_UINT i = 0; i < 2; i++)
    {
        izanagi::CFileInputStream in;

        VRETURN(in.Open(parameters[i].file));

        // FNT読み込み
        m_FontHost[i] = izanagi::text::CFontHostFNT::CreateFontHostFNT(
            allocator,
            &in);
        VRETURN(m_FontHost[i] != IZ_NULL);

        m_Cache[i] = izanagi::text::CGlyphCacheImpl::CreateGlyphCache(
            allocator,
            device,
            m_FontHost[i]->GetEncodeType(),
            32,
            m_FontHost[i]->GetPixelSize(),
            IZ_FALSE);
        VRETURN(m_Cache[i] != IZ_NULL);
    }

    return IZ_TRUE;
}

// 解放.
void CFontBitmapApp::ReleaseInternal()
{
    for (IZ_UINT i = 0; i < 2; i++)
    {
        SAFE_RELEASE(m_FontHost[i]);
        SAFE_RELEASE(m_Cache[i]);
    }
}

// 更新.
void CFontBitmapApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
    for (IZ_UINT i = 0; i < 2; i++)
    {
        izanagi::text::CUString str(
            parameters[i].charcode,
            parameters[i].text);

        m_Cache[i]->BeginRegister();

        m_Cache[i]->Register(&str, m_FontHost[i]);

        m_Cache[i]->EndRegister();
    }
}

// 描画.
void CFontBitmapApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    if (device->Begin2D()) {
        // フォント描画

        for (IZ_UINT i = 0; i < 2; i++)
        {
            izanagi::text::CUString str(
                parameters[i].charcode,
                parameters[i].text);

            m_Cache[i]->Prepare(device);

            IZ_FLOAT width = device->GetTexture(0)->GetWidth();
            IZ_FLOAT height = device->GetTexture(0)->GetHeight();

            IZ_UINT posX = 100;
            IZ_UINT posY = 100 * (i + 1);

            IZ_UINT code = 0;

            const IZ_UINT baseline = 16;

            str.BeginIter();

            while ((code = str.GetNext()) != 0)
            {
                izanagi::text::SGlyphCacheItem* item = m_Cache[i]->FindCache(code);

                if (item == IZ_NULL)
                {
                    if (code == 0x20)
                    {
                        posX += 10;
                    }
                }
                else
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
        }

        device->End2D();
    }
}

static const IZ_UINT BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT8 BUF[BUF_SIZE];

static const IZ_UINT GFX_BUF_SIZE = 1 * 1024 * 1024;
static IZ_UINT GFX_BUF[GFX_BUF_SIZE];

static const IZ_UINT SCREEN_WIDTH = 1280;
static const IZ_UINT SCREEN_HEIGHT = 720;

IzMain()
{
    CFontBitmapApp app;

    int ret = SampleMain(
        IzGetSystemDataForMainFunc(),
        &app,
        "FontBitmap",
        SCREEN_WIDTH, SCREEN_HEIGHT,
        BUF, BUF_SIZE,
        GFX_BUF, GFX_BUF_SIZE);

    return ret;
}
