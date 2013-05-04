#if !defined(__IZANAGI_DEFS_H__)
#define __IZANAGI_DEFS_H__

#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include <float.h>
#include <limits.h>

#include "izTypes.h"

#ifndef WINDOWS
    #if defined(WIN32) || defined(WIN64)
        #define WINDOWS
    #endif
#endif  // #ifndef WINDOWS

#ifdef WINDOWS
    #ifdef _WINDLL
        #define IZ_API  __declspec(dllexport)
    #else
        #define IZ_API  __declspec(dllimport)
    #endif
#endif  // #ifdef WINDOWS

#ifndef IZ_API
    #define IZ_API
#endif

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

typedef char            IZ_CHAR;
typedef wchar_t         IZ_WCHAR;

typedef const char*     IZ_PCSTR;
typedef const wchar_t*  IZ_PCWSTR;

#ifdef _UNICODE
    typedef wchar_t         izChar;
    typedef const wchar_t*  izPcstr;
    #define IZ_VSPRINTF     vswprintf_s
    #define IZ_SPRINTF      swprintf_s
#else   // #ifdef _UNICODE
    typedef char            izChar;
    typedef const char*     izPcstr;
    #ifdef WIN32
        #define IZ_VSPRINTF     vsprintf_s
        #define IZ_SPRINTF      sprintf_s
        #define IZ_FPRINTF      fprintf_s
    #endif
#endif  // #ifdef _UNICODE

/////////////////////////////////////////////////////////////
// いろいろ便利系

#ifndef DEBUG_BREAK
    #ifdef WINDOWS
        //#define DEBUG_BREAK()     ::DebugBreak()
        //#define DEBUG_BREAK()       assert(false)
        #define DEBUG_BREAK()       __debugbreak()
    #else
        // TODO
    #endif
#endif  // #ifndef DEBUG_BREAK

#ifndef UNUSED_ALWAYS
    #define UNUSED_ALWAYS(v)    (v)
#endif  // #ifndef UNUSED

inline void _OutputDebugString(izPcstr pszFormat, ...)
{
    va_list argp;
    izChar pszBuf[256];
    va_start(argp, pszFormat);
    IZ_VSPRINTF(pszBuf, sizeof(pszBuf), pszFormat, argp);
    va_end(argp);
    ::OutputDebugString(pszBuf);
}

#ifndef IZ_PRINTF
    #ifdef WINDOWS
        #define IZ_PRINTF   _OutputDebugString
    #else
        #define IZ_PRINTF   printf
    #endif
#endif  // #ifndef IZ_PRINTF

#ifndef IZ_ASSERT
    #ifdef __IZ_DEBUG__
        #define IZ_ASSERT(b)\
            if (!(b)) {\
                IZ_PRINTF("assert : %s(%d)\n", __FILE__, __LINE__);\
                DEBUG_BREAK();\
            }
    #else   // #ifdef __IZ_DEBUG__
        #define IZ_ASSERT(b)
    #endif  // #ifdef __IZ_DEBUG__
#endif  // #ifndef IZ_ASSERT

#ifndef IZ_ASSERT_NULL
    #ifdef __IZ_DEBUG__
        #define IZ_ASSERT_NULL(p)   IZ_ASSERT((p) != IZ_NULL)
    #else   // #ifdef __IZ_DEBUG__
        #define IZ_ASSERT_NULL(p)
    #endif  // #ifdef __IZ_DEBUG__
#endif  // #ifndef IZ_ASSERT_NULL

#ifndef IZ_VERIFY
    #ifdef __IZ_DEBUG__
        #define IZ_VERIFY(b)\
            if (!(b)) {\
                IZ_PRINTF("assert : %s(%d)\n", __FILE__, __LINE__);\
                DEBUG_BREAK();\
            }
    #else   // #ifdef __IZ_DEBUG__
        #define IZ_VERIFY(b)    (b)
    #endif  // #ifdef __IZ_DEBUG__
#endif  // #ifndef IZ_VERIFY

#ifndef IZ_EXPECT
    #ifdef __IZ_DEBUG__
        #define IZ_EXPECT(b)\
            if (!(b)) {\
                IZ_PRINTF("expect : %s(%d)\n", __FILE__, __LINE__);\
            }
    #else   // #ifdef __IZ_DEBUG__
        #define IZ_EXPECT(b)
    #endif  // #ifdef __IZ_DEBUG__
#endif  // #ifndef IZ_EXPECT

#ifndef IZ_C_ASSERT
    #ifdef __IZ_DEBUG__
        #define IZ_C_ASSERT(b)  typedef IZ_BYTE __xxxxx__[(b) ? 1 : -1];
    #else
        #define IZ_C_ASSERT(b)
    #endif
#endif  // #ifndef IZ_C_ASSERT

#ifndef IZ_MAX
    #define IZ_MAX(a, b)    ((a) >= (b) ? (a) : (b))
#endif  // #ifndef IZ_MAX

#ifndef IZ_MIN
    #define IZ_MIN(a, b)    ((a) <= (b) ? (a) : (b))
#endif  // #ifndef IZ_MIN

#ifndef CLAMP
    #define CLAMP(n, a, b)  IZ_MIN(IZ_MAX(n, a), b)
#endif  // #ifndef CLAMP

#ifndef IS_IN_BOUND
    #define IS_IN_BOUND(n, a, b)    (((a) <= (n)) && ((n) <= (b)))
#endif  // #ifndef IS_IN_BOUND

#ifndef SAFE_RELEASE
    #define SAFE_RELEASE(p)\
        if (p) {\
            (p)->Release();\
            p = IZ_NULL;\
        }
