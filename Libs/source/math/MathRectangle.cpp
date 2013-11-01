#include "math/MathRectangle.h"
#include "math/MathRay.h"
#include "math/MathPlane.h"
#include "math/MathCVector.h"
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
        const SVector& point,
        const SVector& v0,
        const SVector& v1)
    {
        Set(point, v0, v1);
    }

    CRectangle::CRectangle(const CRectangle& rhs)
    {
        *this = rhs;
    }

    const CRectangle& CRectangle::operator=(const CRectangle& rhs)
    {
        SVector::Copy(pt, rhs.pt);

        SVector::Copy(v[0], rhs.v[0]);
        SVector::Copy(v[1], rhs.v[1]);

        SVector::Copy(nml, rhs.nml);

        d = rhs.d;

        return *this;
    }

    // 矩形を設定.
    void CRectangle::Set(
        const SVector& point,
        const SVector& v0,
        const SVector& v1)
    {
        SVector::Copy(pt, point);

        SVector::Copy(v[0], v0);
        SVector::Copy(v[1], v1);

        SVector::Cross(nml, v[0], v[1]);
        SVector::Normalize(nml, nml);

        d = -(a * pt.x + b * pt.y + c * pt.z);
    }

    // 原点からの距離を取得.
    IZ_FLOAT CRectangle::GetDistance() const
    {
        return ::fabs(d);
    }

    // 矩形上に存在する点かどうか.
    IZ_BOOL CRectangle::IsOnRectangle(const SVector& ptr) const
    {
#if 0
        SVector p;
        SVector::Sub(p, ptr, pt);

        IZ_FLOAT dot1 = SVector::Dot(p, v[0]);
        IZ_FLOAT dot2 = SVector::Dot(p, v[1]);

        IZ_FLOAT length1 = SVector::Length(v[0]);
        IZ_FLOAT length2 = SVector::Length(v[1]);

#if 0
        return IS_IN_BOUND(dot1 / length1, 0.0f, 1.0f) && IS_IN_BOUND(dot2 / length2, 0.0f, 1.0f);
#else
        // ptは矩形の中心点なので
        // 矩形を構成する点は二つのベクトルの -0.5 - 0.5 の範囲に存在する
        return IS_IN_BOUND(dot1 / length1, -0.5f, 0.5f)
            && IS_IN_BOUND(dot2 / length2, -0.5f, 0.5f);
#endif
#else
        // 矩形を二つの三角形に分割して、それぞれの三角形で判定を行う

        CVector p0 = CVector(pt.x, pt.y, pt.z) + CVector(v[0].x, v[0].y, v[0].z);
        CVector p1 = CVector(pt.x, pt.y, pt.z) + CVector(v[1].x, v[1].y, v[1].z);
        CVector p2 = p0 + CVector(v[1].x, v[1].y, v[1].z);

        CTriangle tri0(pt, p0, p1);

        IZ_BOOL isIntersect = tri0.IsOnTriangle(ptr);

        if (!isIntersect) {
            CTriangle tri1(p0, p2, p1);

            isIntersect = tri1.IsOnTriangle(ptr);
        }

        return isIntersect;
#endif
    }

    // 4x4行列による変換.
    void CRectangle::Transform(const SMatrix& mtx)
    {
        CVector p(0.0f, 0.0f, 0.0f);
        SMatrix::Apply(p, p, mtx);

        SVector::AddXYZ(pt, pt, p);

        SMatrix::ApplyXYZ(v[0], v[0], mtx);
        SMatrix::ApplyXYZ(v[1], v[1], mtx);

        SVector::Cross(nml, v[0], v[1]);
        SVector::Normalize(nml, nml);

        // 計算誤算を丸める
        nml.x = CMath::IsNearyEqualZero(nml.x) ? 0.0f : nml.x;
        nml.y = CMath::IsNearyEqualZero(nml.y) ? 0.0f : nml.y;
        nml.z = CMath::IsNearyEqualZero(nml.z) ? 0.0f : nml.z;
        SVector::Normalize(nml, nml);

        d = -(a * pt.x + b * pt.y + c * pt.z);
    }

    // レイと交差する点を取得
    IZ_BOOL CRectangle::GetIntersectPoint(
        const SRay& ray,
        SVector& refPtr) const
    {
        IZ_BOOL isIntersect = GetIntersectPoint(
            CPlane::GetIntersectPoint,
            ray,
            refPtr);

        return isIntersect;
    }

    // レイと交差するかどうか
    IZ_BOOL CRectangle::IsIntersect(const SRay& ray)
    {
        CVector ptr;

        IZ_BOOL isIntersect = GetIntersectPoint(ray, ptr);

        return isIntersect;
    }

    // 平面を取得.
    void CRectangle::GetPlane(SPlane& plane) const
    {
        plane.pt.Set(
            pt.x,
            pt.y,
            pt.z);

        plane.nml.Set(a, b, c, 0.0f);
        plane.d = d;
    }

    // レイと交差する点を取得
    IZ_BOOL CRectangle::GetIntersectPoint(
        CRectangle::GetCrossPointFunc func,
        const SRay& ray,
        SVector& refPtr) const
    {
        CVector ptr;

        IZ_BOOL isIntersect = (*func)(
            CPlane(nml, pt),
            ray, ptr);

        if (isIntersect) {
            isIntersect = IsOnRectangle(ptr);
            if (isIntersect) {
                SVector::Copy(refPtr, ptr);
            }
        }

        return isIntersect;
    }
}   // namespace math
}   // namespace izanagi
