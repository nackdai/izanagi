#if !defined(__IZANAGI__MATH_DEFS_H__)
#define __IZANAGI__MATH_DEFS_H__

#include <math.h>
#include "izDefs.h"

#ifndef __IZANAGI_NO_USE_D3D__
	// For using direct3d math.
	#define __USE_D3D_MATH__
#endif	// #ifndef __IZANAGI_NO_USE_D3D__

#if defined(__USE_D3D_MATH__)
	#include "d3dx9math.h"
#endif	// #if defined(__USE_D3D_MATH__)

#define IZ_MATH_PI			(3.14159265358979323846f)	// pi
#define IZ_MATH_PI2			(IZ_MATH_PI * 2.0f)			// 2pi
#define IZ_MATH_PI1_2		(IZ_MATH_PI * 0.5f)			// pi/2

#define IZ_MATH_E			(2.7182818284590452354f)	// e
#define IZ_MATH_LOG2E		(1.4426950408889634074f)	// log 2e
#define IZ_MATH_LOG10E		(0.43429448190325182765f)	// log 10e
#define IZ_MATH_LN2			(0.69314718055994530942f)	// log e2
#define IZ_MATH_LN10		(2.30258509299404568402f)	// log e10

#define IZ_MATH_SQRT2		(1.41421356237309504880f)	// sqrt(2)
#define IZ_MATH_SQRT1_2		(0.70710678118654752440f)	// 1/sqrt(2)

#define IZ_MATH_EPSILON		IZ_FLOAT_EPSILON

#define IZ_DEG2RAD(d)	(IZ_MATH_PI * d / 180.0f)
#define IZ_RAD2DEG(r)	(r * 180.0f / IZ_MATH_PI)

#endif	// #if !defined(__IZANAGI__MATH_DEFS_H__)
