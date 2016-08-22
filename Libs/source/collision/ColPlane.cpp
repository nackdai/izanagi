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

    // 平面を設定.
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

    // 平面を設定.
    void Plane::Set(const math::SVector4& normal, IZ_FLOAT _d)
    {
        m_nml = normal;

        m_nml.w = 0.0f;
        m_nml.Normalize();

        m_d = _d;

        // 平面の基準位置を計算する
        // => 原点からの法線方向へのレイとの交点を計算する
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

    // 面の正側（法線の向き側）に点があるかどうか.
    IZ_BOOL Plane::IsPositive(const math::SVector4& ptr) const
    {
#if 0
        // 原点からの距離
        IZ_FLOAT len = math::SVector4::Length(ptr);

        // 平面まで届いていない
        if (len <= ::fabs(d))
        {
            return IZ_FALSE;
        }

        // 原点からの方向ベクトル
        math::CVector4 dir(ptr.x, ptr.y, ptr.z, 0.0f);

        // NOTE
        //   | /       |
        //   |/        |
        //---+----  ---+---
        //            /
        //           /
        //
        // 面の法線となす角が90度より小さければ面の正の側
        // => 内積の値がプラスであれば法線の向き側に点がある

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
            レイの方程式 : p = x + t v
            平面の方程式 : (p - p0)・n = 0
            x  : レイの出る点
            v  : レイの方向
            p0 : 平面上の１点
            n  : 法線ベクトル

            連立した結果、レイの進んだ距離tは

                (x - p0)・n
            t = ------------
                    v・n

            v・n=0 の時は、レイと平面が平行に進んでいるので、交わらない.
            t<0の時は、視線の後ろ側に交点があるので、やはり交わることがない.
        */

        math::CVector4 pt(m_pt);
        math::CVector4 nml(m_nml);
        auto d = computePlane(m_L2W, pt, nml);

        auto vn = math::SVector4::Dot(ray.v, nml);

        // カリングと発散を外す
        if (-0.00001f <= vn) {
            return IZ_FALSE;
        }

        math::CVector4 xp = pt - ray.p;

        float xpn = math::SVector4::Dot(xp, nml);

        IZ_FLOAT t = xpn / vn;

        // 後ろ向きのレイは無効
        if (t < 0) {
            return IZ_FALSE;
        }

        // 平面への射影.
        auto p = ray.p + t * ray.v;

        res.pt = p;
        res.normal = nml;
        res.t = t;

        return IZ_TRUE;
    }
}   // namespace math
}   // namespace izanagi
