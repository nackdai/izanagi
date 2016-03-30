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

    void AABB::apply(const math::SMatrix44& mtx)
    {
        // AABB�Ȃ̂ŁA��]�͖���.
        const auto& translate = mtx.v[3];

        math::SVector4::AddXYZ(m_min, m_min, translate);

        // TODO
        // �X�P�[��...
    }

    IZ_FLOAT AABB::computeRadius(const math::SVector4& normal) const
    {
        // �{�b�N�X���\������3�����v�Z.
        math::CVector4 x;
        x.x = m_size.x;

        math::CVector4 y;
        y.y = m_size.y;

        math::CVector4 z;
        z.z = m_size.z;

        // �������a���v�Z.
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
        // �{�b�N�X���\������3�����v�Z.
        math::CVector4 x;
        x.x = m_size.x;

        math::CVector4 y;
        y.y = m_size.y;

        math::CVector4 z;
        z.z = m_size.z;

        // View���W�n�ɕϊ�.
        math::SMatrix44::ApplyXYZ(x, x, mtxW2V);
        math::SMatrix44::ApplyXYZ(y, y, mtxW2V);
        math::SMatrix44::ApplyXYZ(z, z, mtxW2V);

        // �������a���v�Z.
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

    // �ŏ����W���擾.
    const math::SVector4& AABB::getMin() const
    {
        return m_min;
    }

    // �ő���W���擾.
    const math::SVector4 AABB::getMax() const
    {
        math::SVector4 ret;

        ret.Set(m_min);

        ret.x += m_size.x;
        ret.y += m_size.y;
        ret.z += m_size.z;

        return std::move(ret);
    }

    // �T�C�Y���擾.
    const math::SVector3& AABB::getSize() const
    {
        return m_size;
    }

    void AABB::makeCubic()
    {
        // �ő�̃T�C�Y���擾.
        IZ_FLOAT maxSize = IZ_MAX(m_size.x, m_size.y);
        maxSize = IZ_MAX(maxSize, m_size.z);

        // �S�������T�C�Y�ɂ���.
        m_size.x = maxSize;
        m_size.y = maxSize;
        m_size.z = maxSize;
    }

    IZ_BOOL AABB::canDisplay(const math::SMatrix44& mtxW2C)
    {
        // 8�_.
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

            // �͂ݏo���`�F�b�N
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

    IZ_BOOL AABB::canDisplay() const
    {
        return (m_displayFlag > 0);
    }
}   // namespace math
}   // namespace izanagi