#endif  // #ifndef SAFE_RELEASE

#ifndef SAFE_REPLACE
    #define SAFE_REPLACE(dst, src)\
        {\
            if (dst) { (dst)->Release(); }\
            if (src) { (src)->AddRef(); }\
            dst = src;\
        }
#endif  // #ifndef SAFE_REPLACE

#ifndef SAFE_DELETE
    #define SAFE_DELETE(p)\
        if (p) {\
            delete p;\
            p = IZ_NULL;\
        }
#endif  // #ifndef SAFE_DELETE

#ifndef SAFE_DELETE_ARRAY
    #define SAFE_DELETE_ARRAY(p)\
        if (p) {\
            delete[] p;\
            p = IZ_NULL;\
        }
#endif  // #ifndef SAFE_DELETE_ARRAY

#ifndef VRETURN_VAL
    #define VRETURN_VAL(b, r)\
        if (!(b)) {\
            IZ_ASSERT(IZ_FALSE);\
            return r;\
        }
#endif  // #ifndef VRETURN_VAL

#ifndef VRETURN
    #define VRETURN(b)      VRETURN_VAL(b, IZ_FALSE)
#endif  // #ifndef VRETURN

#ifndef VRETURN_NULL
    #define VRETURN_NULL(b) VRETURN_VAL(b, IZ_NULL)
#endif  // #ifndef VRETURN_NULL

#ifndef VRETURN_ZERO
    #define VRETURN_ZERO(b) VRETURN_VAL(b, 0)
#endif  // #ifndef VRETURN_ZERO

#ifndef VRETURN_NONE
    #define VRETURN_NONE(b) VRETURN_VAL(b,)
#endif

#ifndef VGOTO
    #define VGOTO(b, label)\
        if (!(b)) {\
            IZ_ASSERT(IZ_FALSE);\
            goto label;\
        }
#endif  // #ifndef VGOTO

#ifndef COUNTOF
    #define COUNTOF(a)      (sizeof(a) / sizeof(a[0]))
#endif  // #ifndef COUNTOF

#ifndef OFFSETOF
    #define OFFSETOF(s,m)   (size_t)&(((s *)0)->m)
#endif  // #ifndef OFFSETOF

#ifndef PURE_VIRTUAL
    #define PURE_VIRTUAL(d) virtual d = 0
#endif  // #ifndef PURE_VIRTUAL

#ifndef NO_COPIABLE
    #define NO_COPIABLE(clazz)\
        private:\
            clazz(const clazz & rhs);\
            const clazz & operator=(const clazz & rhs)
#endif  // #ifndef NO_COPIABLE

#ifndef FILL_ZERO
    #define FILL_ZERO(p, size)  memset(p, 0, size)
#endif  // #ifndef SET_ZERO

#ifndef COUNT_BIT
    #define COUNT_BIT(n)    (sizeof(n) << 3)
#endif  // #ifndef COUNT_BIT

#ifndef CONST_CAST
    #define CONST_CAST(t1, t2, p) reinterpret_cast<t1>(const_cast<t2>(p))
#endif  // #ifndef CONST_CAST

/////////////////////////////////////////////////////////////
// カラー

#define IZ_COLOR    IZ_DWORD
#define IZ_COLOR_RGBA(r, g, b, a)\
    ((IZ_COLOR)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)))

#define IZ_COLOR_R(color)   (((color) >> 16) & 0xff)
#define IZ_COLOR_G(color)   (((color) >>  8) & 0xff)
#define IZ_COLOR_B(color)   ((color) & 0xff)
#define IZ_COLOR_A(color)   (((color) >> 24) & 0xff)

/////////////////////////////////////////////////////////////
// サイズチェック

IZ_C_ASSERT(sizeof(IZ_CHAR)  == 1)
IZ_C_ASSERT(sizeof(IZ_BYTE)  == 1)
IZ_C_ASSERT(sizeof(IZ_INT8)  == 1)
IZ_C_ASSERT(sizeof(IZ_UINT8) == 1)

IZ_C_ASSERT(sizeof(IZ_SHORT)  == 2)
IZ_C_ASSERT(sizeof(IZ_USHORT) == 2)
IZ_C_ASSERT(sizeof(IZ_INT16)  == 2)
IZ_C_ASSERT(sizeof(IZ_UINT16) == 2)

IZ_C_ASSERT(sizeof(IZ_INT)    == 4)
IZ_C_ASSERT(sizeof(IZ_UINT)   == 4)
IZ_C_ASSERT(sizeof(IZ_INT32)  == 4)
IZ_C_ASSERT(sizeof(IZ_UINT32) == 4)
IZ_C_ASSERT(sizeof(IZ_BOOL)   == 4)
IZ_C_ASSERT(sizeof(IZ_DWORD)  == 4)

IZ_C_ASSERT(sizeof(IZ_FLOAT)  == 4)
IZ_C_ASSERT(sizeof(IZ_DOUBLE) == 8)

IZ_C_ASSERT(sizeof(IZ_LONG)  == 8)
IZ_C_ASSERT(sizeof(IZ_ULONG)  == 8)

IZ_C_ASSERT(sizeof(IZ_INT64)  == 8)
IZ_C_ASSERT(sizeof(IZ_UINT64) == 8)

IZ_C_ASSERT(sizeof(size_t) == sizeof(IZ_UINT))

/////////////////////////////////////////////////////////////

namespace izanagi {
    enum E_PLATFORM {
        E_PLATFORM_DX9 = 0,

        E_PLATFORM_NUM,
    };
}

#define IZ_COORD_LEFT_HAND

#endif  // #if !defined(__IZANAGI_DEFS_H__)
