#include "collision/ColRectangle.h"
#include "collision/ColRay.h"
#include "collision/ColPlane.h"
#include "collision/ColTriangle.h"

namespace izanagi
{
namespace col
{
    Rectangle::Rectangle()
    {
        m_pt[0].Set(0.0f, 0.0f, 0.0f);
        m_pt[1].Set(0.0f, 0.0f, 0.0f);
        m_pt[2].Set(0.0f, 0.0f, 0.0f);
        m_pt[3].Set(0.0f, 0.0f, 0.0f);

        m_nml.Set(0.0f, 0.0f, 0.0f, 0.0f);
        m_d = 0.0f;
    }

    Rectangle::Rectangle(
        const math::SVector4& point,
        const math::SVector4& v0,
        const math::SVector4& v1)
    {
        Set(point, v0, v1);
    }

    Rectangle::Rectangle(const Rectangle& rhs)
    {
        *this = rhs;
    }

    const Rectangle& Rectangle::operator=(const Rectangle& rhs)
    {
        m_pt[0] = rhs.m_pt[0];
        m_pt[1] = rhs.m_pt[1];
        m_pt[2] = rhs.m_pt[2];
        m_pt[3] = rhs.m_pt[3];

        m_nml = rhs.m_nml;
        m_d = rhs.m_d;

        return *this;
    }

    // 矩形を設定.
    void Rectangle::Set(
        const math::SVector4& point,
        const math::SVector4& v0,
        const math::SVector4& v1)
    {
        math::SVector4 pt1;
        math::SVector4::AddXYZ(pt1, point, v0);
        pt1.w = 1.0f;

        math::SVector4 pt2;
        math::SVector4::AddXYZ(pt2, point, v1);
        pt2.w = 1.0f;

        math::SVector4 pt3;
        math::SVector4::AddXYZ(pt3, pt1, v1);
        pt3.w = 1.0f;

        Set(point, pt1, pt2, pt3);
    }

    void Rectangle::Set(
        const math::SVector4& pt0,
        const math::SVector4& pt1,
        const math::SVector4& pt2,
        const math::SVector4& pt3)
    {
        m_pt[0] = pt0;
        m_pt[1] = pt1;
        m_pt[2] = pt2;
        m_pt[3] = pt3;

        m_d = computePlane(
            m_pt[0], m_pt[1], m_pt[2], m_pt[3],
            m_nml);
    }

    IZ_FLOAT Rectangle::computePlane(
        const math::CMatrix44& mtxL2W,
        math::CVector4& pt0,
        math::CVector4& pt1,
        math::CVector4& pt2,
        math::CVector4& pt3,
        math::CVector4& nml)
    {
        math::SMatrix44::Apply(pt0, pt0, mtxL2W);
        math::SMatrix44::Apply(pt1, pt1, mtxL2W);
        math::SMatrix44::Apply(pt2, pt2, mtxL2W);
        math::SMatrix44::Apply(pt3, pt3, mtxL2W);

        auto d = computePlane(
            pt0, pt1, pt2, pt3,
            nml);

        return d;
    }

    IZ_FLOAT Rectangle::computePlane(
        const math::CVector4& pt0,
        const math::CVector4& pt1,
        const math::CVector4& pt2,
        const math::CVector4& pt3,
        math::CVector4& nml)
    {
        auto v0 = pt1 - pt0;
        auto v1 = pt2 - pt0;

        math::SVector4::Cross(nml, v0, v1);
        nml.Normalize();

        IZ_FLOAT a = nml.x;
        IZ_FLOAT b = nml.y;
        IZ_FLOAT c = nml.z;

        auto d = -(a * pt0.x + b * pt0.y + c * pt0.z);
        return d;
    }

    IZ_FLOAT Rectangle::computeRadius(const math::SVector4& normal) const
    {
        auto center = getCenter();

        math::CVector4 pt;
        math::SMatrix44::Apply(pt, m_pt[0], m_L2W);

        auto radius = math::SVector4::Length2(center, pt);

        return radius;
    }

    IZ_FLOAT Rectangle::computeRadius(
        const math::SMatrix44& mtxW2V,
        const math::SVector4& normal) const
    {
        auto radius = computeRadius(normal);
        return radius;
    }

    void Rectangle::apply(math::CVector4* pt) const
    {
        for (IZ_UINT i = 0; i < COUNTOF(m_pt); i++) {
            math::SMatrix44::Apply(pt[i], m_pt[i], m_L2W);
        }
    }

    const math::SVector4 Rectangle::getCenter() const
    {
        math::CVector4 pt[4];
        apply(pt);

        auto center = pt[0] + pt[1] + pt[2] + pt[3];
        center /= 4;

        return std::move(center);
    }

    // Get if the ray is hit.
    IZ_BOOL Rectangle::isHit(const Ray& ray, HitResult& res)
    {
        // 2つの三角形に分割して、それぞれでテストする.

        math::CVector4 pt[4];
        apply(pt);

        Triangle tri0(pt[0], pt[1], pt[2]);

        if (tri0.isHit(ray, res)) {
            return IZ_TRUE;
        }

        Triangle tri1(pt[1], pt[3], pt[2]);

        if (tri1.isHit(ray, res)) {
            return IZ_TRUE;
        }

        return IZ_FALSE;
    }
}   // namespace math
}   // namespace izanagi
