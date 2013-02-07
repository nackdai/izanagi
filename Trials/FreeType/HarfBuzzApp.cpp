#include "HarfBuzzApp.h"
#include FT_GLYPH_H

#include <hb.h>
#include <hb-ft.h>

const char* text = "This is some english text";
const hb_direction_t text_direction = HB_DIRECTION_LTR;
const char* language = "en";
const hb_script_t script = HB_SCRIPT_LATIN;

hb_font_t* hb_ft_font;
hb_face_t* hb_ft_face;

CHarfBuzzApp::CHarfBuzzApp()
{
}

CHarfBuzzApp::~CHarfBuzzApp()
{
}

// 初期化.
IZ_BOOL CHarfBuzzApp::InitInternal(
    izanagi::IMemoryAllocator* allocator,
    izanagi::graph::CGraphicsDevice* device,
    izanagi::sample::CSampleCamera& camera)
{
    IZ_BOOL result = IZ_TRUE;

    FT_Error err = FT_Init_FreeType(&m_Library);

    err = FT_New_Face(
        m_Library,
        "font/DejaVuSerif.ttf",
        0,
        &m_Face);

    FT_Set_Pixel_Sizes(
        m_Face,
        0,
        50);

    hb_ft_font = hb_ft_font_create(m_Face, NULL);
    hb_ft_face = hb_ft_face_create(m_Face, NULL);
   
    /* Create a buffer for harfbuzz to use */
    hb_buffer_t *buf = hb_buffer_create();

    //alternatively you can use hb_buffer_set_unicode_funcs(buf, hb_glib_get_unicode_funcs());
    hb_buffer_set_unicode_funcs(buf, NULL);
    hb_buffer_set_direction(buf, text_direction); /* or LTR */
    hb_buffer_set_script(buf, script); /* see hb-unicode.h */
    hb_buffer_set_language(buf, hb_language_from_string(language, strlen(language)));

    /* Layout the text */
    hb_buffer_add_utf8(buf, text, strlen(text), 0, strlen(text));
    hb_shape(hb_ft_font, buf, NULL, 0);

    m_Tex = device->CreateTexture(
        1024,
        1024,
        1,
        izanagi::graph::E_GRAPH_PIXEL_FMT_A8,
        izanagi::graph::E_GRAPH_RSC_TYPE_STATIC);
   
    unsigned int glyph_count;
    hb_glyph_info_t* glyph_info = hb_buffer_get_glyph_infos(buf, &glyph_count);
    hb_glyph_position_t* glyph_pos  = hb_buffer_get_glyph_positions(buf, &glyph_count);

    unsigned int string_width_in_pixels = 0;
    for (int i=0; i < glyph_count; ++i)
    {
        string_width_in_pixels += glyph_pos[i].x_advance / 64;
    }

    int X = 20;
    int Y = 100;

    IZ_UINT8* data;
    IZ_UINT pitch = m_Tex->Lock(0, (void**)&data, IZ_FALSE);

    for (int i=0; i < glyph_count; ++i)
    {
        FT_Error err = FT_Load_Glyph(
            m_Face,
            glyph_info[i].codepoint,
            FT_LOAD_DEFAULT);

        err = FT_Render_Glyph(
            m_Face->glyph,
            FT_RENDER_MODE_NORMAL);

        int _X = X + (glyph_pos[i].x_offset);
        int _Y = Y - (glyph_pos[i].y_offset);

        for (IZ_UINT y = 0; y < m_Face->glyph->bitmap.rows; y++)
        {
            memcpy(
                data + (Y + y) * pitch /*+ m_Face->glyph->bitmap_left*/ - m_Face->glyph->bitmap_top * pitch,
                m_Face->glyph->bitmap.buffer + y * m_Face->glyph->bitmap.pitch,
                m_Face->glyph->bitmap.pitch);
        }

        X += glyph_pos[i].x_advance / 64;
        Y -= glyph_pos[i].y_advance / 64;

        data += glyph_pos[i].x_advance / 64;
    }

    m_Tex->Unlock(0);

    return IZ_TRUE;
}

// 解放.
void CHarfBuzzApp::ReleaseInternal()
{
    SAFE_RELEASE(m_Tex);
}

// 更新.
void CHarfBuzzApp::UpdateInternal(izanagi::graph::CGraphicsDevice* device)
{
}

// 描画.
void CHarfBuzzApp::RenderInternal(izanagi::graph::CGraphicsDevice* device)
{
    device->Begin2D();

    device->SetTexture(0, m_Tex);

    device->Draw2DSprite(
        izanagi::CFloatRect(0.0f, 0.0f, 1.0f, 1.0f),
        izanagi::CIntRect(100, 100, 356, 356));

    device->End2D();
}
