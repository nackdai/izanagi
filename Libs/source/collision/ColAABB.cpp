#include "collision/ColAABB.h"
#include "collision/ColRay.h"

namespace izanagi
{
namespace col
{
    AABB::AABB()
    {
    }

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
        m_size.Set(rhs.m_size);
        return *this;
    }

    void AABB::initialize(
        const math::SVector4& minPtr,
        const math::SVector4& maxPtr)
    {
        m_min.Set(minPtr);

        m_size.x = maxPtr.x - minPtr.x;
        m_size.y = maxPtr.y - minPtr.y;
        m_size.z = maxPtr.z - minPtr.z;
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

        m_size.x = lengthX;
        m_size.y = lengthY;
        m_size.z = lengthZ;
    }

    IZ_FLOAT AABB::computeRadius(const math::SVector4& normal) const
    {
        // ボックスを構成する3軸を計算.
        math::CVector4 x;
        x.x = m_size.x;

        math::CVector4 y;
        y.y = m_size.y;

        math::CVector4 z;
        z.z = m_size.z;

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
        x.x = m_size.x;

        math::CVector4 y;
        y.y = m_size.y;

        math::CVector4 z;
        z.z = m_size.z;

        // View座標系に変換.
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
        math::CVector4 center = getMin();

        center.x += m_size.x * 0.5f;
        center.y += m_size.y * 0.5f;
        center.z += m_size.z * 0.5f;

        return std::move(center);
    }

    IZ_BOOL AABB::isHit(const Ray& ray, HitResult& res)
    {
        IZ_BOOL ret = IZ_FALSE;

        math::CVector4 dir(ray.v);

        const auto min = getMin();
        const auto max = getMax();

        IZ_FLOAT tmin = -IZ_FLOAT_MAX;
        IZ_FLOAT tmax = IZ_FLOAT_MAX;

        for (IZ_UINT i = 0; i < 3; i++) {
            if (dir.v[i] == 0.0f) {
                continue;
            }

            IZ_FLOAT inv = 1.0f / dir.v[i];

            // NOTE
            // ray : r = p + t * v
            // plane of AABB : x(t) = p(x) + t * v(x)
            //  t = (p(x) - x(t)) / v(x)
            // x軸の面は手前と奥があるので、それぞれの t を計算.
            // t がx軸の面の手前と奥の x の範囲内であれば、レイがAABBを通る.
            // これをxyz軸について計算する.

            IZ_FLOAT t0 = (min.v[i] - ray.p.v[i]) * inv;
            IZ_FLOAT t1 = (max.v[i] - ray.p.v[i]) * inv;

            if (inv < 0.0f) {
                std::swap(t0, t1);
            }

            tmin = (t0 > tmin ? t0 : tmin);
            tmax = (t1 < tmax ? t1 : tmax);

            if (tmax <= tmin) {
                return IZ_FALSE;
            }

            ret = IZ_TRUE;
        }

        return ret;
    }

    // 最小座標を取得.
    const math::SVector4 AABB::getMin() const
    {
        math::CVector4 ret(m_min);
        math::SMatrix44::Apply(ret, m_min, m_L2W);

        return std::move(ret);
    }

    // 最大座標を取得.
    const math::SVector4 AABB::getMax() const
    {
        math::CVector4 ret = getMin();

        ret.x += m_size.x;
        ret.y += m_size.y;
        ret.z += m_size.z;

        return std::move(ret);
    }

    // サイズを取得.
    const math::SVector3& AABB::getSize() const
    {
        return m_size;
    }

    void AABB::makeCubic()
    {
        // 最大のサイズを取得.
        IZ_FLOAT maxSize = IZ_MAX(m_size.x, m_size.y);
        maxSize = IZ_MAX(maxSize, m_size.z);

        // 全部同じサイズにする.
        m_size.x = maxSize;
        m_size.y = maxSize;
        m_size.z = maxSize;
    }

    // AABBに外接するバウンディングスフィアを取得.
    col::Sphere AABB::getCircumscribedSphere() const
    {
        // TODO
        // 毎回計算するのはコスト的に問題ありそうなので、何かのタイミングのときだけ更新したい.
        
        auto center = getCenter();

        auto min = getMin();

        auto radius = math::SVector4::Length2(center, min);

        col::Sphere sphere(radius, center);
        sphere.setL2W(m_L2W);

        return std::move(sphere);
    }

