#if !defined(__IZANAGI_TEXT_FONT_HOST_FT_H__)
#define __IZANAGI_TEXT_FONT_HOST_FT_H__

#include "izDefs.h"
#include "FNTFormat.h"
#include "FontDefs.h"

namespace izanagi
{
    class IInputStream;
    class IMemoryAllocator;

namespace text
{
    class IFontHost : public CObject
    {
    protected:
        IFontHost() {}
        virtual ~IFontHost() {}

    public:
        PURE_VIRTUAL(
            IZ_BOOL GetImage(
                IZ_UINT code,
                SGlyphImage& image,
                SGlyphMetrics& metrics));
    };

    /**
    * フォントレンダラ
    */
    class CFontHostFT : public IFontHost
    {
        friend class CFontRendererFT;

    public:
        // インスタンス作成
        static CFontHostFT* CreateFontHostFT(
            IMemoryAllocator* allocator,
            IInputStream* in);

        static void SetAllocatorForFreetype(IMemoryAllocator* allocator);

    private:
        CFontHostFT() {}
        ~CFontHostFT();

        NO_COPIABLE(CFontHostFT);
        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        IZ_UINT GetFaceNum() const;

        void SetFace(IZ_UINT id);

        IZ_UINT GetGlyphID(IZ_UINT code);

        IZ_BOOL GetGlyphMetricsByID(IZ_UINT id, SGlyphMetrics& metrics);

        IZ_BOOL GetGlyphMetricsByCode(IZ_UINT code, SGlyphMetrics& metrics);

        void SetPixelSize(IZ_UINT height);

        virtual IZ_BOOL GetImage(
            IZ_UINT code,
            SGlyphImage& image,
            SGlyphMetrics& metrics);

    private:
        IMemoryAllocator* m_Allocator;

        class Impl;
        Impl* m_Impl;

    private:
        Impl* GetImpl();
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_HOST_FT_H__)
