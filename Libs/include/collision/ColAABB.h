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

        /** マトリクスを適用.
         */
        virtual void apply(const math::SMatrix44& mtx) override;

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

        /** 最小座標を取得.
         */
        const math::SVector4& getMin() const;

        /** 最大座標を取得.
         */
        const math::SVector4 getMax() const;

        /** サイズを取得.
         */
        const math::SVector3& getSize() const;

        /** 立方体にする.
         */
        void makeCubic();

        IZ_BOOL canDisplay(const math::SMatrix44& mtxW2C);

        IZ_BOOL canDisplay() const;

    protected:
        math::SVector4 m_min;
        math::SVector3 m_size;

        enum Clip {
            PositiveX,
            NegativeX,
            PositiveY,
            NegativeY,
            PositiveZ,
            NegativeZ,
        };

        IZ_UINT m_displayFlag{ 0 };
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_AABB_H__)
