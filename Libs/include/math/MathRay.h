#if !defined(__IZANAGI_MATH_RAY_H__)
#define __IZANAGI_MATH_RAY_H__

#include "MathVector4.h"

namespace izanagi
{
namespace math
{
    /**
     */
    struct SRay {
        SVector4 p;
        SVector4 v;
        IZ_FLOAT t;
    };

    /**
     */
    class CRay : public SRay {
    public:
        CRay();
        CRay(const SVector4& ptr, const SVector4& dir);
        CRay(const SVector4& ptr, const SVector4& dir, IZ_FLOAT _t);
        CRay(const SRay& rhs);

        ~CRay() {}

    public:
        /** 直線をコピー.
         */
        const CRay& operator=(const SRay& rhs);

        /** 直線を設定する.
         */
        void Set(
            const SVector4& ptr,
            const SVector4& dir,
            IZ_FLOAT _t);

        /** 平行かどうか.
         */
        IZ_BOOL IsParallel(const SRay& rhs) const;

        /** 無限遠方の直線にする.
         */
        void SetInifite();

        /** 係数を掛けた方向ベクトルを取得.
         */
        void GetMulAttnDir(SVector4& dir);

        /** 直線上にあるかどうか.
         */
        IZ_BOOL IsOnRay(const SVector4& ptr);

        /** 方向ベクトルを正規化.
         */
        void Normalize();
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI__MATH_RAY_H__)
