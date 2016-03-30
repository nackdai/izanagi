#include "collision/ColSphere.h"

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

    void Sphere::apply(const math::SMatrix44& mtx)
    {
        math::SMatrix44::Apply(m_center, m_center, mtx);

        // TODO
        // スケール...
        // 半径をスケーリングしないといけない...
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
        return m_center;
    }
}   // namespace math
}   // namespace izanagi
