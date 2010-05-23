#if !defined(__URANUS_DEFS_H__)
#define __URANUS_DEFS_H__

#include <windows.h>
#include <stdio.h>
#include <memory.h>
#include <assert.h>

#include <float.h>
#include <limits.h>

#ifndef WINDOWS
	#if defined(WIN32) || defined(WIN64)
		#define WINDOWS
	#endif
#endif	// #ifndef WINDOWS

/////////////////////////////////////////////////////////////
// 基本

#define UN_TRUE		(1)
#define UN_FALSE	(0)

#define UN_NULL		(0)

/////////////////////////////////////////////////////////////
// 型

typedef unsigned __int8		UN_BYTE;

typedef __int16				UN_SHORT;
typedef unsigned __int16	UN_USHORT;

typedef __int32				UN_INT;
typedef unsigned __int32	UN_UINT;

typedef __int64				UN_LONG;
typedef unsigned __int64	UN_ULONG;

typedef __int32				UN_BOOL;

typedef float				UN_FLOAT;
typedef double				UN_DOUBLE;

typedef __int8				UN_INT8;
typedef unsigned __int8		UN_UINT8;

typedef __int16				UN_INT16;
typedef unsigned __int16	UN_UINT16;

typedef __int32				UN_INT32;
typedef unsigned __int32	UN_UINT32;

typedef __int64				UN_INT64;
typedef unsigned __int64	UN_UINT64;

typedef unsigned __int16	UN_WORD;
typedef unsigned __int32	UN_DWORD;

#if 1

#define UN_INT8_MIN			SCHAR_MIN
#define UN_INT16_MIN		SHRT_MIN
#define UN_INT32_MIN		INT_MIN
#define UN_INT64_MIN		LLONG_MIN

#define UN_INT8_MAX			SCHAR_MAX
#define UN_INT16_MAX		SHRT_MAX
#define UN_INT32_MAX		INT_MAX
#define UN_INT64_MAX		LLONG_MAX

#define UN_UINT8_MAX		UCHAR_MAX
#define UN_UINT16_MAX		USHRT_MAX
#define UN_UINT32_MAX		UINT_MAX
#define UN_UINT64_MAX		ULLONG_MAX

#define UN_FLOAT_MAX		FLT_MAX
#define UN_FLOAT_MIN		FLT_MIN
#define UN_FLOAT_EPSILON	FLT_EPSILON

#define UN_DOUBLE_MAX		DBL_MAX
#define UN_DOUBLE_MIN		DBL_MIN
#define UN_DOUBLE_EPSILON	DBL_EPSILON

#else

#define UN_INT8_MIN			(-128)
#define UN_INT16_MIN		(-32768)
#define UN_INT32_MIN		(-2147483648)
#define UN_INT64_MIN		(-9223372036854775807i64 - 1)

#define UN_INT8_MAX			0x7f
#define UN_INT16_MAX		0x7fff
#define UN_INT32_MAX		0x7fffffff
#define UN_INT64_MAX		0x7fffffffffffffffi64

#define UN_UINT8_MAX		0xff
#define UN_UINT16_MAX		0xffff
#define UN_UINT32_MAX		0xffffffff
#define UN_UINT64_MAX		0xffffffffffffffffui64

#define UN_FLOAT_MAX		3.402823466e
#define UN_FLOAT_MIN		1.175494351e-38F
#define UN_FLOAT_EPSILON	1.192092896e-07F

#define UN_DOUBLE_MAX		1.7976931348623158e
#define UN_DOUBLE_MIN		2.2250738585072014e-308
#define UN_DOUBLE_EPSILON	2.2204460492503131e-016

#endif

typedef char			UN_CHAR;
typedef wchar_t			UN_WCHAR;

typedef const char*		UN_PCSTR;
typedef const wchar_t*	UN_PCWSTR;

/////////////////////////////////////////////////////////////
// いろいろ便利系

#ifndef UNUSED_ALWAYS
#define UNUSED_ALWAYS(v)	(v)
#endif	// #ifndef UNUSED

inline void _OutputDebugString(UN_PCSTR pszFormat, ...)
{
#ifdef __UN_DEBUG__
    va_list	argp;
    UN_CHAR pszBuf[256];
    va_start(argp, pszFormat);
    vsprintf_s(pszBuf, sizeof(pszBuf), pszFormat, argp);
    va_end(argp);
	::OutputDebugString(pszBuf);
#else	// #ifdef __UN_DEBUG__
	UNUSED_ALWAYS(pszFormat);
#endif	// #ifdef __UN_DEBUG__
}

#ifndef UN_PRINTF
#define UN_PRINTF	_OutputDebugString
#endif	// #ifndef UN_PRINTF

#ifndef UN_ASSERT
	#ifdef __UN_DEBUG__
		#define UN_ASSERT(b)\
			if (!(b)) {\
				UN_PRINTF("assert : %s(%d)\n", __FILE__, __LINE__);\
				::DebugBreak();\
			}
	#else	// #ifdef __UN_DEBUG__
		#define UN_ASSERT(b)
	#endif	// #ifdef __UN_DEBUG__
#endif	// #ifndef UN_ASSERT

