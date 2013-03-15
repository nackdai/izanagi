#if !defined(__IZANAGI_TEXT_FONT_HOST_FNT_H__)
#define __IZANAGI_TEXT_FONT_HOST_FNT_H__

#include "izDefs.h"
#include "FNTFormat.h"
#include "FontHost.h"

namespace izanagi
{
    class IInputStream;
    class IMemoryAllocator;

namespace text
{
    class CFontHostFNT : public IFontHost
    {
    public:
        // インスタンス作成
        static CFontHostFNT* CreateFontHostFNT(
            IMemoryAllocator* allocator,
            IInputStream* in);

    private:
        CFontHostFNT();
        virtual ~CFontHostFNT();

        NO_COPIABLE(CFontHostFNT);
        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        IZ_BOOL Read(
            IInputStream* in,
            IZ_UINT8* buf);

        const S_FNT_MAP* GetFontMap(IZ_UINT code);

        const S_FNT_MAP* GetFontMapByID(IZ_UINT id);

    public:
        virtual E_FONT_CHAR_ENCODE GetEncodeType() const;

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
        IMemoryAllocator* m_Allocator;

        S_FNT_HEADER m_Header;

        // フォントマップ情報
        S_FNT_MAP* m_MapList;

        // フォントイメージ情報
        S_FNT_IMAGE* m_ImageList;
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_HOST_FNT_H__)
