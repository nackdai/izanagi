#if !defined(__IZANAGI_MATH_MATH_UTIL_H__)
#define __IZANAGI_MATH_MATH_UTIL_H__

#include "MathDefs.h"

namespace izanagi
{
namespace math
{
    /**
    */
    class CMath {
    public:
        /**
        * 与えられた整数値をアラインメントする
        */
        static inline IZ_INT Align(IZ_UINT nValue, IZ_UINT nAlign)
        {
#if 0
            return nAlign * ((nValue + nAlign - 1) / nAlign);
#else
            // アライン基底は２の倍数であること
            IZ_ASSERT((nAlign & 0x01) == 0);
            IZ_UINT mask = nAlign - 1;
            return (nAlign == 0 ? nValue : (nValue + mask) & ~mask);
#endif
        }

        /**
        * Convets degree to radian.
        */
        static inline IZ_FLOAT Deg2Rad(IZ_FLOAT fDeg)
        {
            return IZ_MATH_PI * fDeg / 180.0f;
        }

        /**
        * Converts radian to degree.
        */
        static inline IZ_FLOAT Rad2Deg(IZ_FLOAT fRad)
        {
            return fRad * 180.0f / IZ_MATH_PI;
        }

        /**
        * IZ_FLOATの絶対値を取得する
        */
        static inline IZ_FLOAT Absf(IZ_FLOAT val)
        {
            return (IZ_FLOAT)fabs(val);
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
        static _T Lerp(_T v0, _T v1, IZ_FLOAT f)
        {
            return static_cast<_T>((1.0f - f) * v0 + f * v1);
        }

        /**
        * Returns whether sign is same.
        */
        template <typename _T>
        static IZ_BOOL IsSameSign(_T a, _T b)
        {
            return (a * b >= (_T)0);
        }

        /**
        * ２つの値が「だいたい」等しいかどうか比べる
        */
        static inline IZ_BOOL IsNearyEqual(IZ_FLOAT a, IZ_FLOAT b)
        {
            //return ((a < b + IZ_MATH_EPSILON) && (a > b - IZ_MATH_EPSILON));
            return ((b - 1e-5f <= a) && (a <= b + 1e-5f));
        }

        /**
        * 値が「だいたい」０になっているかどうか調べる
        */
        static inline IZ_BOOL IsNearyEqualZero(IZ_FLOAT val)
        {
            return IsNearyEqual(val, 0.0f);
        }

        /**
        * sinθ
        */
        static inline IZ_FLOAT SinF(IZ_FLOAT fTheta)
        {
            return sinf(fTheta);
        }

        /**
        * cosθ
        */
        static inline IZ_FLOAT CosF(IZ_FLOAT fTheta)
        {
            return cosf(fTheta);
        }

        /**
        * sinθ, cosθ を求める。ただし、-2π < θ < 2π
        */
        static inline void GetSinCosF(IZ_FLOAT fTheta, IZ_FLOAT& sin, IZ_FLOAT& cos)
        {
            sin = ::sinf(fTheta);
            cos = 1.0f - (sin * sin);
        }

        /**
        * tanθ
        */
        static inline IZ_FLOAT TanF(IZ_FLOAT fTheta)
        {
            return tanf(fTheta);
        }

        /**
        * arctan
        */
        static inline IZ_FLOAT ArcTanF(IZ_FLOAT f)
        {
            return atanf(f);
        }

#if 0
        /**
        * 一番近い２の乗数の指数を計算する
        */
        static inline IZ_UINT ComputeExponentBy2(IZ_FLOAT fVal)
        {
            // NOTE
            // 対数の底の変換
            // Log a X = (Log b X) / (Log b a)
            IZ_FLOAT ret = log(fVal) / log(2.0f);
            return static_cast<IZ_UINT>(ret);
        }
#endif
        /**
        * http://aggregate.org/MAGIC/#Population%20Count%20%28Ones%20Count%29
        */
        static inline IZ_UINT CountOnes(IZ_UINT x)
        {
            /**
            * 32-bit recursive reduction using SWAR...
            * but first step is mapping 2-bit values
            * into sum of 2 1-bit values in sneaky way
            */
            x -= ((x >> 1) & 0x55555555);
            x = (((x >> 2) & 0x33333333) + (x & 0x33333333));
            x = (((x >> 4) + x) & 0x0f0f0f0f);
            x += (x >> 8);
            x += (x >> 16);
            return(x & 0x0000003f);
        }

        /**
        * Count Leading Zero (= CLZ)
        * http://aggregate.org/MAGIC/#Leading%20Zero%20Count
        */
        static inline IZ_UINT CountLeadingZero(IZ_UINT x)
        {
            if (x == 0) {
                return 32;
            }

            x |= (x >> 1);
            x |= (x >> 2);
            x |= (x >> 4);
            x |= (x >> 8);
            x |= (x >> 16);

            return (32 - CountOnes(x));
        }

        /**
        * http://aggregate.org/MAGIC/#Next%20Largest%20Power%20of%202
        */
        static inline IZ_UINT ComputeNextPow2(IZ_INT n)
        {
            if (n <= 0) {
                return 0;
            }

            // NOTE
            // I think algortihm in http://aggregate.org/MAGIC/#Next%20Largest%20Power%20of%202 is not correct.
            // I found that 1 is subtracted from argument value in WWW.

            --n;
            n |= n >> 1;
            n |= n >> 2;
            n |= n >> 4;
            n |= n >> 8;
            n |= n >> 16;

            return n + 1;
        }

        /**
        * http://aggregate.org/MAGIC/#Log2%20of%20an%20Integer
        */
        static inline IZ_UINT ComputeFloorLog2(IZ_UINT x)
        {
            if (x <= 0) {
                return 0;
            }

            x |= (x >> 1);
            x |= (x >> 2);
            x |= (x >> 4);
            x |= (x >> 8);
            x |= (x >> 16);

            return (CountOnes(x) - 1);
        }

        /**
        */
        static inline IZ_UINT ComputeNextLog2(IZ_UINT n)
        {
            n = ComputeNextPow2(n);
            return ComputeFloorLog2(n);
        }

        /**
        * Return whether float value is valid.
        */
        static inline IZ_BOOL IsValidFloat(IZ_FLOAT f)
        {
            IZ_UINT nVal = *(IZ_UINT*)&f;
            IZ_UINT nExp = (nVal >> 23) & 0x0f;
            
            return (((1 <= nExp) && (nExp <= 255)) || (nVal == 0));
        }

        /**
        * Return whether float value is Not a Number(= NaN).
        */
        static inline IZ_BOOL IsFloatNaN(IZ_FLOAT f)
        {
            IZ_UINT nVal = *(IZ_UINT*)&f;
            IZ_UINT nExp = (nVal >> 23) & 0x0f;
            IZ_UINT nFraction = nVal & 0x7fffff;

            return ((nExp == 0xff) && (nFraction != 0));
        }

        /**
        * Return whether float value is Zero.
        */
        static inline IZ_BOOL IsFloatZero(IZ_FLOAT f)
        {
            IZ_UINT nVal = *(IZ_UINT*)&f;
            return (nVal == 0);
        }

        /**
        * Return whether float value is inifinite.
        */
        static inline IZ_BOOL IsFloatInfinite(IZ_FLOAT f)
        {
            IZ_UINT nVal = *(IZ_UINT*)&f;
            IZ_UINT nExp = (nVal >> 23) & 0x0f;
            IZ_UINT nFraction = nVal & 0x7fffff;

            return ((nExp == 0xff) && (nFraction == 0));
        }

        /**
        * Return whether float value is not normal.
        */
        static inline IZ_BOOL IsFloatNotNormal(IZ_FLOAT f)
        {
            IZ_UINT nVal = *(IZ_UINT*)&f;
            IZ_UINT nExp = (nVal >> 23) & 0x0f;
            IZ_UINT nFraction = nVal & 0x7fffff;

            return ((nExp == 0) && (nFraction != 0));
        }
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_MATH_UTIL_H__)
