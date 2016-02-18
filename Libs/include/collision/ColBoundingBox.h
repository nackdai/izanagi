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
        /** マトリクスを適用.
         */
        PURE_VIRTUAL(void apply(const math::SMatrix44& mtx));

        /** 実効半径を計算.
         */
        PURE_VIRTUAL(IZ_FLOAT computeRadius(const math::SVector4& normal) const);

        /** 実効半径を計算.
         */
        PURE_VIRTUAL(IZ_FLOAT computeRadius(
            const math::SMatrix44& mtxW2V,
            const math::SVector4& normal) const);

        /** 中心座標を取得.
         */
        PURE_VIRTUAL(const math::SVector4 getCenter() const);
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_BOUNDING_BOX_H__)
