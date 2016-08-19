#include "collision/ColPlane.h"
#include "collision/ColRay.h"

namespace izanagi
{
namespace col
{
    Plane::Plane()
    {
        m_nml.Set(0.0f, 0.0f, 0.0f, 0.0f);
        m_d = 0.0f;

        m_pt.Set(0.0f, 0.0f, 0.0f);
    }

    Plane::Plane(const math::SVector4& normal, const math::SVector4& ptr)
    {
        Set(normal, ptr);
    }

    const Plane& Plane::operator=(const Plane& rhs)
    {
        Set(rhs.m_nml, rhs.m_pt);
        return *this;
    }

    // ���ʂ�ݒ�.
    void Plane::Set(const math::SVector4& normal, const math::SVector4& ptr)
    {
        math::SVector4::Copy(m_nml, normal);

        m_nml.w = 0.0f;
        m_nml.Normalize();

        IZ_FLOAT a = m_nml.x;
        IZ_FLOAT b = m_nml.y;
        IZ_FLOAT c = m_nml.z;

        m_d = -(a * ptr.x + b * ptr.y + c * ptr.z);

        m_pt.Set(ptr.x, ptr.y, ptr.z);
    }

    // ���ʂ�ݒ�.
    void Plane::Set(const math::SVector4& normal, IZ_FLOAT _d)
    {
        m_nml = normal;

        m_nml.w = 0.0f;
        m_nml.Normalize();

        m_d = _d;

        // ���ʂ̊�ʒu���v�Z����
        // => ���_����̖@�������ւ̃��C�Ƃ̌�_���v�Z����
        Ray ray(
            math::CVector4(0.0f, 0.0f, 0.0f),
            m_nml);

        HitResult hit;

        auto result = isHit(ray, hit);
        IZ_ASSERT(result);

        m_pt = hit.pt;
    }

    IZ_FLOAT Plane::computePlane(
        const math::CMatrix44& mtxL2W,
        math::CVector4& pt,
        math::CVector4& nml)
    {
        math::SMatrix44::Apply(pt, pt, mtxL2W);
        math::SMatrix44::Apply(nml, nml, mtxL2W);

        IZ_FLOAT a = nml.x;
        IZ_FLOAT b = nml.y;
        IZ_FLOAT c = nml.z;

        auto d = -(a * pt.x + b * pt.y + c * pt.z);
        return d;
    }

    // �ʂ̐����i�@���̌������j�ɓ_�����邩�ǂ���.
    IZ_BOOL Plane::IsPositive(const math::SVector4& ptr) const
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
        math::CVector4 pt(m_pt);
        math::CVector4 nml(m_nml);
        auto d = computePlane(m_L2W, pt, nml);

        math::CVector4 n(nml.x, nml.y, nml.z, d);
        IZ_FLOAT dot = math::SVector4::Dot(n, pt);
        dot = (math::CMath::IsNearyEqualZero(dot) ? 0.0f : dot);
        return (dot >= 0.0f);
#endif
    }

    IZ_FLOAT Plane::computeRadius(const math::SVector4& normal) const
    {
        IZ_ASSERT(IZ_FALSE);
        return 0.0f;
    }

    IZ_FLOAT Plane::computeRadius(
        const math::SMatrix44& mtxW2V,
        const math::SVector4& normal) const
    {
        IZ_ASSERT(IZ_FALSE);
        return 0.0f;
    }

    const math::SVector4 Plane::getCenter() const
    {
        IZ_ASSERT(IZ_FALSE);
        math::CVector4 tmp;
        return std::move(tmp);
    }

    // Get if the ray is hit.
    IZ_BOOL Plane::isHit(const Ray& ray, HitResult& res)
    {
        /*
            ���C�̕����� : p = x + t v
            ���ʂ̕����� : (p - p0)�En = 0
            x  : ���C�̏o��_
            v  : ���C�̕���
            p0 : ���ʏ�̂P�_
            n  : �@���x�N�g��

            �A���������ʁA���C�̐i�񂾋���t��

                (x - p0)�En
            t = ------------
                    v�En

            v�En=0 �̎��́A���C�ƕ��ʂ����s�ɐi��ł���̂ŁA�����Ȃ�.
            t<0�̎��́A�����̌�둤�Ɍ�_������̂ŁA��͂����邱�Ƃ��Ȃ�.
        */

        math::CVector4 pt(m_pt);
        math::CVector4 nml(m_nml);
        auto d = computePlane(m_L2W, pt, nml);

        auto vn = math::SVector4::Dot(ray.v, nml);

        // �J�����O�Ɣ��U���O��
        if (-0.00001f <= vn) {
            return IZ_FALSE;
        }

        math::CVector4 xp = pt - ray.p;

        float xpn = math::SVector4::Dot(xp, nml);

        IZ_FLOAT t = xpn / vn;

        // �������̃��C�͖���
        if (t < 0) {
            return IZ_FALSE;
        }

        // ���ʂւ̎ˉe.
        auto p = ray.p + t * ray.v;

        res.pt = p;
        res.normal = nml;
        res.t = t;

        return IZ_TRUE;
    }
}   // namespace math
}   // namespace izanagi
