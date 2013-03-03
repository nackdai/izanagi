#if !defined(__IZANAGI_TEXT_FONT_HOST_FT_H__)
#define __IZANAGI_TEXT_FONT_HOST_FT_H__

#include "izDefs.h"
#include "FNTFormat.h"
#include "FontHost.h"

namespace izanagi
{
    class IInputStream;
    class IMemoryAllocator;

namespace text
{
    class CFontHostFT : public IFontHost
    {
        friend class CFontRendererFT;

    public:
        // インスタンス作成
        static CFontHostFT* CreateFontHostFT(
            IMemoryAllocator* allocator,
            IInputStream* in,
            IZ_UINT pixelSize);

        static void SetAllocatorForFreetype(IMemoryAllocator* allocator);

    private:
        CFontHostFT() {}
        ~CFontHostFT();

        NO_COPIABLE(CFontHostFT);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_UINT GetFaceNum() const;

        void SetFace(IZ_UINT id);

        virtual IZ_UINT GetGlyphID(IZ_UINT code);

        virtual IZ_BOOL GetGlyphMetricsByID(IZ_UINT id, SGlyphMetrics& metrics);

        virtual IZ_BOOL GetGlyphMetricsByCode(IZ_UINT code, SGlyphMetrics& metrics);

        virtual IZ_UINT GetPixelSize();

        virtual IZ_BOOL GetImage(
            IZ_UINT code,
            SGlyphImage& image,
            SGlyphMetrics& metrics);

        virtual IZ_BOOL GetImageByID(
            IZ_UINT id,
            SGlyphImage& image,
            SGlyphMetrics& metrics);

        virtual IZ_UINT GetAscender();
        virtual IZ_UINT GetDescender();
        virtual IZ_UINT GetUnitsPerEM();

    private:
        void SetPixelSize(IZ_UINT size);

    private:
        IMemoryAllocator* m_Allocator;

        class Impl;
        Impl* m_Impl;

    private:
        Impl* GetImpl();

        IZ_UINT m_PixelSize;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_HOST_FT_H__)
