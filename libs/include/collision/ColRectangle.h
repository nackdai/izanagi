#if !defined(__IZANAGI_COLLISION_RECTANGLE_H__)
#define __IZANAGI_COLLISION_RECTANGLE_H__

#include "izMath.h"

namespace izanagi
{
namespace col
{
    struct SRay;
    struct SPlane;
    class CPlane;

    /** ��`�p�����[�^�\����
     */
    struct SRectangle {
        math::SVector4 pt;     ///< ���㌴�_
        math::SVector4 v[2];   ///< ���_����L�т��`���`������Q�x�N�g��

        union {
            math::SVector4 nml;    ///< �@��
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
            const math::SVector4& point,
            const math::SVector4& v0,
            const math::SVector4& v1);
        CRectangle(const CRectangle& rhs);

        ~CRectangle() {}

    public:
        const CRectangle& operator=(const CRectangle& rhs);

        /** ��`��ݒ�.
         */
        void Set(
            const math::SVector4& point,
            const math::SVector4& v0,
            const math::SVector4& v1);

        /** ���_����̋������擾.
         */
        IZ_FLOAT GetDistance() const;

        /** ��`��ɑ��݂���_���ǂ���.
         */
        IZ_BOOL IsOnRectangle(const math::SVector4& ptr) const;

        /** 4x4�s��ɂ��ϊ�.
         */
        void Transform(const math::SMatrix44& mtx);

        void Transform(
            CRectangle& dst,
            const math::SMatrix44& mtx);

        /** ���C�ƌ�������_���擾.
         */
        IZ_BOOL GetIntersectPoint(
            const SRay& ray,
            math::SVector4& refPtr,
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
        typedef IZ_BOOL (*GetCrossPointFunc)(const CPlane& plane, const SRay& ray, math::SVector4& refPtr, IZ_FLOAT* retRayCoefficient);

        // ���C�ƌ�������_���擾
        IZ_BOOL GetIntersectPoint(
            GetCrossPointFunc func,
            const SRay& ray,
            math::SVector4& refPtr,
            IZ_FLOAT* retRayCoefficient) const;
    };
}   // namespace math
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_COLLISION_RECTANGLE_H__)
