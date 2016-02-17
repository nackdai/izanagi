#include "collision/ColRay.h"

namespace izanagi
{
namespace col
{
    CRay::CRay()
    {
        p.Set(0.0f, 0.0f, 0.0f);
        v.Set(0.0f, 0.0f, 0.0f, 0.0f);
        t = 0.0f;
    }

    CRay::CRay(const math::SVector4& ptr, const math::SVector4& dir)
    {
        IZ_FLOAT t = math::SVector4::Length(dir);

        math::SVector4 v;
        math::SVector4::Normalize(v, dir);

        Set(ptr, v, t);
    }

    CRay::CRay(const math::SVector4& ptr, const math::SVector4& dir, IZ_FLOAT _t)
    {
        Set(ptr, dir, _t);
    }

    CRay::CRay(const SRay& rhs)
    {
        Set(rhs.p, rhs.v, rhs.t);
    }

    // 直線をコピー.
    const CRay& CRay::operator=(const SRay& rhs)
    {
        Set(rhs.p, rhs.v, rhs.t);
        return *this;
    }

    // 直線を設定する.
    void CRay::Set(
        const math::SVector4& ptr,
        const math::SVector4& dir,
        IZ_FLOAT _t)
    {
        math::SVector4::Copy(p, ptr);
        math::SVector4::CopyXYZ(v, dir);
        t = _t;

        Normalize();
    }

    // 平行かどうか.
    IZ_BOOL CRay::IsParallel(const SRay& rhs) const
    {
        // NOTE
        // 方向ベクトルは正規化済み

        IZ_BOOL ret = (math::CMath::IsNearyEqual(v.x, rhs.v.x)
            && math::CMath::IsNearyEqual(v.y, rhs.v.y)
            && math::CMath::IsNearyEqual(v.z, rhs.v.z));
        return ret;
    }

    // 無限遠方の直線にする.
    void CRay::SetInifite()
    {
        t = IZ_FLOAT_MAX;
    }

    // 係数を掛けた方向ベクトルを取得.
    void CRay::GetMulAttnDir(math::SVector4& dir)
    {
        math::SVector4::ScaleXYZ(dir, v, t);
    }

    // 直線上にあるかどうか.
    IZ_BOOL CRay::IsOnRay(const math::SVector4& ptr)
    {
        // TODO
        return IZ_TRUE;
    }

    // 方向ベクトルを正規化.
    void CRay::Normalize()
    {
        math::SVector4::Normalize(v, v);
    }
}   // namespace math
}   // namespace izanagi
