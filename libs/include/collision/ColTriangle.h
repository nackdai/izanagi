#if !defined(__IZANAGI_COLLISION_TRIANGLE_H__)
#define __IZANAGI_COLLISION_TRIANGLE_H__

#include "izMath.h"
#include "collision/ColBoundingVolume.h"

namespace izanagi
{
namespace col
{
    struct SRay;

    class Triangle : public BoundingVolume {
    public:
        Triangle();
        Triangle(
            const math::SVector4 point[3],
            IZ_BOOL isBothSides = IZ_FALSE);
        Triangle(
            const math::SVector4& pt0,
            const math::SVector4& pt1,
            const math::SVector4& pt2,
            IZ_BOOL isBothSides = IZ_FALSE);
        Triangle(const Triangle& rhs);

        virtual ~Triangle() {}

    public:
        const Triangle& operator=(const Triangle& rhs);

        void Set(
            const math::SVector4 point[3],
            IZ_BOOL isBothSides = IZ_FALSE);

        void Set(
            const math::SVector4& point0,
            const math::SVector4& point1,
            const math::SVector4& point2,
            IZ_BOOL isBothSides = IZ_FALSE);

        /** é¿å¯îºåaÇåvéZ.
         */
        virtual IZ_FLOAT computeRadius(const math::SVector4& normal) const override;

        /** é¿å¯îºåaÇåvéZ.
         */
        virtual IZ_FLOAT computeRadius(
            const math::SMatrix44& mtxW2V,
            const math::SVector4& normal) const override;

        /** íÜêSç¿ïWÇéÊìæ.
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
        IZ_BOOL isHit(
            const Ray& ray, 
            const math::CVector4* pt,
            const math::CVector4& nml,
            IZ_BOOL isFlipped,
            math::CVector4& retP, 
            IZ_FLOAT& retT);

        static IZ_FLOAT computePlane(
            const math::CVector4& pt0,
            const math::CVector4& pt1,
            const math::CVector4& pt2,
            math::CVector4& retNml);

    private:
        math::CVector4 m_pt[3];
        math::CVector4 m_nml;
        IZ_FLOAT m_d;

        IZ_BOOL m_isBothSides{ IZ_FALSE };  ///< óºñ îªíËÇÃâ¬î€.
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_TRIANGLE_H__)
