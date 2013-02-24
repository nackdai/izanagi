#if !defined(__IZANAGI_STD_STD_UTF_H__)
#define __IZANAGI_STD_STD_UTF_H__

#include "izDefs.h"

namespace izanagi {
    /**
    */
    class CStdUtf {
    public:
        // Converts UTF8 to Unicode
        static void ConvertUtf8ToUnicode(
            void* dst,
            IZ_UINT nDstSize,
            IZ_CHAR* src);

        static IZ_UINT ConvertUtf8ToUnicode(IZ_UINT code);

        // Converts UTF16 to Unicode
        static void ConvertUTF16ToUnicode(
            void* dst,
            IZ_UINT nDstSize,
            IZ_UINT16* src);

        // Get a character code as specified character encode.
        static void* GetOneCharCodeAsUTF8(void* src, IZ_UINT* ret);
        static void* GetOneCharCodeAsUnicode(void* src, IZ_UINT* ret);
        static void* GetOneCharCodeAsSJIS(void* src, IZ_UINT* ret);

        /**
        * Returns whether character code is ASCII.
        */
        static inline IZ_BOOL IsAscii(IZ_UINT ch)
        {
#if 0
            return (((0x21 <= ch) && (ch <= 0x7e))
                    || ((0xa1 <= ch) && (ch <= 0xdf)));
#else
            return (((0x21 <= ch) && (ch <= 0x7e))
                    || ((0xa1 <= ch) && (ch <= 0xdf))
                    || ((0x0a == ch) || (0x0d == ch)));
#endif
        }

        static inline IZ_BOOL IsLineFeed(IZ_UINT ch)
        {
            return ((0x0a == ch) || (0x0d == ch));
        }

        static inline IZ_BOOL IsSpace(IZ_UINT ch)
        {
            return (0x20 == ch);
        }
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_UTF_H__)
