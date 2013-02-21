#include <ft2build.h>
#include FT_FREETYPE_H

#include "text/FontHostFT.h"
#include "izIo.h"
#include "izGraph.h"
#include "izStd.h"

namespace izanagi
{
namespace text
{
    FT_Library g_FTLibrary = IZ_NULL;

    class CFontHostFT::Impl : public CObject
    {
        static IZ_UINT m_ImplNum;

    public:
        static Impl* Create(
            IMemoryAllocator* allocator,
            IInputStream* in);

    public:
        Impl() {}
        ~Impl() {}

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_UINT GetFaceNum() const;

        void SetFace(IZ_UINT id);

        IZ_UINT GetGlyphID(IZ_UINT code);

        FT_GlyphSlot GetGlyphSlotByID(IZ_UINT id);

        void SetPixelSize(IZ_UINT height);

    private:
        IMemoryAllocator* m_Allocator;

        struct SFace
        {
            FT_Face ftFace;
        };

        IZ_UINT m_FaceNum;
        SFace* m_FaceList;

        IZ_UINT m_FaceIdx;
    };

    IZ_UINT CFontHostFT::Impl::m_ImplNum = 0;

    CFontHostFT::Impl* CFontHostFT::Impl::Create(
        IMemoryAllocator* allocator,
        IInputStream* in)
    {
        if (g_FTLibrary == IZ_NULL)
        {
            FT_Init_FreeType(&g_FTLibrary);
        }

        void* readBuf = ALLOC(allocator, (size_t)in->GetSize());

        IZ_BOOL result = IZ_INPUT_READ(in, readBuf, 0, (size_t)in->GetSize());
        VRETURN(result);

        FT_Open_Args arg;
        {
            arg.flags = FT_OPEN_MEMORY;
            arg.memory_base = reinterpret_cast<const FT_Byte*>(readBuf);
            arg.memory_size = (FT_Long)in->GetSize();
            arg.pathname = IZ_NULL;
            arg.stream = IZ_NULL;
            arg.driver = IZ_NULL;
            arg.num_params = 0;
            arg.params = IZ_NULL;
        }

        FT_Face face;

        FT_Error err = FT_Open_Face(
            g_FTLibrary,
            &arg,
            0,
            &face);
        VRETURN(result);

        size_t size = sizeof(Impl) + sizeof(SFace) * face->num_faces;
        IZ_UINT8* buf = (IZ_UINT8*)ALLOC(allocator, size);
        
        Impl* instance = new(buf) Impl;
        {
            buf += sizeof(Impl);

            instance->AddRef();

            instance->m_Allocator = allocator;
            instance->m_FaceNum = face->num_faces;
            instance->m_FaceList = reinterpret_cast<SFace*>(buf);

            instance->m_FaceList[0].ftFace = face;

            for (IZ_INT i = 1; i < face->num_faces; i++)
            {
                FT_Error err = FT_Open_Face(
                    g_FTLibrary,
                    &arg,
                    i,
                    &instance->m_FaceList[i].ftFace);
                VRETURN(result);
            }
        }

        m_ImplNum++;

        FREE(allocator, readBuf);

        return instance;
    }

    IZ_UINT CFontHostFT::Impl::GetFaceNum() const
    {
        return m_FaceNum;
    }

    void CFontHostFT::Impl::SetFace(IZ_UINT id)
    {
        IZ_ASSERT(id < m_FaceNum);
        m_FaceIdx = id;
    }

    IZ_UINT CFontHostFT::Impl::GetGlyphID(IZ_UINT code)
    {
        FT_UInt idx = FT_Get_Char_Index(m_FaceList[m_FaceIdx].ftFace, (FT_ULong)code);
        IZ_ASSERT(idx > 0);
        return idx;
    }

    FT_GlyphSlot CFontHostFT::Impl::GetGlyphSlotByID(IZ_UINT id)
    {
        FT_Error err = FT_Load_Glyph(
            m_FaceList[m_FaceIdx].ftFace,
            id,
            FT_LOAD_DEFAULT);
        IZ_ASSERT(err == 0);

        return m_FaceList[m_FaceIdx].ftFace->glyph;
    }

