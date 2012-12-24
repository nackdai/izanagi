#if !defined(__IZANAGI_MATH_RAND_H__)
#define __IZANAGI_MATH_RAND_H__

#include "izDefs.h"
#include "SFMT/SFMT.h"

namespace izanagi
{
namespace math
{
    /** 乱数生成
     * メルセンヌ・ツイスタを元にした乱数生成アルゴリズムを使用
     */
    class CMathRand {
    private:
        CMathRand();
        ~CMathRand();

        CMathRand(const CMathRand& rhs);
        const CMathRand& operator=(const CMathRand& rhs);

    public:
        /** 初期化
         */
        static void Init(IZ_UINT32 seed)
        {
            init_gen_rand(seed);
        }

        /** 乱数生成
         */
        static IZ_UINT32 GenRandUInt32()
        {
            IZ_UINT32 ret = gen_rand32();
            return ret;
        }

        /** 乱数生成
         * arrayのアドレスは１６バイトアラインされていること！！
         */
        static void GetRandUInt32Array(IZ_UINT32* array, IZ_UINT32 num)
        {
            fill_array32(array, num);
        }

        /** 乱数生成
         */
        static IZ_FLOAT GetRandFloat()
        {
            double ret = genrand_real1();
            return (IZ_FLOAT)ret;
        }

        /** 乱数生成
         */
        static void GetRandVector(SVector& v)
        {
            v.x = GetRandFloat();
            v.y = GetRandFloat();
            v.z = GetRandFloat();
            v.w = GetRandFloat();
        }

        /** 乱数生成
         */
        static void GetRandVectorXYZ(SVector& v)
        {
            v.x = GetRandFloat();
            v.y = GetRandFloat();
            v.z = GetRandFloat();
        }

        /** 乱数生成
         * ２値の間の乱数を取得
         */
        template <typename _T>
        static _T GetRandBetween(_T tMin, _T tMax)
        {
            if (tMin == tMax) {
                return tMin;
            }

            IZ_FLOAT tmp = GetRandFloat();
            _T ret = (_T)(tmp * (tMax - tMin) + tMin);
            return ret;
        }
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_RAND_H__)
