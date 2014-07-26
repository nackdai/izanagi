#include "math/MathTriangle.h"
#include "math/MathRectangle.h"
#include "math/MathPlane.h"
#include "math/MathCVector4.h"

namespace izanagi
{
namespace math
{
    CTriangle::CTriangle()
    {
        a = b = c = d = padding = 0.0f;

        pt[0].Set(0.0f, 0.0f, 0.0f);
        pt[1].Set(0.0f, 0.0f, 0.0f);
        pt[2].Set(0.0f, 0.0f, 0.0f);
    }

    CTriangle::CTriangle(const SVector4 point[3])
    {
        Set(point);
    }

    CTriangle::CTriangle(
        const SVector4& pt0,
        const SVector4& pt1,
        const SVector4& pt2)
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
        SVector4::Copy(nml, rhs.nml);
        d = rhs.d;

        return *this;
    }

    void CTriangle::Set(const SVector4 point[3])
    {
        Set(point[0], point[1], point[2]);
    }

    void CTriangle::Set(
        const SVector4& point0,
        const SVector4& point1,
        const SVector4& point2)
    {
        pt[0].Set(point0.x, point0.y, point0.z);
        pt[1].Set(point1.x, point1.y, point1.z);
        pt[2].Set(point2.x, point2.y, point2.z);

        CVector4 dir0(pt[1], pt[0], CVector4::INIT_SUB);
        CVector4 dir1(pt[2], pt[0], CVector4::INIT_SUB);

        SVector4::Cross(nml, dir0, dir1);
        SVector4::Normalize(nml, nml);

        d = -(a * pt[0].x + b * pt[0].y + c * pt[0].z);
    }

    // 4x4�s��ɂ��ϊ�.
    void CTriangle::Transform(const SMatrix& mtx)
    {
        SVector4 tmp[COUNTOF(pt)];

        for (IZ_UINT i = 0; i < COUNTOF(pt); i++)
        {
            SMatrix::Apply(tmp[i], pt[i], mtx);
        }

        Set(tmp);
    }

    // �O�p�`��ɑ��݂���_���ǂ���.
    IZ_BOOL CTriangle::IsOnTriangle(const SVector4& ptr) const
    {
        for (IZ_UINT i = 0; i < 3; i++)
        {
            IZ_UINT idx = (i + 1) % 3;
            CVector4 e(
                pt[idx].x - pt[i].x,
                pt[idx].y - pt[i].y,
                pt[idx].z - pt[i].z,
                0.0f);

            idx = (i + 2) % 3;
            CVector4 f(
                pt[idx].x - pt[i].x,
                pt[idx].y - pt[i].y,
                pt[idx].z - pt[i].z,
                0.0f);

            CVector4 g(
                ptr.x - pt[i].x,
                ptr.y - pt[i].y,
                ptr.z - pt[i].z,
                0.0f);

            SVector4 normalOfE;
            {
                // �x�N�g��e�ɐ����ȃx�N�g�����v�Z����
                SVector4 cross;
                
                SVector4::Cross(cross, e, f);
                SVector4::Cross(normalOfE, cross, e);

                // �O�p�`�̓����ɂ��邩�v�Z����
                IZ_FLOAT dot0 = SVector4::Dot(f, normalOfE);
                IZ_FLOAT dot1 = SVector4::Dot(g, normalOfE);
                if (dot0 * dot1 < 0.0f)
                {
                    return IZ_FALSE;
                }

#if 0
                // �t�����Ōv�Z���Ă݂�
                SVector4::Cross(normalOfE, e, cross);

                // �O�p�`�̓����ɂ��邩�v�Z����
                dot0 = SVector4::Dot(f, normalOfE);
                dot1 = SVector4::Dot(g, normalOfE);
                if (dot0 * dot1 >= 0.0f)
                {
                    return IZ_TRUE;
                }
#endif
            }
        }

        return IZ_TRUE;
    }

    // ���C�ƌ�������_���擾
    IZ_BOOL CTriangle::GetIntersectPoint(
        const SRay& ray,
        SVector4& refPtr) const
    {
        CPlane plane(nml, pt[0]);

        IZ_BOOL isIntersect = plane.GetIntersectPoint(ray, refPtr);

        if (isIntersect)
        {
            isIntersect = IsOnTriangle(refPtr);
        }

        return isIntersect;
    }

    // ���C�ƌ������邩�ǂ���
    IZ_BOOL CTriangle::IsIntersect(const SRay& ray)
    {
        SVector4 tmp;

        IZ_BOOL isIntersect = GetIntersectPoint(ray, tmp);
        return isIntersect;
    }
}   // namespace math
}   // namespace izanagi
