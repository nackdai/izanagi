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

        /** 4x4�s��ɂ��ϊ�.
         */
        void Transform(const SMatrix& mtx);

        /** �O�p�`��ɑ��݂���_���ǂ���.
         */
        IZ_BOOL IsOnTriangle(const SVector& ptr) const;

        /** ���C�ƌ�������_���擾.
         */
        IZ_BOOL GetCrossPoint(
            const SRay& ray,
            SVector& refPtr) const;

        /** ���C�ƌ������邩�ǂ���.
         */
        IZ_BOOL IsCross(const SRay& ray);
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_TRIANGLE_H__)
