#if !defined(__URANUS_MATH_MATH_UTIL_H__)
#define __URANUS_MATH_MATH_UTIL_H__

#include "MathDefs.h"

namespace uranus {
	/**
	*/
	class CMath {
	public:
		/**
		* 与えられた整数値をアラインメントする
		*/
		static inline UN_INT Align(UN_INT nValue, UN_INT nAlign)
		{
#if 0
			return nAlign * ((nValue + nAlign - 1) / nAlign);
#else
			return (nAlign == 0 ? nValue : (nValue + nAlign - 1) & ~nAlign);
#endif
		}

		/**
		* Convets degree to radian.
		*/
		static inline UN_FLOAT Deg2Rad(UN_FLOAT fDeg)
		{
			return UN_MATH_PI * fDeg / 180.0f;
		}

		/**
		* Converts radian to degree.
		*/
		static inline UN_FLOAT Rad2Deg(UN_FLOAT fRad)
		{
			return fRad * 180.0f / UN_MATH_PI;
		}

		/**
		* UN_FLOATの絶対値を取得する
		*/
		static inline UN_FLOAT Absf(UN_FLOAT val)
		{
			return (UN_FLOAT)fabs(val);
		}

		/**
		* クランプする
		*/
		template <typename _T>
		static _T Clamp(_T val, _T min, _T max)
		{
			if (val > max) {
				val = max;
			}
			else if (val < min) {
				val = min;
			}
			return val;
		}

		/**
		* 線形補間
		*/
		template <typename _T>
		static _T Lerp(_T v0, _T v1, UN_FLOAT f)
		{
			return static_cast<_T>((1.0f - f) * v0 + f * v1);
		}

		/**
		* Returns whether sign is same.
		*/
		template <typename _T>
		static UN_BOOL IsSameSign(_T a, _T b)
		{
			return (a * b >= (_T)0);
		}

		/**
		* ２つの値が「だいたい」等しいかどうか比べる
		*/
		static inline UN_BOOL IsNearyEqual(UN_FLOAT a, UN_FLOAT b)
		{
			return ((a < b + UN_MATH_EPSILON) && (a > b - UN_MATH_EPSILON));
		}

		/**
		* 値が「だいたい」０になっているかどうか調べる
		*/
		static inline UN_BOOL IsNearyEqualZero(UN_FLOAT val)
		{
			return IsNearyEqual(val, 0.0f);
		}

		/**
		* sinθ
		*/
		static inline UN_FLOAT SinF(UN_FLOAT fTheta)
		{
			return sinf(fTheta);
		}

		/**
		* cosθ
		*/
		static inline UN_FLOAT CosF(UN_FLOAT fTheta)
		{
			return cosf(fTheta);
		}

		/**
		* sinθ, cosθ を求める。ただし、-2π < θ < 2π
		*/
		static inline void GetSinCosF(UN_FLOAT fTheta, UN_FLOAT* pSin, UN_FLOAT* pCos)
		{
			if (pSin != UN_NULL) {
				*pSin = SinF(fTheta);
			}
			if (pCos != UN_NULL) {
				*pCos = CosF(fTheta);
			}
		}

		/**
		* tanθ
		*/
		static inline UN_FLOAT TanF(UN_FLOAT fTheta)
		{
			return tanf(fTheta);
		}

		/**
		* arctan
		*/
		static inline UN_FLOAT ArcTanF(UN_FLOAT f)
		{
			return atanf(f);
		}

#if 0
		/**
		* 一番近い２の乗数の指数を計算する
		*/
		static inline UN_UINT ComputeExponentBy2(UN_FLOAT fVal)
		{
			// NOTE
			// 対数の底の変換
			// Log a X = (Log b X) / (Log b a)
			UN_FLOAT ret = log(fVal) / log(2.0f);
			return static_cast<UN_UINT>(ret);
		}
#endif

		/**
		* Count Leading Zero (= CLZ)
		* Returns the number of leading zero bits, 
		* starting at the most significant bit position.
		* If the argument is zero, the result is undefined.
		*/
		static inline UN_UINT CountLeadingZero(UN_UINT val)
		{
			if (val == 0) {
				return 32;
			}

			UN_UINT ret = 31;
			if (val & 0xffff0000) {
				ret -= 16;
				val >>= 16;
			}
			if (val & 0x0000ff00) {
				ret -= 8;
				val >>= 8;
			}
			if (val & 0x000000f0) {
				ret -= 4;
				val >>= 4;
			}
			if (val & 0x0000000c) {
				ret -= 2;
				val >>= 2;
			}
			if (val & 0x00000002) {
				ret -= 1;
				val >>= 2;
			}

			return ret;
		}

		/**
		* Returns the smallest power-of-2 that is >= the specified value.
		* If value is already a power of 2, then it is returned unchanged.
		* It is undefined if value is <= 0.
		*/
		static inline UN_UINT ComputeNextPow2(UN_UINT val)
		{
			UN_ASSERT(val > 0);
			return (1 << (32 - CountLeadingZero(val - 1)));
		}

		/**
		* Returns the log2 of the specified value, were that value to be rounded up to the next power of 2. 
		* It is undefined to pass 0.
		* Examples:
		*	ComputeNextLog2(1) -> 0
		*	ComputeNextLog2(2) -> 1
		*	ComputeNextLog2(3) -> 2
		*	ComputeNextLog2(4) -> 2
		*	ComputeNextLog2(5) -> 3
		*/
		static inline UN_UINT ComputeNextLog2(UN_UINT val)
		{
			UN_ASSERT(val > 0);
			return (32 - CountLeadingZero(val - 1));
		}
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_MATH_MATH_UTIL_H__)
