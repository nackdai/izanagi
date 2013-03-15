#if !defined(__IZANAGI_TEXT_FONT_HOST_H__)
#define __IZANAGI_TEXT_FONT_HOST_H__

#include "izDefs.h"
#include "FontDefs.h"

namespace izanagi
{
namespace text
{
    class IFontHost : public CObject
    {
    protected:
        IFontHost() {}
        virtual ~IFontHost() {}

    public:
        PURE_VIRTUAL(E_FONT_CHAR_ENCODE GetEncodeType() const);

        PURE_VIRTUAL(IZ_UINT GetGlyphID(IZ_UINT code));

        PURE_VIRTUAL(IZ_BOOL GetGlyphMetricsByID(IZ_UINT id, SGlyphMetrics& metrics));

        PURE_VIRTUAL(IZ_BOOL GetGlyphMetricsByCode(IZ_UINT code, SGlyphMetrics& metrics));

        PURE_VIRTUAL(IZ_UINT GetPixelSize());

        PURE_VIRTUAL(
            IZ_BOOL GetImage(
                IZ_UINT code,
                SGlyphImage& image,
                SGlyphMetrics& metrics));

        PURE_VIRTUAL(
            IZ_BOOL GetImageByID(
                IZ_UINT id,
                SGlyphImage& image,
                SGlyphMetrics& metrics));

        PURE_VIRTUAL(IZ_UINT GetAscender());
        PURE_VIRTUAL(IZ_UINT GetDescender());
        PURE_VIRTUAL(IZ_UINT GetUnitsPerEM());
    };
}    // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_TEXT_FONT_HOST_H__)
