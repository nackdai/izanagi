#if !defined(__IZANAGI_STD_STD_BASE_64_H__)
#define __IZANAGI_STD_STD_BASE_64_H__

#include <vector>
#include "izDefs.h"

namespace izanagi {
    class CBase64 {
    private:
        static const IZ_CHAR table[];

        enum {
            LARGE_ALPHABET_POS = 0,
            SMALL_ALPHABET_POS = 26,
            NUMBER_POS = 52,
            PLUS_POS = 62,
            SLUSH_POS = 63,
        };
        
    private:
        CBase64();
        ~CBase64();

        NO_COPIABLE(CBase64);

    private:
        static inline IZ_INT FindTable(IZ_CHAR code);

    public:
        static IZ_BOOL Encode(
            const std::vector<IZ_BYTE>& src,
            std::vector<IZ_CHAR>& dst);

        static IZ_BOOL Decode(
            const std::vector<IZ_CHAR>& src,
            std::vector<IZ_BYTE>& dst);
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_STD_BASE_64_H__)
