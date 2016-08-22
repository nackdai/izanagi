#if !defined(__IZANAGI_COLLISION_PLANE_H__)
#define __IZANAGI_COLLISION_PLANE_H__

#include "izMath.h"
#include "collision/ColBoundingVolume.h"

namespace izanagi
{
namespace col
{
    struct SRay;

    /**
     */
    class Plane : public BoundingVolume {
    public:
        Plane();
        Plane(const math::SVector4& normal, const math::SVector4& ptr);

        virtual ~Plane() {}

    private:
        Plane(IZ_FLOAT _a, IZ_FLOAT _b, IZ_FLOAT _c, IZ_FLOAT _d);

    public:
        /** 平面をコピー.
         */
        const Plane& operator=(const Plane& rhs);

        /** 平面を設定.
         */
        void Set(const math::SVector4& normal, const math::SVector4& ptr);

        /** 平面を設定.
         */
        void Set(const math::SVector4& normal, IZ_FLOAT _d);

        /** 面の正側（法線の向き側）に点があるかどうか.
         */
        IZ_BOOL IsPositive(const math::SVector4& ptr) const;

        /** 実効半径を計算.
         */
        virtual IZ_FLOAT computeRadius(const math::SVector4& normal) const override;

        /** 実効半径を計算.
         */
        virtual IZ_FLOAT computeRadius(
            const math::SMatrix44& mtxW2V,
            const math::SVector4& normal) const override;

        /** 中心座標を取得.
         */
        virtual const math::SVector4 getCenter() const override;

        /** Get if the ray is hit.
         */
        virtual IZ_BOOL isHit(const Ray& ray, HitResult& res) override;

    private:
        static IZ_FLOAT computePlane(
            const math::CMatrix44& mtxL2W,
            math::CVector4& pt,
            math::CVector4& nml);

    private:
        math::CVector4 m_nml;
        IZ_FLOAT m_d;

        math::CVector4 m_pt;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_PLANE_H__)
