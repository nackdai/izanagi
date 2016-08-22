#if !defined(__IZANAGI_COLLISION_BOUNDING_VOLUME_H__)
#define __IZANAGI_COLLISION_BOUNDING_VOLUME_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    class Ray;

    struct HitResult {
        math::CVector4 pt;
        math::CVector4 normal;
        IZ_FLOAT t;
    };

    /**
     */
    class BoundingVolume {
    public:
        BoundingVolume() {}
        virtual ~BoundingVolume() {}

    public:
        /** é¿å¯îºåaÇåvéZ.
         */
        PURE_VIRTUAL(IZ_FLOAT computeRadius(const math::SVector4& normal) const);

        /** é¿å¯îºåaÇåvéZ.
         */
        PURE_VIRTUAL(IZ_FLOAT computeRadius(
            const math::SMatrix44& mtxW2V,
            const math::SVector4& normal) const);

        /** íÜêSç¿ïWÇéÊìæ.
         */
        PURE_VIRTUAL(const math::SVector4 getCenter() const);

        /** Get if the ray is hit.
         */
        PURE_VIRTUAL(IZ_BOOL isHit(const Ray& ray, HitResult& res));

        void setL2W(const math::SMatrix44& mtxL2W)
        {
            m_L2W = mtxL2W;
        }

        const math::CMatrix44& getL2W() const
        {
            return m_L2W;
        }

        math::CMatrix44& getL2W()
        {
            return m_L2W;
        }

    protected:
        math::CMatrix44 m_L2W;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_BOUNDING_VOLUME_H__)
