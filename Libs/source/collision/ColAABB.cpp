#include "collision/ColAABB.h"

namespace izanagi
{
namespace col
{
    AABB::AABB()
    {
        m_min.Set(IZ_FLOAT_MAX, IZ_FLOAT_MAX, IZ_FLOAT_MAX);
        m_max.Set(0, 0, 0);
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
        m_max.Set(rhs.m_max);
        m_size.Set(rhs.m_size);
        return *this;
    }

    void AABB::initialize(
        const math::SVector4& minPtr,
        const math::SVector4& maxPtr)
    {
        m_min.Set(minPtr);
        m_max.Set(maxPtr);

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

        m_max.Set(m_min);
        m_max.x += lengthX;
        m_max.y += lengthY;
        m_max.z += lengthZ;

        m_size.x = lengthX;
        m_size.y = lengthY;
        m_size.z = lengthZ;
    }

    void AABB::apply(const math::SMatrix44& mtx)
    {
        // AABBなので、回転は無視.
        const auto& translate = mtx.v[3];

        math::SVector4::AddXYZ(m_min, m_min, translate);
        math::SVector4::AddXYZ(m_max, m_max, translate);

        // TODO
        // スケール...
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
        math::SVector4 center;

        center.Set(m_min);

        center.x += m_size.x * 0.5f;
        center.y += m_size.y * 0.5f;
        center.z += m_size.z * 0.5f;

        return std::move(center);
    }

    // 最小座標を取得.
    const math::SVector4& AABB::getMin() const
    {
        return m_min;
    }

    // 最大座標を取得.
    const math::SVector4 AABB::getMax() const
    {
#if 0
        math::SVector4 ret;

        ret.Set(m_min);

        ret.x += m_size.x;
        ret.y += m_size.y;
        ret.z += m_size.z;

        return std::move(ret);
#else
        return m_max;
#endif
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

        m_max.x = m_min.x + m_size.x;
        m_max.y = m_min.y + m_size.y;
        m_max.z = m_min.z + m_size.z;
    }

    // AABBに外接するバウンディングスフィアを取得.
    col::Sphere AABB::getCircumscribedSphere() const
    {
        // TODO
        // 毎回計算するのはコスト的に問題ありそうなので、何かのタイミングのときだけ更新したい.
        
        auto center = getCenter();
        auto radius = math::SVector4::Length2(center, m_min);

        col::Sphere sphere(radius, center);

        return std::move(sphere);
    }

    // AABBに内接するバウンディングスフィアを取得.
    col::Sphere AABB::getInscribedSphere() const
    {
        // TODO
        // 毎回計算するのはコスト的に問題ありそうなので、何かのタイミングのときだけ更新したい.

#if 0
        // 立方体でないといけない.
        makeCubic();

        // 立方体なので、全方向のサイズは同じ.
        auto radius = m_size.x * 0.5f;
#else
        // もっとも小さい辺の長さを取得.
        IZ_FLOAT minSize = IZ_MIN(m_size.x, m_size.y);
        minSize = IZ_MIN(minSize, m_size.z);

        auto radius = minSize * 0.5f;
#endif

        auto center = getCenter();

        col::Sphere sphere(radius, center);

        return std::move(sphere);
    }

    // Compute if this aabb can be displayed by world to clip transform matrix.
    IZ_BOOL AABB::canDisplay(const math::SMatrix44& mtxW2C)
    {
        // 8点.
        math::CVector4 ptr[8];
        {
            ptr[0].Set(m_min.x, m_min.y, m_min.z);

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

    // Check if the point is contain in this aabb.
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
        return ((m_min.x <= point.x && point.x <= m_max.x)
            && (m_min.y <= point.y && point.y <= m_max.y)
            && (m_min.z <= point.z && point.z <= m_max.z));
#endif
    }
}   // namespace math
}   // namespace izanagi
