#if !defined(__IZANAGI_FONT_FONT_DEFS_H__)
#define __IZANAGI_FONT_FONT_DEFS_H__

#include "izDefs.h"

namespace izanagi {
    /**
    */
    enum E_FONT_CHAR_ENCODE {
        E_FONT_CHAR_ENCODE_SJIS = 0,    // SJIS
        E_FONT_CHAR_ENCODE_UTF16,       // UTF16
        E_FONT_CHAR_ENCODE_UTF8,        // UTF8
        E_FONT_CHAR_ENCODE_UNICODE,

        E_FONT_CHAR_ENCODE_NUM,
        E_FONT_CHAR_ENCODE_FORCE_INT32 = 0x7fffffff,
    };

namespace text
{
    struct SGlyphMetrics
    {
        IZ_UINT8 width;
        IZ_UINT8 height;

        IZ_UINT8 bearingX;
        IZ_UINT8 bearingY;
        IZ_UINT8 advance;

        IZ_UINT8 padding[3];
    };

    struct SGlyphImage
    {
        const IZ_UINT8* bmp;

        IZ_UINT16 pitch;
        IZ_UINT8 rows;
        IZ_UINT8 rowBytes;

        IZ_UINT8 leftOffset;
        IZ_UINT8 topOffset;

        IZ_UINT8 padding[2];
    };
}   // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_FONT_FONT_DEFS_H__)
