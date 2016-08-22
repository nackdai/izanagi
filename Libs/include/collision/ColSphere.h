#if !defined(__IZANAGI_COLLISION_SPHERE_H__)
#define __IZANAGI_COLLISION_SPHERE_H__

#include "izMath.h"
#include "collision/ColBoundingVolume.h"

namespace izanagi
{
namespace col
{
    /**
     */
    class Sphere : public BoundingVolume {
    public:
        Sphere() {}
        virtual ~Sphere() {}

        Sphere(
            IZ_FLOAT radius,
            const math::SVector4& center);

    public:
        void initialize(
            IZ_FLOAT radius,
            const math::SVector4& center);

        /** ÀŒø”¼Œa‚ğŒvZ.
         */
        virtual IZ_FLOAT computeRadius(const math::SVector4& normal) const override;

        /** ÀŒø”¼Œa‚ğŒvZ.
         */
        virtual IZ_FLOAT computeRadius(
            const math::SMatrix44& mtxW2V,
            const math::SVector4& normal) const override;

        /** ’†SÀ•W‚ğæ“¾.
         */
        virtual const math::SVector4 getCenter() const override;

        /** Get if the ray is hit.
        */
        virtual IZ_BOOL isHit(const Ray& ray, HitResult& res) override;

    private:
        IZ_FLOAT m_radius{ 0.0f };

        math::CVector4 m_center;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_BOUNDING_VOLUME_H__)
