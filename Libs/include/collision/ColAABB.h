#if !defined(__IZANAGI_COLLISION_AABB_H__)
#define __IZANAGI_COLLISION_AABB_H__

#include "izMath.h"
#include "collision/ColBoundingBox.h"

namespace izanagi
{
namespace col
{
    /**
     */
    class AABB : public BoundingBox {
    public:
        AABB(){}
        AABB(
            const math::SVector4& minPtr,
            const math::SVector4& maxPtr);
        AABB(
            const math::SVector4& center,
            IZ_FLOAT lengthX,
            IZ_FLOAT lengthY,
            IZ_FLOAT lengthZ);

        virtual ~AABB() {}

        AABB(const AABB& rhs);

        const AABB& operator=(const AABB& rhs);

    public:
        void initialize(
            const math::SVector4& minPtr,
            const math::SVector4& maxPtr);

        void initialize(
            const math::SVector4& center,
            IZ_FLOAT lengthX,
            IZ_FLOAT lengthY,
            IZ_FLOAT lengthZ);

        /** �}�g���N�X��K�p.
         */
        virtual void apply(const math::SMatrix44& mtx) override;

        /** �������a���v�Z.
         */
        virtual IZ_FLOAT computeRadius(const math::SVector4& normal) const override;

        /** �������a���v�Z.
         */
        virtual IZ_FLOAT computeRadius(
            const math::SMatrix44& mtxW2V,
            const math::SVector4& normal) const override;

        /** ���S���W���擾.
         */
        virtual const math::SVector4 getCenter() const override;

    protected:
        math::SVector4 m_min;
        math::SVector3 m_length;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_AABB_H__)
