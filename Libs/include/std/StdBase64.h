#if !defined(__IZANAGI_STD_STD_BASE_64_H__)
#define __IZANAGI_STD_STD_BASE_64_H__

#include <vector>
#include "izDefs.h"

namespace izanagi {
    /** Base64.
     */
    class CBase64 {
    private:
        // Character table.
        static const IZ_CHAR table[];

        // Character position in the table.
        enum {
            LARGE_ALPHABET_POS = 0,     // position of 'A'.
            SMALL_ALPHABET_POS = 26,    // position of 'a'.
            NUMBER_POS = 52,            // position of '0'.
            PLUS_POS = 62,              // position of '+'.
            SLUSH_POS = 63,             // position of '/'.
        };
        
    private:
        CBase64();
        ~CBase64();

        NO_COPIABLE(CBase64);

    private:
        // Convert from code to a character in the character table.
        static inline IZ_INT FindTable(IZ_CHAR code);

    public:
        /** Encode from data in std::vector<IZ_BYTE> to std::vector<IZ_CHAR>.
         */
        static IZ_BOOL Encode(
            const std::vector<IZ_BYTE>& src,
            std::vector<IZ_CHAR>& dst);

        /** Decode from data in std::vector<IZ_CHAR> to std::vector<IZ_BYTE>.
         */
        static IZ_BOOL Decode(
            const std::vector<IZ_CHAR>& src,
            std::vector<IZ_BYTE>& dst);

        /** Encode from data in memory buffer to memory buffer.
         */
        static IZ_UINT Encode(
            const IZ_BYTE* src,
            IZ_UINT srcLength,
            IZ_CHAR* dst);

        /** Decode from data in memory buffer to memory buffer.
         */
        static IZ_UINT Decode(
            const IZ_CHAR* src,
            IZ_UINT srcLength,
            IZ_BYTE* dst);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_BASE_64_H__)
