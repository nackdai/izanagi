#include "collision/ColSphere.h"
#include "collision/ColRay.h"

namespace izanagi
{
namespace col
{
    Sphere::Sphere(
        IZ_FLOAT radius,
        const math::SVector4& center)
        : m_radius(radius), m_center(center)
    {
    }

    void Sphere::initialize(
        IZ_FLOAT radius,
        const math::SVector4& center)
    {
        m_radius = radius;
        m_center = center;
    }

    IZ_FLOAT Sphere::computeRadius(const math::SVector4& normal) const
    {
        return m_radius;
    }

    IZ_FLOAT Sphere::computeRadius(
        const math::SMatrix44& mtxW2V,
        const math::SVector4& normal) const
    {
        return m_radius;
    }

    const math::SVector4 Sphere::getCenter() const
    {
        math::CVector4 center;
        math::SMatrix44::Apply(center, m_center, m_L2W);
        return std::move(center);
    }

    // Get if the ray is hit.
    IZ_BOOL Sphere::isHit(const Ray& ray, HitResult& res)
    {
        // NOTE
        // http://t-pot.com/program/92_RayTraceSphere/index.html

        /*
            ���C�̕����� : X = x + tv
            �~�̕�����   : (X - c)(X - c) = r2

            �����̎���A������ƃ��C�̈ړ������ʂ̃p�����[�^t�����߂�v�Z�ɂȂ�

            �A�������� : (x + tv - c)(x + tv - c) = r2

            ���̎���t�Ɋւ��鎮�Ƃ��ĕό`�����

            |v|^2 * t^2 + 2v(x - c)t + |x - c|^2 - r^2 = 0

            ���̌������狅�ƌ������鎞�̃��C�Ɋւ���t�̒l��

                 - v(x - c)�}��D
            t = -----------------
                     | v |^2

            D = {v(x - c)}^2 - |v|^2 * (|x - c|^2 - r^2)
        */

        auto center = getCenter();

        IZ_FLOAT radius_sq = m_radius * m_radius;

        // ���S����̑��Έʒu
        math::CVector4 xc = ray.p - center;

        float xc2 = math::SVector4::DotXYZ(xc, xc);

        math::CVector4 v(ray.v);
        v.Normalize();
        v *= ray.t;

        float vxc = math::SVector4::DotXYZ(v, xc);
        float v2 = math::SVector4::DotXYZ(v, v);

        // ���ʎ�
        // NOTE
        // |v| = 1
        float D = vxc * vxc - v2 * (xc2 - radius_sq);

        // ��_�����݂��Ȃ��O�ꂽ
        if (D < 0) {
            return IZ_FALSE;
        }

        float tn = (-vxc - sqrtf(D)) / v2;
        float tp = (-vxc + sqrtf(D)) / v2;

        float t = 0.0f;

        if (tn < 0) {
            if (tp < 0) {
                return IZ_FALSE;
            }
            t = tp;
        }
        else {
            t = tn;
        }

        // pt = x + t * v
        res.pt = ray.p + t * ray.v;

        // nml = pt - center
        res.normal = res.pt - center;
        res.normal.Normalize();

        res.t = t;

        return IZ_TRUE;
    }
}   // namespace math
}   // namespace izanagi
