#include "collision/ColTriangle.h"
#include "collision/ColRectangle.h"
#include "collision/ColPlane.h"

namespace izanagi
{
namespace col
{
    CTriangle::CTriangle()
    {
        a = b = c = d = padding = 0.0f;

        pt[0].Set(0.0f, 0.0f, 0.0f);
        pt[1].Set(0.0f, 0.0f, 0.0f);
        pt[2].Set(0.0f, 0.0f, 0.0f);
    }

    CTriangle::CTriangle(const math::SVector4 point[3])
    {
        Set(point);
    }

    CTriangle::CTriangle(
        const math::SVector4& pt0,
        const math::SVector4& pt1,
        const math::SVector4& pt2)
    {
        Set(pt0, pt1, pt2);
    }

    CTriangle::CTriangle(const CTriangle& rhs)
    {
        *this = rhs;
    }

    const CTriangle& CTriangle::operator=(const CTriangle& rhs)
    {
        memcpy(pt, rhs.pt, sizeof(pt));
        math::SVector4::Copy(nml, rhs.nml);
        d = rhs.d;

        return *this;
    }

    void CTriangle::Set(const math::SVector4 point[3])
    {
        Set(point[0], point[1], point[2]);
    }

    void CTriangle::Set(
        const math::SVector4& point0,
        const math::SVector4& point1,
        const math::SVector4& point2)
    {
        pt[0].Set(point0.x, point0.y, point0.z);
        pt[1].Set(point1.x, point1.y, point1.z);
        pt[2].Set(point2.x, point2.y, point2.z);

        math::CVector4 dir0(pt[1], pt[0], math::CVector4::INIT_SUB);
        math::CVector4 dir1(pt[2], pt[0], math::CVector4::INIT_SUB);

        math::SVector4::Cross(nml, dir0, dir1);
        math::SVector4::Normalize(nml, nml);

        d = -(a * pt[0].x + b * pt[0].y + c * pt[0].z);
    }

    // 4x4行列による変換.
    void CTriangle::Transform(const math::SMatrix44& mtx)
    {
        math::SVector4 tmp[COUNTOF(pt)];

        for (IZ_UINT i = 0; i < COUNTOF(pt); i++)
        {
            math::SMatrix44::Apply(tmp[i], pt[i], mtx);
        }

        Set(tmp);
    }

    // 三角形上に存在する点かどうか.
    IZ_BOOL CTriangle::IsOnTriangle(const math::SVector4& ptr) const
    {
        for (IZ_UINT i = 0; i < 3; i++)
        {
            IZ_UINT idx = (i + 1) % 3;
            math::CVector4 e(
                pt[idx].x - pt[i].x,
                pt[idx].y - pt[i].y,
                pt[idx].z - pt[i].z,
                0.0f);

            idx = (i + 2) % 3;
            math::CVector4 f(
                pt[idx].x - pt[i].x,
                pt[idx].y - pt[i].y,
                pt[idx].z - pt[i].z,
                0.0f);

            math::CVector4 g(
                ptr.x - pt[i].x,
                ptr.y - pt[i].y,
                ptr.z - pt[i].z,
                0.0f);

            math::SVector4 normalOfE;
            {
                // ベクトルeに垂直なベクトルを計算する
                math::SVector4 cross;
                
                math::SVector4::Cross(cross, e, f);
                math::SVector4::Cross(normalOfE, cross, e);

                // 三角形の内側にあるか計算する
                IZ_FLOAT dot0 = math::SVector4::Dot(f, normalOfE);
                IZ_FLOAT dot1 = math::SVector4::Dot(g, normalOfE);
                if (dot0 * dot1 < 0.0f)
                {
                    return IZ_FALSE;
                }

#if 0
                // 逆向きで計算してみる
                math::SVector4::Cross(normalOfE, e, cross);

                // 三角形の内側にあるか計算する
                dot0 = math::SVector4::Dot(f, normalOfE);
                dot1 = math::SVector4::Dot(g, normalOfE);
                if (dot0 * dot1 >= 0.0f)
                {
                    return IZ_TRUE;
                }
#endif
            }
        }

        return IZ_TRUE;
    }

    // レイと交差する点を取得
    IZ_BOOL CTriangle::GetIntersectPoint(
        const SRay& ray,
        math::SVector4& refPtr) const
    {
        CPlane plane(nml, pt[0]);

        IZ_BOOL isIntersect = plane.GetIntersectPoint(ray, refPtr);

        if (isIntersect)
        {
            isIntersect = IsOnTriangle(refPtr);
        }

        return isIntersect;
    }

    // レイと交差するかどうか
    IZ_BOOL CTriangle::IsIntersect(const SRay& ray)
    {
        math::SVector4 tmp;

        IZ_BOOL isIntersect = GetIntersectPoint(ray, tmp);
        return isIntersect;
    }
}   // namespace math
}   // namespace izanagi