#ifndef UN_C_ASSERT
	#ifdef __UN_DEBUG__
		#define UN_C_ASSERT(b)	typedef UN_BYTE __xxxxx__[(b) ? 1 : -1];
	#else
		#define UN_C_ASSERT(b)
	#endif
#endif	// #ifndef UN_C_ASSERT

#ifndef MAX
	#define MAX(a, b)	((a) >= (b) ? (a) : (b))
#endif	// #ifndef MAX

#ifndef MIN
	#define MIN(a, b)	((a) <= (b) ? (a) : (b))
#endif	// #ifndef MIN

#ifndef CLAMP
	#define CLAMP(n, a, b)	MIN(MAX(n, a), b)
#endif	// #ifndef CLAMP

#ifndef IS_IN_BOUND
	#define IS_IN_BOUND(n, a, b)	(((a) <= (n)) && ((n) <= (b)))
#endif	// #ifndef IS_IN_BOUND

#ifndef SAFE_RELEASE
	#define SAFE_RELEASE(p)\
		if (p) {\
			(p)->Release();\
			p = UN_NULL;\
		}
#endif	// #ifndef SAFE_RELEASE

#ifndef SAFE_REPLACE
	#define SAFE_REPLACE(dst, src)\
		{\
			if (dst) { (dst)->Release(); }\
			if (src) { (src)->AddRef(); }\
			dst = src;\
		}
#endif	// #ifndef SAFE_REPLACE

#ifndef SAFE_DELETE
	#define SAFE_DELETE(p)\
		if (p) {\
			delete p;\
			p = UN_NULL;\
		}
#endif	// #ifndef SAFE_DELETE

#ifndef VRETURN_VAL
	#define VRETURN_VAL(b, r)\
		if (!(b)) {\
			UN_ASSERT(UN_FALSE);\
			return (r);\
		}
#endif	// #ifndef VRETURN_VAL

#ifndef VRETURN
	#define VRETURN(b)		VRETURN_VAL(b, UN_FALSE);
#endif	// #ifndef VRETURN

#ifndef VRETURN_NULL
	#define VRETURN_NULL(b)	VRETURN_VAL(b, UN_NULL);
#endif	// #ifndef VRETURN_NULL

#ifndef VGOTO
	#define VGOTO(b, label)\
		if (!(b)) {\
			UN_ASSERT(UN_FALSE);\
			goto label;\
		}
#endif	// #ifndef VGOTO

#ifndef COUNTOF
	#define COUNTOF(a)		(sizeof(a) / sizeof(a[0]))
#endif	// #ifndef COUNTOF

#ifndef OFFSETOF
	#define OFFSETOF(s,m)	(size_t)&(((s *)0)->m)
#endif	// #ifndef OFFSETOF

#ifndef PURE_VIRTUAL
	#define PURE_VIRTUAL(d)	virtual d = 0
#endif	// #ifndef PURE

#ifndef NO_COPIABLE
	#define NO_COPIABLE(clazz)\
		private:\
			clazz(const clazz & rhs);\
			const clazz & operator=(const clazz & rhs)
#endif	// #ifndef NO_COPIABLE

#ifndef FILL_ZERO
	#define FILL_ZERO(p, size)	memset(p, 0, size)
#endif	// #ifndef SET_ZERO

/////////////////////////////////////////////////////////////
// カラー

#define UN_COLOR	UN_DWORD
#define UN_COLOR_RGBA(r, g, b, a)\
	((UN_COLOR)((((a) & 0xff) << 24) | (((r) & 0xff) << 16) | (((g) & 0xff) << 8) | ((b) & 0xff)))

/////////////////////////////////////////////////////////////
// サイズチェック

UN_C_ASSERT(sizeof(UN_CHAR)  == 1)
UN_C_ASSERT(sizeof(UN_BYTE)  == 1)
UN_C_ASSERT(sizeof(UN_INT8)  == 1)
UN_C_ASSERT(sizeof(UN_UINT8) == 1)

UN_C_ASSERT(sizeof(UN_SHORT)  == 2)
UN_C_ASSERT(sizeof(UN_USHORT) == 2)
UN_C_ASSERT(sizeof(UN_INT16)  == 2)
UN_C_ASSERT(sizeof(UN_UINT16) == 2)

UN_C_ASSERT(sizeof(UN_INT)    == 4)
UN_C_ASSERT(sizeof(UN_UINT)   == 4)
UN_C_ASSERT(sizeof(UN_INT32)  == 4)
UN_C_ASSERT(sizeof(UN_UINT32) == 4)
UN_C_ASSERT(sizeof(UN_BOOL)   == 4)
UN_C_ASSERT(sizeof(UN_DWORD)  == 4)

UN_C_ASSERT(sizeof(UN_FLOAT)  == 4)
UN_C_ASSERT(sizeof(UN_DOUBLE) == 8)

UN_C_ASSERT(sizeof(UN_LONG)  == 8)
UN_C_ASSERT(sizeof(UN_ULONG)  == 8)

UN_C_ASSERT(sizeof(UN_INT64)  == 8)
UN_C_ASSERT(sizeof(UN_UINT64) == 8)

UN_C_ASSERT(sizeof(size_t) == sizeof(UN_UINT))

#endif	// #if !defined(__URANUS_DEFS_H__)