    void CFontHostFT::Impl::SetPixelSize(IZ_UINT height)
    {
        FT_Error err = FT_Set_Pixel_Sizes(
            m_FaceList[m_FaceIdx].ftFace,
            0,
            height);

        IZ_ASSERT(err == 0);
    }

//////////////////////////////////////////////////

    CFontHostFT* CFontHostFT::CreateFontHostFT(
        IMemoryAllocator* allocator,
        IInputStream* in)
    {
        IZ_UINT8* buf = (IZ_UINT8*)ALLOC(allocator, sizeof(CFontHostFT));

        CFontHostFT* instance = new(buf) CFontHostFT;
        {
            instance->AddRef();

            instance->m_Allocator = allocator;
            instance->m_Impl = Impl::Create(allocator, in);
        }

        return instance;
    }

    CFontHostFT::~CFontHostFT()
    {
        SAFE_RELEASE(m_Impl);
    }

    CFontHostFT::Impl* CFontHostFT::GetImpl()
    {
        return m_Impl;
    }

    void CFontHostFT::SetAllocatorForFreetype(IMemoryAllocator* allocator)
    {
    }

    IZ_UINT CFontHostFT::GetFaceNum() const
    {
        return m_Impl->GetFaceNum();
    }

    void CFontHostFT::SetFace(IZ_UINT id)
    {
        m_Impl->SetFace(id);
    }

    IZ_UINT CFontHostFT::GetGlyphID(IZ_UINT code)
    {
        return m_Impl->GetGlyphID(code);
    }

    IZ_BOOL CFontHostFT::GetGlyphMetricsByID(IZ_UINT id, SGlyphMetrics& metrics)
    {
        FT_GlyphSlot glyph = m_Impl->GetGlyphSlotByID(id);
        VRETURN(glyph != IZ_NULL);

        metrics.width = glyph->metrics.width >> 6;
        metrics.height = glyph->metrics.height >> 6;

        // TODO
        // horizontal or vertical
        metrics.bearingX = glyph->metrics.horiBearingX >> 6;
        metrics.bearingY = glyph->metrics.horiBearingY >> 6;
        metrics.advance = glyph->metrics.horiAdvance >> 6;

        return IZ_TRUE;
    }

    IZ_BOOL CFontHostFT::GetGlyphMetricsByCode(IZ_UINT code, SGlyphMetrics& metrics)
    {
        IZ_UINT id = m_Impl->GetGlyphID(code);
        VRETURN(id > 0);

        return GetGlyphMetricsByID(id, metrics);
    }

    void CFontHostFT::SetPixelSize(IZ_UINT height)
    {
        m_Impl->SetPixelSize(height);
    }

    IZ_BOOL CFontHostFT::GetImage(
        IZ_UINT code,
        SGlyphImage& image,
        SGlyphMetrics& metrics)
    {
        IZ_UINT id = GetGlyphID(code);
        VRETURN(id);

        FT_GlyphSlot glyph = GetImpl()->GetGlyphSlotByID(id);

        FT_Error err = FT_Render_Glyph(glyph, FT_RENDER_MODE_NORMAL);
        VRETURN(err == 0);

        {
            image.bmp = glyph->bitmap.buffer;
            image.pitch = glyph->bitmap.pitch;
            image.rows = glyph->bitmap.rows;
            image.rowBytes = glyph->bitmap.pitch;   // A8フォーマットなので
            image.leftOffset = glyph->bitmap_left;
            image.topOffset = glyph->bitmap_top;
        }

        {
            metrics.width = glyph->metrics.width >> 6;
            metrics.height = glyph->metrics.height >> 6;

            // TODO
            // horizontal or vertical
            metrics.bearingX = glyph->metrics.horiBearingX >> 6;
            metrics.bearingY = glyph->metrics.horiBearingY >> 6;
            metrics.advance = glyph->metrics.horiAdvance >> 6;
        }

        return IZ_TRUE;
    }
}    // namespace text
}    // namespace izanagi
