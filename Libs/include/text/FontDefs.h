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
        IZ_INT width;
        IZ_INT height;

        IZ_INT bearingX;
        IZ_INT bearingY;
        IZ_INT advance;
    };

    struct SGlyphImage
    {
        IZ_UINT8* bmp;
        IZ_UINT8 pitch;
        IZ_UINT8 rows;
        IZ_UINT8 rowBytes;
        IZ_UINT8 leftOffset;
        IZ_UINT8 topOffset;
        IZ_UINT8 padding[3];
    };
}   // namespace text
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_FONT_FONT_DEFS_H__)
