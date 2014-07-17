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

    /** ��`�p�����[�^�\����
     */
    struct SRectangle {
        SVector pt;     ///< ���㌴�_
        SVector v[2];   ///< ���_����L�т��`���`������Q�x�N�g��

        union {
            SVector nml;    ///< �@��
            struct {
                IZ_FLOAT a;
                IZ_FLOAT b;
                IZ_FLOAT c;
                IZ_FLOAT padding;
            };
        };
        IZ_FLOAT d;     ///< ���_����̋���
    };

    /** ��`�N���X
     */
    class CRectangle : public SRectangle {
    public:
        CRectangle();
        CRectangle(
            const SVector& point,
            const SVector& v0,
            const SVector& v1);
        CRectangle(const CRectangle& rhs);

        ~CRectangle() {}

    public:
        const CRectangle& operator=(const CRectangle& rhs);

        /** ��`��ݒ�.
         */
        void Set(
            const SVector& point,
            const SVector& v0,
            const SVector& v1);

        /** ���_����̋������擾.
         */
        IZ_FLOAT GetDistance() const;

        /** ��`��ɑ��݂���_���ǂ���.
         */
        IZ_BOOL IsOnRectangle(const SVector& ptr) const;

        /** 4x4�s��ɂ��ϊ�.
         */
        void Transform(const SMatrix& mtx);

        void Transform(
            CRectangle& dst,
            const SMatrix& mtx);

        /** ���C�ƌ�������_���擾.
         */
        IZ_BOOL GetIntersectPoint(
            const SRay& ray,
            SVector& refPtr,
            IZ_FLOAT* retRayCoefficient = IZ_NULL) const;

        /** ���C�ƌ������邩�ǂ���.
         */
        IZ_BOOL IsIntersect(
            const SRay& ray,
            IZ_FLOAT* retRayCoefficient = IZ_NULL);

        /** ���ʂ��擾.
         */
        void GetPlane(SPlane& plane) const;

    private:
        typedef IZ_BOOL (*GetCrossPointFunc)(const CPlane& plane, const SRay& ray, SVector& refPtr, IZ_FLOAT* retRayCoefficient);

        // ���C�ƌ�������_���擾
        IZ_BOOL GetIntersectPoint(
            GetCrossPointFunc func,
            const SRay& ray,
            SVector& refPtr,
            IZ_FLOAT* retRayCoefficient) const;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_MATH_RECTANGLE_H__)
