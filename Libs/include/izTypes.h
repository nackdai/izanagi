#if !defined(__IZANAGI_TYPES_H__)
#define __IZANAGI_TYPES_H__

/////////////////////////////////////////////////////////////
// 基本

#define IZ_TRUE     (1)
#define IZ_FALSE    (0)

#define IZ_NULL     (0)

/////////////////////////////////////////////////////////////
// 型

#ifdef WINDOWS
typedef unsigned __int8     IZ_BYTE;

typedef __int16             IZ_SHORT;
typedef unsigned __int16    IZ_USHORT;

typedef __int32             IZ_INT;
typedef unsigned __int32    IZ_UINT;

typedef __int64             IZ_LONG;
typedef unsigned __int64    IZ_ULONG;

typedef __int32             IZ_BOOL;

typedef float               IZ_FLOAT;
typedef double              IZ_DOUBLE;

typedef __int8              IZ_INT8;
typedef unsigned __int8     IZ_UINT8;

typedef __int16             IZ_INT16;
typedef unsigned __int16    IZ_UINT16;

typedef __int32             IZ_INT32;
typedef unsigned __int32    IZ_UINT32;

typedef __int64             IZ_INT64;
typedef unsigned __int64    IZ_UINT64;

typedef unsigned __int16    IZ_WORD;
typedef unsigned __int32    IZ_DWORD;
#else
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
#endif

#endif  // #if !defined(__IZANAGI_DEFS_H__)
