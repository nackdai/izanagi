#include "SimpleTextApp.h"
#include "izFont.h"

//const char* chars = "\xE3\x81\x82\xE3\x81\x84\xE3\x81\x86";
const char* chars = "\x82\x81\xE3\x84\x81\xE3\x86\x81\xE3";
//const char* chars = "ghijklmn";
izanagi::text::CFontHostFT* host;
izanagi::text::CGlyphCacheBase* cache;

CSimpleTextApp::CSimpleTextApp()
{
}

CSimpleTextApp::~CSimpleTextApp()
{
}

// 初期化.
IZ_BOOL CSimpleTextApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
   IZ_BOOL result = IZ_TRUE;

   izanagi::text::CFontHostFT::SetAllocatorForFreetype(allocator);

   izanagi::CFileInputStream in;
   in.Open("font/azuki.ttf");
   
   host = izanagi::text::CFontHostFT::CreateFontHostFT(
       allocator,
       &in,
       32);
   in.Close();

   cache = izanagi::text::CGlyphCache::CreateGlyphCache(
       allocator,
       device,
       100,
       32,
       IZ_TRUE);

   m_Tex = device->CreateTexture(
       256,
       256,
       1,
       izanagi::graph::E_GRAPH_PIXEL_FMT_A8,
       izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);

    return IZ_TRUE;
}

// 解放.
void CSimpleTextApp::ReleaseInternal()
{
    SAFE_RELEASE(host);
    SAFE_RELEASE(m_Tex);
}

// 更新.
void CSimpleTextApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
}

// 描画.
#if 0
void CSimpleTextApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    //izanagi::text::CUtf8String str(chars, strlen(chars));
    izanagi::text::CUnicodeString str(m_Allocator);
    str.Read(
        izanagi::E_FONT_CHAR_ENCODE_UTF8,
        chars,
        strlen(chars));

    str.BeginIter();

#if 0
    IZ_UINT8* data;
    IZ_UINT pitch = m_Tex->Lock(0, (void**)&data, IZ_FALSE);

    IZ_UINT8* cur = data;

    izanagi::text::CFontRendererFT renderer;

    IZ_UINT posX = 0;
    IZ_UINT posY = 0;

    IZ_UINT code = 0;
    while ((code = str.GetNext()) > 0)
    {
        izanagi::text::SGlyphImage image;

        code = izanagi::CStdUtf::ConvertUtf8ToUtf16(code);

        host->GetImage(
            code,
            image);

        for (IZ_UINT y = 0; y < image.rows; y++)
        {
            memcpy(
                //cur + (posY + y + 30) * pitch + image.dstLeftOffset - image.dstTopOffset * pitch,
                cur + posX + (posY + y + 32) * pitch + image.dstLeftOffset - image.dstTopOffset * pitch,
                image.bmp + y * image.pitch,
                image.pitch);
        }

        izanagi::text::SGlyphMetrics metrics;
        host->GetGlyphMetricsByCode(code, metrics);

        // NOTE
        // 16.16 point

        if ((posX + metrics.advance) >= m_Tex->GetWidth())
        {
            posX = 0;
            posY += metrics.height;
            cur = data;
        }
        else
        {
            posX += metrics.advance;
            //cur += metrics.advance;
        }
    }

    m_Tex->Unlock(0);
#else
    cache->BeginRegister();

    IZ_UINT code = 0;
#if 1
    while ((code = str.GetNext()) > 0)
    {
        //code = izanagi::CStdUtf::ConvertUtf8ToUnicode(code);

        izanagi::text::SGlyphImage image;
        izanagi::text::SGlyphMetrics metrics;
        host->GetImage(
            code,
            image,
            metrics);

        cache->Register(
            code,
            metrics,
            image);
    }
#else
    cache->Register(&str, host);
#endif

    cache->EndRegister();
