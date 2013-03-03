#if !defined(__IZANAGI_STD_STD_UTF_H__)
#define __IZANAGI_STD_STD_UTF_H__

#include "izDefs.h"

namespace izanagi
{
    class IMemoryAllocator;

    /**
    */
    class CStdUtf
    {
    public:
        struct SFuncIfLowMemory
        {
            IMemoryAllocator* allocator;
            void* (*funcIfLowMem)(IMemoryAllocator*, void*, IZ_UINT);

            void* operator()(void* ptr, IZ_UINT size);
        };

    public:
        // Converts UTF8 to Unicode
        static IZ_UINT ConvertUtf8ToUnicode(
            void* dst,
            IZ_UINT nDstSize,
            const void* src);

        static IZ_UINT ConvertUtf8ToUnicode(
            void* dst,
            IZ_UINT nDstSize,
            const void* src,
            SFuncIfLowMemory& func);

        static IZ_UINT ConvertUtf8ToUnicode(IZ_UINT code);

        // Converts UTF16 to Unicode
        static IZ_UINT ConvertUtf16ToUnicode(
            void* dst,
            IZ_UINT nDstSize,
            const void* src);

        static IZ_UINT ConvertUtf16ToUnicode(
            void* dst,
            IZ_UINT nDstSize,
            const void* src,
            SFuncIfLowMemory& func);

        // Get a character code as specified character encode.
        static void* GetOneCharCodeAsUTF8(const void* src, IZ_UINT* ret);
        static void* GetOneCharCodeAsUnicode(const void* src, IZ_UINT* ret);
        static void* GetOneCharCodeAsSJIS(const void* src, IZ_UINT* ret);

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
