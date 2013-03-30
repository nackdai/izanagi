#include "FreeTypeApp.h"
#include FT_GLYPH_H

CFreeTypeApp::CFreeTypeApp()
{
}

CFreeTypeApp::~CFreeTypeApp()
{
}

// 初期化.
IZ_BOOL CFreeTypeApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
   IZ_BOOL result = IZ_TRUE;

   FT_Error err = FT_Init_FreeType(&m_Library);

#if 0
   err = FT_New_Face(
       m_Library,
       "font/azuki.ttf",
       0,
       &m_Face);
#else
   izanagi::CFileInputStream in;
   in.Open("font/azuki.ttf");

   void* readBuf = ALLOC(allocator, (size_t)in.GetSize());

        result = IZ_INPUT_READ(&in, readBuf, 0, (size_t)in.GetSize());
        VRETURN(result);

        FT_Open_Args arg;
        {
            arg.flags = FT_OPEN_MEMORY;
            arg.memory_base = reinterpret_cast<const FT_Byte*>(readBuf);
            arg.memory_size = (FT_Long)in.GetSize();
            arg.pathname = IZ_NULL;
            arg.stream = IZ_NULL;
            arg.driver = IZ_NULL;
            arg.num_params = 0;
            arg.params = IZ_NULL;
        }

        err = FT_Open_Face(
            m_Library,
            &arg,
            0,
            &m_Face);
        VRETURN(result);

        in.Close();
#endif

        FT_Face face = m_Face;

   err = FT_Set_Pixel_Sizes(
       face,
       0,
       32);

   m_Tex = device->CreateTexture(
       256,
       256,
       1,
       izanagi::graph::E_GRAPH_PIXEL_FMT_A8,
       izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);

    return IZ_TRUE;
}

// 解放.
void CFreeTypeApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Tex);
}

void RenderGlyphToBitmap(
    izanagi::graph::CGraphicsDevice* device,
    FT_Face& face,
    IZ_UINT8* data,
    IZ_UINT pitch)
{
    FT_Error err;

    IZ_UINT posX = 0;
    IZ_UINT posY = 0;

    IZ_UINT8* cur = data;

    for (char ch = 'a'; ch <= 'z'; ch++)
    {
        FT_UInt idx = FT_Get_Char_Index(face, (FT_ULong)ch);

        err = FT_Load_Glyph(
            face,
            idx,
            FT_LOAD_DEFAULT);

        err = FT_Render_Glyph(
            face->glyph,
            FT_RENDER_MODE_NORMAL);

        // NOTE
        // GDIで書く場合、ペンの位置をフォントの左上の位置
        // (このサイトで言うtmAscent-gmGlyphOrigine.yの位置 http://marupeke296.com/WINT_GetGlyphOutline.html)
        // に持っていかなくてはならない
        // FreeTypeの場合、何もしなくてもペンの位置がベースラインにある
        // ただ、このままだと複数文字をラスタライズした場合に、
        // 各々の文字でビットマップサイズが違うため、ベースラインがずれてしまう
        // それをあわせるために、bitmap_topプロパティで調整してやる必要がある

        for (IZ_UINT y = 0; y < face->glyph->bitmap.rows; y++)
        {
            memcpy(
                cur + (posY + 32 + y) * pitch + face->glyph->bitmap_left - face->glyph->bitmap_top * pitch,
                face->glyph->bitmap.buffer + y * face->glyph->bitmap.pitch,
                face->glyph->bitmap.pitch);
        }

        if (posX + (face->glyph->advance.x >> 6) >= 256)
        {
            posX = 0;
            posY += face->glyph->metrics.height >> 6;

            cur = data;
        }
        else
        {
            // NOTE
            // The advance vector is expressed in 1/64th of pixels,

            posX += face->glyph->advance.x >> 6;
            cur += face->glyph->advance.x >> 6;
        }
    }
}

void TransformGlyph(
    izanagi::graph::CGraphicsDevice* device,
    FT_Face& face,
    IZ_UINT8* data,
    IZ_UINT pitch)
{
    FT_Error err;

    IZ_UINT posX = 0;

    for (char ch = 'a'; ch <= 'b'; ch++)
    {
        FT_UInt idx = FT_Get_Char_Index(face, (FT_ULong)ch);

        err = FT_Load_Glyph(
            face,
            idx,
            FT_LOAD_DEFAULT);

        FT_Glyph  glyph;

        err = FT_Get_Glyph(face->glyph, &glyph);

        FT_Matrix  matrix;
        matrix.xx = 2 * 0x10000L;
        matrix.xy = 0;
        matrix.yx = 0;
        matrix.yy = 2 * 0x10000L;

        FT_Glyph_Transform(glyph, &matrix, 0);

        FT_Vector  origin;
        {
            origin.x = 0;
            origin.y = 0;
        }

        err = FT_Glyph_To_Bitmap(&glyph, FT_RENDER_MODE_NORMAL, &origin, 1);
        FT_BitmapGlyph bmp = (FT_BitmapGlyph)glyph;

        for (IZ_UINT y = 0; y < bmp->bitmap.rows; y++)
        {
            memcpy(
                data + (y + 30) * pitch + bmp->left - bmp->top * pitch,
                bmp->bitmap.buffer + y * bmp->bitmap.pitch,
                bmp->bitmap.pitch);
        }

        // NOTE
        // 16.16 point

        if ((posX + glyph->advance.x >> 16) >= 256)
        {
            posX = 0;
        }
        else
        {
            posX += glyph->advance.x >> 16;
            data += glyph->advance.x >> 16;
        }
    }
}

// 更新.
void CFreeTypeApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
   IZ_UINT8* data;
   IZ_UINT pitch = m_Tex->Lock(0, (void**)&data, IZ_FALSE);

   IZ_BOOL hasKerning = FT_HAS_KERNING(m_Face);

#if 1
   RenderGlyphToBitmap(
       device,
       m_Face,
       data, pitch);
#elif 1
   TransformGlyph(
       device,
       m_Face,
       data, pitch);
#endif

   m_Tex->Unlock(0);
}

// 描画.
void CFreeTypeApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    device->Begin2D();

    device->SetTexture(0, m_Tex);

    device->Draw2DSprite(
        izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
        izanagi::CIntRect(100, 100, 356, 356));

    device->End2D();
}
