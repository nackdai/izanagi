#if !defined(__IZANAGI_MATH_RECTANGLE_H__)
#define __IZANAGI_MATH_RECTANGLE_H__

#include "MathVector.h"
#include "MathMatrix.h"

namespace izanagi
{
namespace math
{
    struct SRay;
    struct SPlane;
    class CPlane;

    struct SRectangle {
        SVector pt;
        struct {
            SVector dir;
            IZ_FLOAT length;
        } v[2];
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

    class CRectangle : public SRectangle {
    public:
        CRectangle();
        CRectangle(
            const SVector& point,
            IZ_FLOAT lengthX,
            IZ_FLOAT lengthZ);
        CRectangle(const CRectangle& rhs);

        ~CRectangle() {}

    public:
        const CRectangle& operator=(const CRectangle& rhs);

        /** ��`��ݒ�.
         */
        void Set(
            const SVector& point,
            IZ_FLOAT lengthX,
            IZ_FLOAT lengthZ);

        /** ���_����̋������擾.
         */
        IZ_FLOAT GetDistance() const;

        /** ��`��ɑ��݂���_���ǂ���.
         */
        IZ_BOOL IsOnRectangle(const SVector& ptr) const;

        /** 4x4�s��ɂ��ϊ�.
         */
        void Transform(const SMatrix& mtx);

        /** ���C�ƌ�������_���擾.
         */
        IZ_BOOL GetCrossPoint(
            const SRay& ray,
            SVector& refPtr) const;

        /** ���C�ƌ������邩�ǂ���.
         */
        IZ_BOOL IsCross(const SRay& ray);

        /** ���ʂ��擾.
         */
        void GetPlane(SPlane& plane) const;

        /** X�����̃x�N�g�����擾.
         */
        const SVector& GetX() const;

        /** Y�����̃x�N�g�����擾.
         */
        const SVector& GetY() const;

        /** �����擾.
         */
        IZ_FLOAT GetWidth() const;

        /** �������擾.
         */
        IZ_FLOAT GetHeight() const;

    private:
        typedef IZ_BOOL (*GetCrossPointFunc)(const CPlane& plane, const SRay& ray, SVector& refPtr);

        // ���C�ƌ�������_���擾
        IZ_BOOL GetCrossPoint(
            GetCrossPointFunc func,
            const SRay& ray,
            SVector& refPtr) const;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_RECTANGLE_H__)