    // AABBに内接するバウンディングスフィアを取得.
    col::Sphere AABB::getInscribedSphere() const
    {
        // TODO
        // 毎回計算するのはコスト的に問題ありそうなので、何かのタイミングのときだけ更新したい.

        // もっとも小さい辺の長さを取得.
        IZ_FLOAT minSize = IZ_MIN(m_size.x, m_size.y);
        minSize = IZ_MIN(minSize, m_size.z);

        auto radius = minSize * 0.5f;

        auto center = getCenter();

        col::Sphere sphere(radius, center);
        sphere.setL2W(m_L2W);

        return std::move(sphere);
    }

    // Compute if this aabb can be displayed by world to clip transform matrix.
    IZ_BOOL AABB::canDisplay(const math::SMatrix44& mtxW2C)
    {
        // 8点.
        math::CVector4 ptr[8];
        {
            ptr[0] = getMin();

            ptr[1].Set(ptr[0]);
            ptr[1].Add(m_size.x, 0.0f, 0.0f);

            ptr[2].Set(ptr[0]);
            ptr[2].Add(0.0f, m_size.y, 0.0f);

            ptr[3].Set(ptr[1]);
            ptr[3].Add(0.0f, m_size.y, 0.0f);

            ptr[4].Set(ptr[0]);
            ptr[4].Add(0.0f, 0.0f, m_size.z);

            ptr[5].Set(ptr[4]);
            ptr[5].Add(m_size.x, 0.0f, 0.0f);

            ptr[6].Set(ptr[4]);
            ptr[6].Add(0.0f, m_size.y, 0.0f);

            ptr[7].Set(ptr[5]);
            ptr[7].Add(0.0f, m_size.y, 0.0f);
        }

        m_displayFlag = 0;

        for (IZ_UINT i = 0; i < COUNTOF(ptr); i++) {
            math::SVector4 tmp;
            math::SMatrix44::Apply(tmp, ptr[i], mtxW2C);

            math::SVector4::Div(tmp, tmp, tmp.w);

            // はみ出しチェック
            if (tmp.x <= 1.0f) {
                m_displayFlag |= (1 << Clip::PositiveX);
            }
            if (tmp.x >= -1.0f) {
                m_displayFlag |= (1 << Clip::NegativeX);
            }
            if (tmp.y <= 1.0f) {
                m_displayFlag |= (1 << Clip::PositiveY);
            }
            if (tmp.y >= -1.0f) {
                m_displayFlag |= (1 << Clip::NegativeY);
            }
            if (tmp.z <= 1.0f) {
                m_displayFlag |= (1 << Clip::PositiveZ);
            }
            if (tmp.z >= -1.0f) {
                m_displayFlag |= (1 << Clip::NegativeZ);
            }
        }

        return (m_displayFlag > 0);
    }

    // Get the pre-computed result if the aabb can be displayed. 
    IZ_BOOL AABB::canDisplay() const
    {
        return (m_displayFlag > 0);
    }

    // Check if the point is contain in the aabb.
    IZ_BOOL AABB::isContain(const math::SVector3& point)
    {
#if 0
        auto x = point.x;
        auto y = point.y;
        auto z = point.z;

        const auto& vMin = m_min;
        izanagi::math::SVector3 vMax;
        vMax.x = m_min.x + m_size.x;
        vMax.y = m_min.y + m_size.y;
        vMax.z = m_min.z + m_size.z;

        IZ_BOOL ret = IZ_FALSE;

        if ((vMin.x <= x && x <= vMax.x)
            && (vMin.y <= y && y <= vMax.y)
            && (vMin.z <= z && z <= vMax.z))
        {
            ret = IZ_TRUE;
        }

        return ret;
#else
        auto min = getMin();
        auto max = getMax();

        return ((min.x <= point.x && point.x <= max.x)
            && (min.y <= point.y && point.y <= max.y)
            && (min.z <= point.z && point.z <= max.z));
#endif
    }

    // Check if the aabb is valid.
    IZ_BOOL AABB::isValid()
    {
        auto min = getMin();
        auto max = getMax();

        return (min.x < max.x && min.y < max.y && min.z < max.z);
    }
}   // namespace math
}   // namespace izanagi
