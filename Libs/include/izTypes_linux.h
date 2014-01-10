#if !defined(__IZANAGI_TYPES_LINUX_H__)
#define __IZANAGI_TYPES_LINUX_H__

/////////////////////////////////////////////////////////////
// 基本

#define IZ_TRUE     (1)
#define IZ_FALSE    (0)

#define IZ_NULL     NULL

/////////////////////////////////////////////////////////////
// 型

#include <stdint.h>

typedef uint8_t     IZ_BYTE;

typedef int16_t     IZ_SHORT;
typedef uint16_t    IZ_USHORT;

typedef int32_t     IZ_INT;
typedef uint32_t    IZ_UINT;

typedef int64_t     IZ_LONG;
typedef uint64_t    IZ_ULONG;

typedef int32_t     IZ_BOOL;

typedef float       IZ_FLOAT;
typedef double      IZ_DOUBLE;

typedef int8_t      IZ_INT8;
typedef uint8_t     IZ_UINT8;

typedef int16_t     IZ_INT16;
typedef uint16_t    IZ_UINT16;

typedef int32_t     IZ_INT32;
typedef uint32_t    IZ_UINT32;

typedef int64_t     IZ_INT64;
typedef uint64_t    IZ_UINT64;

typedef uint16_t    IZ_WORD;
typedef uint32_t    IZ_DWORD;

#endif  // #if !defined(__IZANAGI_TYPES_LINUX_H__)
