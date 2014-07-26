#include "math/MathRectangle.h"
#include "math/MathRay.h"
#include "math/MathPlane.h"
#include "math/MathCVector4.h"
#include "math/MathTriangle.h"

namespace izanagi
{
namespace math
{
    CRectangle::CRectangle()
    {
        a = b = c = d = padding = 0.0f;

        pt.Set(0.0f, 0.0f, 0.0f);
        v[0].Set(0.0f, 0.0f, 0.0f, 0.0f);
        v[1].Set(0.0f, 0.0f, 0.0f, 0.0f);
    }

    CRectangle::CRectangle(
        const SVector4& point,
        const SVector4& v0,
        const SVector4& v1)
    {
        Set(point, v0, v1);
    }

    CRectangle::CRectangle(const CRectangle& rhs)
    {
        *this = rhs;
    }

    const CRectangle& CRectangle::operator=(const CRectangle& rhs)
    {
#if 0
        SVector4::Copy(pt, rhs.pt);

        SVector4::Copy(v[0], rhs.v[0]);
        SVector4::Copy(v[1], rhs.v[1]);

        SVector4::Copy(nml, rhs.nml);

        d = rhs.d;
#else
        Set(
            rhs.pt,
            rhs.v[0],
            rhs.v[1]);
#endif

        return *this;
    }

    // ��`��ݒ�.
    void CRectangle::Set(
        const SVector4& point,
        const SVector4& v0,
        const SVector4& v1)
    {
        SVector4::Copy(pt, point);

        SVector4::Copy(v[0], v0);
        SVector4::Copy(v[1], v1);

        SVector4::Cross(nml, v[0], v[1]);
        SVector4::Normalize(nml, nml);

        d = -(a * pt.x + b * pt.y + c * pt.z);
    }

    // ���_����̋������擾.
    IZ_FLOAT CRectangle::GetDistance() const
    {
        return ::fabs(d);
    }

    // ��`��ɑ��݂���_���ǂ���.
    IZ_BOOL CRectangle::IsOnRectangle(const SVector4& ptr) const
    {
#if 0
        SVector4 p;
        SVector4::Sub(p, ptr, pt);

        IZ_FLOAT dot1 = SVector4::Dot(p, v[0]);
        IZ_FLOAT dot2 = SVector4::Dot(p, v[1]);

        IZ_FLOAT length1 = SVector4::Length(v[0]);
        IZ_FLOAT length2 = SVector4::Length(v[1]);

#if 0
        return IS_IN_BOUND(dot1 / length1, 0.0f, 1.0f) && IS_IN_BOUND(dot2 / length2, 0.0f, 1.0f);
#else
        // pt�͋�`�̒��S�_�Ȃ̂�
        // ��`���\������_�͓�̃x�N�g���� -0.5 - 0.5 �͈̔͂ɑ��݂���
        return IS_IN_BOUND(dot1 / length1, -0.5f, 0.5f)
            && IS_IN_BOUND(dot2 / length2, -0.5f, 0.5f);
#endif
#else
        // ��`���̎O�p�`�ɕ������āA���ꂼ��̎O�p�`�Ŕ�����s��

        CVector4 p0 = CVector4(pt.x, pt.y, pt.z) + CVector4(v[0].x, v[0].y, v[0].z);
        CVector4 p1 = CVector4(pt.x, pt.y, pt.z) + CVector4(v[1].x, v[1].y, v[1].z);
        CVector4 p2 = p0 + CVector4(v[1].x, v[1].y, v[1].z);

        CTriangle tri0(pt, p0, p1);

        IZ_BOOL isIntersect = tri0.IsOnTriangle(ptr);

        if (!isIntersect) {
            CTriangle tri1(p0, p2, p1);

            isIntersect = tri1.IsOnTriangle(ptr);
        }

        return isIntersect;
#endif
    }

    // 4x4�s��ɂ��ϊ�.
    void CRectangle::Transform(const SMatrix44& mtx)
    {
        SMatrix44::Apply(pt, pt, mtx);

        SMatrix44::ApplyXYZ(v[0], v[0], mtx);
        SMatrix44::ApplyXYZ(v[1], v[1], mtx);

        SVector4::Cross(nml, v[0], v[1]);
        SVector4::Normalize(nml, nml);

        // �v�Z��Z���ۂ߂�
        nml.x = CMath::IsNearyEqualZero(nml.x) ? 0.0f : nml.x;
        nml.y = CMath::IsNearyEqualZero(nml.y) ? 0.0f : nml.y;
        nml.z = CMath::IsNearyEqualZero(nml.z) ? 0.0f : nml.z;
        SVector4::Normalize(nml, nml);

        d = -(a * pt.x + b * pt.y + c * pt.z);
    }

    void CRectangle::Transform(
        CRectangle& dst,
        const SMatrix44& mtx)
    {
        dst = *this;    // Copy
        dst.Transform(mtx);
    }

    // ���C�ƌ�������_���擾
    IZ_BOOL CRectangle::GetIntersectPoint(
        const SRay& ray,
        SVector4& refPtr,
        IZ_FLOAT* retRayCoefficient/*= IZ_NULL*/) const
    {
        IZ_BOOL isIntersect = GetIntersectPoint(
            CPlane::GetIntersectPoint,
            ray,
            refPtr,
            retRayCoefficient);

        return isIntersect;
    }

    // ���C�ƌ������邩�ǂ���
    IZ_BOOL CRectangle::IsIntersect(
        const SRay& ray,
        IZ_FLOAT* retRayCoefficient/*= IZ_NULL*/)
    {
        CVector4 ptr;

        IZ_BOOL isIntersect = GetIntersectPoint(ray, ptr, retRayCoefficient);

        return isIntersect;
    }

    // ���ʂ��擾.
    void CRectangle::GetPlane(SPlane& plane) const
    {
        plane.pt.Set(
            pt.x,
            pt.y,
            pt.z);

        plane.nml.Set(a, b, c, 0.0f);
        plane.d = d;
    }

    // ���C�ƌ�������_���擾
    IZ_BOOL CRectangle::GetIntersectPoint(
        CRectangle::GetCrossPointFunc func,
        const SRay& ray,
        SVector4& refPtr,
        IZ_FLOAT* retRayCoefficient) const
    {
        CVector4 ptr;

        IZ_BOOL isIntersect = (*func)(
            CPlane(nml, pt),
            ray, ptr,
            retRayCoefficient);

        if (isIntersect) {
            isIntersect = IsOnRectangle(ptr);
            if (isIntersect) {
                SVector4::Copy(refPtr, ptr);
            }
        }

        return isIntersect;
    }
}   // namespace math
}   // namespace izanagi
