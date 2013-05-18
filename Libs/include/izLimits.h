#if !defined(__IZANAGI_LIMITS_H__)
#define __IZANAGI_LIMITS_H__

#include <limits.h>

#if 1

#define IZ_INT8_MIN         SCHAR_MIN
#define IZ_INT16_MIN        SHRT_MIN
#define IZ_INT32_MIN        INT_MIN
#define IZ_INT64_MIN        LLONG_MIN

#define IZ_INT8_MAX         SCHAR_MAX
#define IZ_INT16_MAX        SHRT_MAX
#define IZ_INT32_MAX        INT_MAX
#define IZ_INT64_MAX        LLONG_MAX

#define IZ_UINT8_MAX        UCHAR_MAX
#define IZ_UINT16_MAX       USHRT_MAX
#define IZ_UINT32_MAX       UINT_MAX
#define IZ_UINT64_MAX       ULLONG_MAX

#define IZ_FLOAT_MAX        FLT_MAX
#define IZ_FLOAT_MIN        FLT_MIN
#define IZ_FLOAT_EPSILON    FLT_EPSILON

#define IZ_DOUBLE_MAX       DBL_MAX
#define IZ_DOUBLE_MIN       DBL_MIN
#define IZ_DOUBLE_EPSILON   DBL_EPSILON

#else

#define IZ_INT8_MIN         (-128)
#define IZ_INT16_MIN        (-32768)
#define IZ_INT32_MIN        (-2147483648)
#define IZ_INT64_MIN        (-9223372036854775807i64 - 1)

#define IZ_INT8_MAX         0x7f
#define IZ_INT16_MAX        0x7fff
#define IZ_INT32_MAX        0x7fffffff
#define IZ_INT64_MAX        0x7fffffffffffffffi64

#define IZ_UINT8_MAX        0xff
#define IZ_UINT16_MAX       0xffff
#define IZ_UINT32_MAX       0xffffffff
#define IZ_UINT64_MAX       0xffffffffffffffffui64

#define IZ_FLOAT_MAX        3.402823466e
#define IZ_FLOAT_MIN        1.175494351e-38F
#define IZ_FLOAT_EPSILON    1.192092896e-07F

#define IZ_DOUBLE_MAX       1.7976931348623158e
#define IZ_DOUBLE_MIN       2.2250738585072014e-308
#define IZ_DOUBLE_EPSILON   2.2204460492503131e-016

#endif

#endif  // #if !defined(__IZANAGI_LIMITS_H__)