#endif

    str.EndIter();

    str.BeginIter();

    device->Begin2D();

    //device->SetTexture(0, m_Tex);
    cache->Prepare(device);

    IZ_FLOAT width = device->GetTexture(0)->GetWidth();
    IZ_FLOAT height = device->GetTexture(0)->GetHeight();

    IZ_UINT posX = 100;
    IZ_UINT posY = 100;

    while ((code = str.GetNext()) > 0)
    {
        //code = izanagi::CStdUtf::ConvertUtf8ToUnicode(code);

        izanagi::text::SGlyphCacheItem* item = cache->FindCache(code);

        IZ_FLOAT left = item->rect.left / width;
        IZ_FLOAT right = item->rect.right / width;
        IZ_FLOAT top = item->rect.top / height;
        IZ_FLOAT bottom = item->rect.bottom / height;

        IZ_UINT w = item->rect.Width();
        IZ_UINT h = item->rect.Height();

        IZ_UINT x = posX + item->leftOffset;
        IZ_UINT y = posY + 32 - item->topOffset;

        device->Draw2DSprite(
            izanagi::CFloatRect(left, top, right, bottom),
            izanagi::CIntRect(x, y, x + w, y + h));

        posX += w;
    }

    device->Draw2DSprite(
            izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
            izanagi::CIntRect(100, 200, 612, 712));

    device->End2D();

    str.EndIter();
}
#else

#include "layout/LETypes.h"
#include "layout/LEFontInstance.h"

#include "ParagraphCache.h"

class CSampleFontInstance : public LEFontInstance
{
public:
    CSampleFontInstance() {}
    ~CSampleFontInstance() {}

    void Init(izanagi::text::IFontHost* host);

    virtual le_int32 getUnitsPerEM() const
    {
        return m_Host->GetUnitsPerEM();
    }

    virtual le_int32 getAscent() const
    {
        return m_Host->GetAscender();
    }

    virtual le_int32 getDescent() const
    {
        return m_Host->GetDescender();
    }

    virtual le_int32 getLeading() const
    {
        return 1;
    }

    virtual void getGlyphAdvance(LEGlyphID glyph, LEPoint &advance) const
    {
        izanagi::text::SGlyphMetrics metrics;
        host->GetGlyphMetricsByID(glyph, metrics);
        advance.fX = metrics.advance;
    }

    virtual le_bool getGlyphPoint(LEGlyphID glyph, le_int32 pointNumber, LEPoint &point) const
    {
        return FALSE;
    }

    virtual const void *getFontTable(LETag tableTag) const
    {
        return NULL;
    }

    virtual LEGlyphID mapCharToGlyph(LEUnicode32 ch) const
    {
        return host->GetGlyphID(ch);
    }

    float getXPixelsPerEm() const
    {
        return m_Host->GetUnitsPerEM();
    }

    float getYPixelsPerEm() const
    {
        return m_Host->GetUnitsPerEM();
    }

    float getScaleFactorX() const
    {
        return 1.0f;
    }

    float getScaleFactorY() const
    {
        return 1.0f;
    }

private:
    izanagi::text::IFontHost* m_Host;
};

void CSampleFontInstance::Init(izanagi::text::IFontHost* host)
{
    m_Host = host;
}

void CSimpleTextApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    //izanagi::text::CUtf8String str(chars, strlen(chars));
    izanagi::text::CUnicodeString str(m_Allocator);
    str.Read(
        izanagi::E_FONT_CHAR_ENCODE_UTF8,
        chars,
        strlen(chars));

    CSampleFontInstance fontInst;
    fontInst.Init(host);

    izanagi::text::CParagraphGroup* group = izanagi::text::CParagraphGroup::CreateParagraphGroup(
        m_Allocator,
        str,
        &fontInst);

    group->Layout(70, 100);

    group->Prepare(host, device);

    device->Begin2D();

    group->Render(100, 100, device);

    device->End2D();
}
#endif