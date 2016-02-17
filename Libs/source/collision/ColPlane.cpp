#include "collision/ColPlane.h"
#include "collision/ColRay.h"

namespace izanagi
{
namespace col
{
    // ���ʏ�Ń��C�ƌ�������_���擾
    IZ_BOOL CPlane::GetIntersectPoint(
        const CPlane& plane,
        const SRay& ray,
        math::SVector4& refPtr,
        IZ_FLOAT* retRayCoefficient/*= IZ_NULL*/)
    {
        return plane.GetIntersectPoint(ray, refPtr, retRayCoefficient);
    }

    CPlane::CPlane()
    {
        a = b = c = d = padding = 0.0f;
    }

    CPlane::CPlane(const math::SVector4& normal, const math::SVector4& ptr)
    {
        Set(normal, ptr);
    }

    CPlane::CPlane(const SPlane& rhs)
    {
        *this = rhs;
    }

    CPlane::CPlane(IZ_FLOAT _a, IZ_FLOAT _b, IZ_FLOAT _c, IZ_FLOAT _d)
    {
        a = _a;
        b = _b;
        c = _c;
        d = _d;
    }

    const CPlane& CPlane::operator=(const SPlane& rhs)
    {
        Set(rhs.nml, rhs.pt);
        return *this;
    }

    // ���ʂ�ݒ�.
    void CPlane::Set(const math::SVector4& normal, const math::SVector4& ptr)
    {
        math::SVector4::Copy(nml, normal);

        nml.w = 0.0f;
        math::SVector4::Normalize(nml, nml);

        d = -(a * ptr.x + b * ptr.y + c * ptr.z);

        pt.Set(ptr.x, ptr.y, ptr.z);
    }

    // ���ʂ�ݒ�.
    void CPlane::Set(const math::SVector4& normal, IZ_FLOAT _d)
    {
        math::SVector4::Copy(nml, normal);

        nml.w = 0.0f;
        math::SVector4::Normalize(nml, nml);

        d = _d;

        // ���ʂ̊�ʒu���v�Z����
        // => ���_����̖@�������ւ̃��C�Ƃ̌�_���v�Z����
        CRay ray(
            math::CVector4(0.0f, 0.0f, 0.0f),
            nml);
        GetIntersectPoint(ray, this->pt);
    }

    // ���_����̋������擾.
    IZ_FLOAT CPlane::GetDistance() const
    {
        return ::fabs(d);
    }

    // ���ʏ�ɑ��݂���_���ǂ���.
    IZ_BOOL CPlane::IsOnPlane(const math::SVector4& ptr)
    {
        IZ_FLOAT tmp = a * ptr.x + b * ptr.y + c * ptr.z;
        return math::CMath::IsNearyEqualZero(tmp + d);
    }

    // 4x4�s��ɂ��ϊ�.
    void CPlane::Transform(const math::SMatrix44& mtx)
    {
        // TODO
        // Matrix�ɃX�P�[�����܂܂�Ă����ꍇ
        // ��xNormalize����H

        math::SMatrix44::ApplyXYZ(nml, nml, mtx);
    }

    // ���ʂ�����킷�x�N�g�����擾
    void CPlane::GetPlaneVector(math::SVector4& plane)
    {
        plane.Set(a, b, c, d);
    }

    // ���C�ƌ�������_���擾
    IZ_BOOL CPlane::GetIntersectPoint(
        const SRay& ray,
        math::SVector4& refPtr,
        IZ_FLOAT* retRayCoefficient/*= IZ_NULL*/) const
    {
        // NOTE
        // Plane : L = <N, D>
        // Ray   : P(t) = Q + Vt
        //
        // N�EP(t) + D = 0
        // <=> N�EQ + (N�EV)t + D = 0
        // <=> t = -(N�EQ + D) / N�EV
        // <=> t = -L�EQ / L�EV
        //
        // Q = (Qx, Qy, Qz, 1) => �ʒu�x�N�g��
        // V = (Vx, Vy, Vz, 0) => �����x�N�g��
        // L = (Nx, Ny, Nz, D)
        // �Ȃ̂ŁA
        // N�EQ + D = Nx * Qx + Ny * Qy + Nz * Qz + D * 1 = L�EQ
        // N�EV + 0 = Nx * Vx + Ny * Vy + Nz * Vz + D * 0 = L�EV

        math::CVector4 plane(a, b, c, d);

        // L�EV
        IZ_FLOAT d = math::SVector4::Dot(plane, ray.v);
        if (math::CMath::IsNearyEqualZero(d))
        {
            return IZ_FALSE;
        }

        // L�EQ
        IZ_FLOAT t = math::SVector4::Dot(plane, ray.p);

        // t = -L�EQ / L�EV 
        t = -t / d;

        if (retRayCoefficient != IZ_NULL) {
            *retRayCoefficient = t;
        }

        refPtr.x = ray.p.x + t * ray.v.x;
        refPtr.y = ray.p.y + t * ray.v.y;
        refPtr.z = ray.p.z + t * ray.v.z;
        refPtr.w = 1.0f;

        return IZ_TRUE;
    }

    // �����ƌ�������_���擾.
    IZ_BOOL CPlane::GetIntersectPoint(
        const math::SVector4& from,
        const math::SVector4& to,
        math::SVector4& refPtr,
        IZ_FLOAT* retRayCoefficient/*= IZ_NULL*/) const
    {
        if ((IsPositive(from) && !IsPositive(to))
            || (!IsPositive(from) && IsPositive(to)))
        {
            // ��̓_�͖ʂ̐����̂��ꂼ��ɂȂ��Ƃ����Ȃ�
            CRay ray(
                from,
                math::CVector4(to, from, math::CVector4::INIT_SUB));

            IZ_BOOL ret = GetIntersectPoint(ray, refPtr, retRayCoefficient);
            return ret;
        }

        return IZ_FALSE;
    }

    // ���C�ƌ������邩�ǂ���
    IZ_BOOL CPlane::IsIntersect(const SRay& ray) const
    {
        math::CVector4 plane(a, b, c, d);

        IZ_FLOAT d = math::SVector4::Dot(plane, ray.v);

        if (math::CMath::IsNearyEqualZero(d))
        {
            return IZ_FALSE;
        }

        return IZ_TRUE;
    }

    // �ʂ̐����i�@���̌������j�ɓ_�����邩�ǂ���.
    IZ_BOOL CPlane::IsPositive(const math::SVector4& ptr) const
    {
#if 0
        // ���_����̋���
        IZ_FLOAT len = math::SVector4::Length(ptr);

        // ���ʂ܂œ͂��Ă��Ȃ�
        if (len <= ::fabs(d))
        {
            return IZ_FALSE;
        }

        // ���_����̕����x�N�g��
        math::CVector4 dir(ptr.x, ptr.y, ptr.z, 0.0f);

        // NOTE
        //   | /       |
        //   |/        |
        //---+----  ---+---
        //            /
        //           /
        //
        // �ʂ̖@���ƂȂ��p��90�x��菬������Ζʂ̐��̑�
        // => ���ς̒l���v���X�ł���Ζ@���̌������ɓ_������

        IZ_FLOAT dot = math::SVector4::Dot(nml, dir);

        return (dot >= 0.0f);
#else
        math::CVector4 n(a, b, c, d);
        IZ_FLOAT dot = math::SVector4::Dot(n, ptr);
        dot = (math::CMath::IsNearyEqualZero(dot) ? 0.0f : dot);
        return (dot >= 0.0f);
#endif
    }
}   // namespace math
}   // namespace izanagi
