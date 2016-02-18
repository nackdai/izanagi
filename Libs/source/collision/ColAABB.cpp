#include "collision/ColAABB.h"

namespace izanagi
{
namespace col
{
    AABB::AABB(
        const math::SVector4& minPtr,
        const math::SVector4& maxPtr)
    {
        initialize(minPtr, maxPtr);
    }

    AABB::AABB(
        const math::SVector4& center,
        IZ_FLOAT lengthX,
        IZ_FLOAT lengthY,
        IZ_FLOAT lengthZ)
    {
        initialize(center, lengthX, lengthY, lengthZ);
    }

    AABB::AABB(const AABB& rhs)
    {
        *this = rhs;
    }

    const AABB& AABB::operator = (const AABB& rhs)
    {
        m_min.Set(rhs.m_min);
        m_length.Set(rhs.m_length);
        return *this;
    }

    void AABB::initialize(
        const math::SVector4& minPtr,
        const math::SVector4& maxPtr)
    {
        m_min.Set(minPtr);

        m_length.x = maxPtr.x - minPtr.x;
        m_length.y = maxPtr.y - minPtr.y;
        m_length.z = maxPtr.z - minPtr.z;
    }

    void AABB::initialize(
        const math::SVector4& center,
        IZ_FLOAT lengthX,
        IZ_FLOAT lengthY,
        IZ_FLOAT lengthZ)
    {
        m_min.Set(center);

        m_min.x -= lengthX * 0.5f;
        m_min.y -= lengthY * 0.5f;
        m_min.z -= lengthZ * 0.5f;

        m_length.x = lengthX;
        m_length.y = lengthY;
        m_length.z = lengthZ;
    }

    void AABB::apply(const math::SMatrix44& mtx)
    {
        // AABBなので、回転は無視.
        const auto& translate = mtx.v[3];

        math::SVector4::AddXYZ(m_min, m_min, translate);

        // TODO
        // スケール...
    }

    IZ_FLOAT AABB::computeRadius(const math::SVector4& normal) const
    {
        // ボックスを構成する3軸を計算.
        math::CVector4 x;
        x.x += m_length.x * 0.5f;

        math::CVector4 y;
        y.y += m_length.y * 0.5f;

        math::CVector4 z;
        z.z += m_length.z * 0.5f;

        // 実効半径を計算.
        IZ_FLOAT radius = math::CMath::Absf(math::SVector4::Dot(x, normal));
        radius += math::CMath::Absf(math::SVector4::Dot(y, normal));
        radius += math::CMath::Absf(math::SVector4::Dot(z, normal));
        radius *= 0.5f;

        return radius;
    }

    IZ_FLOAT AABB::computeRadius(
        const math::SMatrix44& mtxW2V,
        const math::SVector4& normal) const
    {
        // ボックスを構成する3軸を計算.
        math::CVector4 x;
        x.x += m_length.x * 0.5f;

        math::CVector4 y;
        y.y += m_length.y * 0.5f;

        math::CVector4 z;
        z.z += m_length.z * 0.5f;

        math::SMatrix44::ApplyXYZ(x, x, mtxW2V);
        math::SMatrix44::ApplyXYZ(y, y, mtxW2V);
        math::SMatrix44::ApplyXYZ(z, z, mtxW2V);

        // 実効半径を計算.
        IZ_FLOAT radius = math::CMath::Absf(math::SVector4::Dot(x, normal));
        radius += math::CMath::Absf(math::SVector4::Dot(y, normal));
        radius += math::CMath::Absf(math::SVector4::Dot(z, normal));
        radius *= 0.5f;

        return radius;
    }

    const math::SVector4 AABB::getCenter() const
    {
        math::SVector4 center;

        center.Set(m_min);

        center.x += m_length.x * 0.5f;
        center.y += m_length.y * 0.5f;
        center.z += m_length.z * 0.5f;

        return center;
    }
}   // namespace math
}   // namespace izanagi
