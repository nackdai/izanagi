#if !defined(__IZANAGI_COLLISION_FRUSTUM_H__)
#define __IZANAGI_COLLISION_FRUSTUM_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    class BoundingVolume;

    class Frustum {
    public:
        enum Plane {
            Left,
            Right,
            Top,
            Bottom,
            Near,
            Far,

            Num,
        };

    public:
        Frustum();
        ~Frustum() {}

        Frustum(const Frustum& rhs);

        const Frustum& operator=(const Frustum& rhs);

    public:
        void initialize(
            IZ_FLOAT fNear,
            IZ_FLOAT fFar,
            IZ_FLOAT verticalFOV,
            IZ_FLOAT aspect);

        void initialize(
            Plane idx,
            const math::SVector4& plane);

        void initialize(const math::SVector4 plane[Plane::Num]);

        /** バウンディングボックスとの交差判定.
         */
        IZ_BOOL isIntersect(
            const BoundingVolume* const volume,
            const math::SMatrix44& mtxW2V);

        void computePlane(const math::SMatrix44& mtxW2C);

        /** バウンディングボックスとの交差判定.
         */
        IZ_BOOL isIntersect(const BoundingVolume* const volume);

    private:
        math::SVector4 m_planes[Plane::Num];
        math::SVector4 m_planesInClipCoord[Plane::Num];
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_FRUSTUM_H__)
