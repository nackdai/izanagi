#if !defined(__IZANAGI_COLLISION_RECTANGLE_H__)
#define __IZANAGI_COLLISION_RECTANGLE_H__

#include "izMath.h"
#include "collision/ColBoundingVolume.h"

namespace izanagi
{
namespace col
{
    struct SRay;
    struct SPlane;
    class Plane;

    /** 矩形クラス
     */
    class Rectangle : public BoundingVolume {
    public:
        Rectangle();
        
        /**
         * p------>v0
         * |
         * v1
         */
        Rectangle(
            const math::SVector4& point,
            const math::SVector4& v0,
            const math::SVector4& v1,
            IZ_BOOL isBothSides = IZ_FALSE);

        /**
         * 0------1
         * |      |
         * 2------3
         */
        Rectangle(
            const math::SVector4& pt0,
            const math::SVector4& pt1,
            const math::SVector4& pt2,
            const math::SVector4& pt3,
            IZ_BOOL isBothSides = IZ_FALSE);
        Rectangle(const Rectangle& rhs);

        virtual ~Rectangle() {}

    public:
        const Rectangle& operator=(const Rectangle& rhs);

        /** 矩形を設定.
         */
        void Set(
            const math::SVector4& point,
            const math::SVector4& v0,
            const math::SVector4& v1,
            IZ_BOOL isBothSides = IZ_FALSE);

        void Set(
            const math::SVector4& pt0,
            const math::SVector4& pt1,
            const math::SVector4& pt2,
            const math::SVector4& pt3,
            IZ_BOOL isBothSides = IZ_FALSE);

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

        void setIsBothSides(IZ_BOOL isBoth)
        {
            m_isBothSides = isBoth;
        }

        IZ_BOOL isBothSides() const
        {
            return m_isBothSides;
        }

    private:
        static IZ_FLOAT computePlane(
            const math::CMatrix44& mtxL2W,
            math::CVector4& pt0,
            math::CVector4& pt1,
            math::CVector4& pt2,
            math::CVector4& pt3,
            math::CVector4& nml);

        static IZ_FLOAT computePlane(
            const math::CVector4& pt0,
            const math::CVector4& pt1,
            const math::CVector4& pt2,
            const math::CVector4& pt3,
            math::CVector4& nml);

        inline void apply(math::CVector4* pt) const;

    private:
        math::CVector4 m_pt[4]; ///< 矩形を構成する4点.
        math::CVector4 m_nml;   ///< 法線.
        IZ_FLOAT m_d;           ///< 原点からの距離.

        IZ_BOOL m_isBothSides{ IZ_FALSE };  ///< 両面判定の可否.
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_RECTANGLE_H__)
