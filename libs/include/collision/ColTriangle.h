#if !defined(__IZANAGI_COLLISION_TRIANGLE_H__)
#define __IZANAGI_COLLISION_TRIANGLE_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    struct SRay;

    struct STriangle {
        math::SVector4 pt[3];
    };

    struct STrianglePlane : public STriangle {
        union {
            math::SVector4 nml;
            struct {
                IZ_FLOAT a;
                IZ_FLOAT b;
                IZ_FLOAT c;
                IZ_FLOAT padding;
            };
        };
        IZ_FLOAT d;
    };

    class CTriangle : public STrianglePlane {
    public:
        CTriangle();
        CTriangle(const math::SVector4 point[3]);
        CTriangle(
            const math::SVector4& pt0,
            const math::SVector4& pt1,
            const math::SVector4& pt2);
        CTriangle(const CTriangle& rhs);

        ~CTriangle() {}

    public:
        const CTriangle& operator=(const CTriangle& rhs);

        void Set(const math::SVector4 point[3]);

        void Set(
            const math::SVector4& point0,
            const math::SVector4& point1,
            const math::SVector4& point2);

        /** 4x4行列による変換.
         */
        void Transform(const math::SMatrix44& mtx);

        /** 三角形上に存在する点かどうか.
         */
        IZ_BOOL IsOnTriangle(const math::SVector4& ptr) const;

        /** レイと交差する点を取得.
         */
        IZ_BOOL GetIntersectPoint(
            const SRay& ray,
            math::SVector4& refPtr) const;

        /** レイと交差するかどうか.
         */
        IZ_BOOL IsIntersect(const SRay& ray);
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_TRIANGLE_H__)
