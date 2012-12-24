#if !defined(__IZANAGI_MATH_TRIANGLE_H__)
#define __IZANAGI_MATH_TRIANGLE_H__

#include "MathVector.h"
#include "MathMatrix.h"

namespace izanagi
{
namespace math
{
    struct SRay;

    struct STriangle {
        SVector pt[3];
    };

    struct STrianglePlane : public STriangle {
        union {
            SVector nml;
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
        CTriangle(const SVector point[3]);
        CTriangle(const CTriangle& rhs);

        ~CTriangle() {}

    public:
        const CTriangle& operator=(const CTriangle& rhs);

        void Set(const SVector point[3]);

        void Set(
            const SVector& point0,
            const SVector& point1,
            const SVector& point2);

        /** 4x4行列による変換.
         */
        void Transform(const SMatrix& mtx);

        /** 三角形上に存在する点かどうか.
         */
        IZ_BOOL IsOnTriangle(const SVector& ptr) const;

        /** レイと交差する点を取得.
         */
        IZ_BOOL GetCrossPoint(
            const SRay& ray,
            SVector& refPtr) const;

        /** レイと交差するかどうか.
         */
        IZ_BOOL IsCross(const SRay& ray);
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_TRIANGLE_H__)
