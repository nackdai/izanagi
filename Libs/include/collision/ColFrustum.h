#if !defined(__IZANAGI_COLLISION_FRUSTUM_H__)
#define __IZANAGI_COLLISION_FRUSTUM_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    class BoundingBox;

    class Frustum {
    public:
        Frustum() {}
        ~Frustum() {}

        Frustum(const Frustum& rhs);

        const Frustum& operator=(const Frustum& rhs);

    public:
        void initialize(
            IZ_FLOAT fNear,
            IZ_FLOAT fFar,
            IZ_FLOAT verticalFOV,
            IZ_FLOAT aspect);

        /** バウンディングボックスとの交差判定.
         */
        IZ_BOOL isIntersect(
            const BoundingBox* const box,
            const math::SMatrix44& mtxW2V);

        void computePlane(const math::SMatrix44& mtxW2C);

        /** バウンディングボックスとの交差判定.
         */
        IZ_BOOL isIntersect(const BoundingBox* const box);

    private:
        enum Plane {
            Left,
            Right,
            Top,
            Bottom,
            Near,
            Far,

            Num,
        };

        math::SVector4 m_planes[Plane::Num];
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_FRUSTUM_H__)
