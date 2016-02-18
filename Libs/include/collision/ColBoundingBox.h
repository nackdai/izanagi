#if !defined(__IZANAGI_COLLISION_BOUNDING_BOX_H__)
#define __IZANAGI_COLLISION_BOUNDING_BOX_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    /**
     */
    class BoundingBox {
    public:
        BoundingBox() {}
        virtual ~BoundingBox() {}

    public:
        /** �}�g���N�X��K�p.
         */
        PURE_VIRTUAL(void apply(const math::SMatrix44& mtx));

        /** �������a���v�Z.
         */
        PURE_VIRTUAL(IZ_FLOAT computeRadius(const math::SVector4& normal) const);

        /** �������a���v�Z.
         */
        PURE_VIRTUAL(IZ_FLOAT computeRadius(
            const math::SMatrix44& mtxW2V,
            const math::SVector4& normal) const);

        /** ���S���W���擾.
         */
        PURE_VIRTUAL(const math::SVector4 getCenter() const);
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_BOUNDING_BOX_H__)
