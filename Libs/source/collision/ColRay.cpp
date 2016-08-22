#include "collision/ColRay.h"

namespace izanagi
{
namespace col
{
    Ray::Ray()
    {
        p.Set(0.0f, 0.0f, 0.0f);
        v.Set(0.0f, 0.0f, 0.0f, 0.0f);
        t = 0.0f;
    }

    Ray::Ray(const math::SVector4& ptr, const math::SVector4& dir)
    {
        IZ_FLOAT t = math::SVector4::Length(dir);

        math::SVector4 v;
        math::SVector4::Normalize(v, dir);

        Set(ptr, v, t);
    }

    Ray::Ray(const math::SVector4& ptr, const math::SVector4& dir, IZ_FLOAT _t)
    {
        Set(ptr, dir, _t);
    }

    Ray::Ray(const SRay& rhs)
    {
        Set(rhs.p, rhs.v, rhs.t);
    }

    // 直線をコピー.
    const Ray& Ray::operator=(const SRay& rhs)
    {
        Set(rhs.p, rhs.v, rhs.t);
        return *this;
    }

    // 直線を設定する.
    void Ray::Set(
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
    IZ_BOOL Ray::IsParallel(const SRay& rhs) const
    {
        // NOTE
        // 方向ベクトルは正規化済み

        IZ_BOOL ret = (math::CMath::IsNearyEqual(v.x, rhs.v.x)
            && math::CMath::IsNearyEqual(v.y, rhs.v.y)
            && math::CMath::IsNearyEqual(v.z, rhs.v.z));
        return ret;
    }

    // 無限遠方の直線にする.
    void Ray::SetInifite()
    {
        t = IZ_FLOAT_MAX;
    }

    // 係数を掛けた方向ベクトルを取得.
    void Ray::GetMulAttnDir(math::SVector4& dir)
    {
        math::SVector4::ScaleXYZ(dir, v, t);
    }

    // 方向ベクトルを正規化.
    void Ray::Normalize()
    {
        math::SVector4::Normalize(v, v);
    }
}   // namespace math
}   // namespace izanagi
