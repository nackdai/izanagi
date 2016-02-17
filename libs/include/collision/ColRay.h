#if !defined(__IZANAGI_COLLISION_RAY_H__)
#define __IZANAGI_COLLISION_RAY_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    /**
     */
    struct SRay {
        math::SVector4 p;
        math::SVector4 v;
        IZ_FLOAT t;
    };

    /**
     */
    class CRay : public SRay {
    public:
        CRay();
        CRay(const math::SVector4& ptr, const math::SVector4& dir);
        CRay(const math::SVector4& ptr, const math::SVector4& dir, IZ_FLOAT _t);
        CRay(const SRay& rhs);

        ~CRay() {}

    public:
        /** 直線をコピー.
         */
        const CRay& operator=(const SRay& rhs);

        /** 直線を設定する.
         */
        void Set(
            const math::SVector4& ptr,
            const math::SVector4& dir,
            IZ_FLOAT _t);

        /** 平行かどうか.
         */
        IZ_BOOL IsParallel(const SRay& rhs) const;

        /** 無限遠方の直線にする.
         */
        void SetInifite();

        /** 係数を掛けた方向ベクトルを取得.
         */
        void GetMulAttnDir(math::SVector4& dir);

        /** 直線上にあるかどうか.
         */
        IZ_BOOL IsOnRay(const math::SVector4& ptr);

        /** 方向ベクトルを正規化.
         */
        void Normalize();
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_RAY_H__)
