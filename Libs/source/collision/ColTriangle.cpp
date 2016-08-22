#include "collision/ColTriangle.h"
#include "collision/ColRay.h"

namespace izanagi
{
namespace col
{
    Triangle::Triangle()
    {
        m_pt[0].Set(0.0f, 0.0f, 0.0f);
        m_pt[1].Set(0.0f, 0.0f, 0.0f);
        m_pt[2].Set(0.0f, 0.0f, 0.0f);

        m_nml.Set(0.0f, 0.0f, 0.0f, 0.0f);
        m_d = 0.0f;
    }

    Triangle::Triangle(
        const math::SVector4 point[3],
        IZ_BOOL isBothSides/*= IZ_FALSE*/)
    {
        Set(point, isBothSides);
    }

    Triangle::Triangle(
        const math::SVector4& pt0,
        const math::SVector4& pt1,
        const math::SVector4& pt2,
        IZ_BOOL isBothSides/*= IZ_FALSE*/)
    {
        Set(pt0, pt1, pt2, isBothSides);
    }

    Triangle::Triangle(const Triangle& rhs)
    {
        *this = rhs;
    }

    const Triangle& Triangle::operator=(const Triangle& rhs)
    {
        memcpy(m_pt, rhs.m_pt, sizeof(m_pt));
        math::SVector4::Copy(m_nml, rhs.m_nml);
        m_d = rhs.m_d;

        m_isBothSides = rhs.m_isBothSides;

        return *this;
    }

    void Triangle::Set(
        const math::SVector4 point[3],
        IZ_BOOL isBothSides/*= IZ_FALSE*/)
    {
        Set(point[0], point[1], point[2], isBothSides);
    }

    void Triangle::Set(
        const math::SVector4& point0,
        const math::SVector4& point1,
        const math::SVector4& point2,
        IZ_BOOL isBothSides/*= IZ_FALSE*/)
    {
        m_pt[0].Set(point0.x, point0.y, point0.z);
        m_pt[1].Set(point1.x, point1.y, point1.z);
        m_pt[2].Set(point2.x, point2.y, point2.z);

        m_d = computePlane(
            m_pt[0], m_pt[1], m_pt[2],
            m_nml);

        m_isBothSides = isBothSides;
    }

    IZ_FLOAT Triangle::computePlane(
        const math::CVector4& pt0,
        const math::CVector4& pt1,
        const math::CVector4& pt2,
        math::CVector4& retNml)
    {
        math::CVector4 dir0 = pt1 - pt0;
        math::CVector4 dir1 = pt2 - pt0;

        math::SVector4::Cross(retNml, dir0, dir1);
        retNml.Normalize();

        IZ_FLOAT a = retNml.x;
        IZ_FLOAT b = retNml.y;
        IZ_FLOAT c = retNml.z;

        IZ_FLOAT d = -(a * pt0.x + b * pt0.y + c * pt0.z);

        return d;
    }

    // �������a���v�Z.
    IZ_FLOAT Triangle::computeRadius(const math::SVector4& normal) const
    {
        // �O�p�`�ɊO�ڂ��鋅�̔��a��Ԃ�.

        // �O�S.
        auto center = getCenter();

        math::CVector4 pt;
        math::SMatrix44::Apply(pt, m_pt[0], m_L2W);

        // �O�S�ƎO�p�`�̓_�̋��� = ���a.
        auto radius = math::SVector4::Length2(center, pt);

        return radius;
    }

    // �������a���v�Z.
    IZ_FLOAT Triangle::computeRadius(
        const math::SMatrix44& mtxW2V,
        const math::SVector4& normal) const
    {
        auto radius = computeRadius(normal);
        return radius;
    }

    // ���S���W���擾.
    const math::SVector4 Triangle::getCenter() const
    {
        // NOTE
        // �O�S���W�̏ؖ�.
        // https://www.google.co.jp/url?sa=t&rct=j&q=&esrc=s&source=web&cd=2&cad=rja&uact=8&ved=0ahUKEwjTvoa0zczOAhUCjpQKHdTBDkkQFggjMAE&url=http%3A%2F%2Fwww.jikkyo.co.jp%2Fcontents%2Fdownload%2F9992655233&usg=AFQjCNEa2crr-haymhuOdRSM5N87MSp-1w&bvm=bv.129759880,d.dGo

        // ��JBC�ɂ��čl����.
        // �_J�͉~�̒��S�ŁA�_A�͉~����ɂ���_�Ȃ̂ŁA�~���p�̒藝�ɂ��A���S�p = 2 * �~���p.
        // ��BJC = 2��BAC

        // �O�p�`�̖ʐς̌������
        // ��JBC = 1/2 * bc * sin��BJC
        //   b = c = R�i���a�j
        //   ��BJC = 2��BAC
        // p = ��JBC = 1/2 * R^2 * sin2A
        // q = 1/2 * R^2 * sin2B
        // r = 1/2 * R^2 * sin2C

        // p / (p + q + r) = (1/2 * R^2 * sin2A) / (1/2 * R^2 * (sin2A + sin2B + sin2C))
        //                 = sin2A / (sin2A + sin2B + sin2C)
        // q / (p + q + r) = sin2B / (sin2A + sin2B + sin2C)
        // r / (p + q + r) = sin2C / (sin2A + sin2B + sin2C)

        // NOTE
        //     pa + qb + rc   a * sin2A + b * sin2B + c * sin2C
        // o = ------------ = ----------------------------------
        //     p + q + r        sin2A + sin2B + sin2C

        math::CVector4 pt[3];
        math::SMatrix44::Apply(pt[0], m_pt[0], m_L2W);
        math::SMatrix44::Apply(pt[1], m_pt[1], m_L2W);
        math::SMatrix44::Apply(pt[2], m_pt[2], m_L2W);

        math::CVector4 nml;
        computePlane(
            pt[0], pt[1], pt[2],
            nml);

        const auto& a = pt[0];
        const auto& b = pt[1];
        const auto& c = pt[2];

        auto ab = b - a;
        auto ac = c - a;

        auto ba = -ab;
        auto bc = c - b;

        auto ca = -ac;
        auto cb = -bc;

        IZ_FLOAT cosA = math::SVector4::Dot(ab, ac);
        IZ_FLOAT cosB = math::SVector4::Dot(ba, bc);
        IZ_FLOAT cosC = math::SVector4::Dot(ca, cb);

        IZ_FLOAT sinA = sqrtf(1.0f - cosA * cosA);
        IZ_FLOAT sinB = sqrtf(1.0f - cosB * cosB);
        IZ_FLOAT sinC = sqrtf(1.0f - cosC * cosC);

        // ���@�藝.
        // sin(A + B) = sinAcosB + cosAsinB
        // -> sin2A = sin(A + A) = sinAcosA + cosAsinA = 2sinAcosA

        IZ_FLOAT sin2A = 2.0f * sinA * cosA;
        IZ_FLOAT sin2B = 2.0f * sinB * cosB;
        IZ_FLOAT sin2C = 2.0f * sinC * cosC;

        auto center = a * sin2A + b * sin2B + c * sin2C;
        center /= (sin2A + sin2B + sin2C);

        center.w = 1.0f;

        return std::move(center);
    }

    static inline IZ_BOOL IsIn(
        const math::CVector4& p,
        const math::CVector4& base,
        const math::CVector4& pos,
        const math::CVector4& normal,
        IZ_BOOL isFlipped = IZ_FALSE)
    {
        math::CVector4 d0 = p - base;

        math::CVector4 d1 = pos - base;

        math::SVector4 c;

        if (isFlipped) {
            math::SVector4::Cross(c, d0, d1);
        }
        else {
            math::SVector4::Cross(c, d1, d0);
        }

        IZ_FLOAT f = math::SVector4::Dot(c, normal);

        if (f < 0) {
            return IZ_FALSE;
        }

        return IZ_TRUE;
    }

    // Get if the ray is hit.
    IZ_BOOL Triangle::isHit(const Ray& ray, HitResult& res)
    {
        // NOTE
        // http://t-pot.com/program/93_RayTraceTriangle/index.html

        /*
            ���C�̕����� : p = x + t v
            �R�p�`�̍�镽�ʂ̕����� : (p - p0)�En = 0
            x  : ���C�̏o��_
            v  : ���C�̕���
            p0 : �R�p�`�̂P�_
            n  : �R�p�`�̖@���x�N�g��

            �A���������ʁA���C�̐i�񂾋���t��

                (x - p0)�En
            t = ------------
                    v�En

            v�En=0 �̎��́A���C�ƂR�p�`�̖ʂ����s�ɐi��ł���̂ŁA�����Ȃ�.
            t<0�̎��́A�����̌�둤�Ɍ�_������̂ŁA��͂����邱�Ƃ��Ȃ�.

            ��_���R�p�`�̓����ɂ��邩�ǂ����̔��������.

            ��_�ƂR�p�`�̓_�����ԃx�N�g��d0�ƁA�R�p�`�̂P��d1�Ƃ̊O�ς��v�Z
            �O�ςɂ���Ăł����x�N�g���́A�R�p�`�̖@���Ɠ������������΂̕���������.
            ���̌����́A��_���R�p�`�̒��ɂ��邩�O�ɂ��邩�ŕ������ς��̂ŁA�S�Ă̕ӂɊւ��āA�����������Ȃ�΁A��_�͂R�p�`�̒��ɂ��邱�ƂɂȂ�.
        */

        math::CVector4 pt[3];
        math::SMatrix44::Apply(pt[0], m_pt[0], m_L2W);
        math::SMatrix44::Apply(pt[1], m_pt[1], m_L2W);
        math::SMatrix44::Apply(pt[2], m_pt[2], m_L2W);

        math::CVector4 nml;
        computePlane(
            pt[0], pt[1], pt[2],
            nml);

#if 0
        // �����Ƃ̔���
        math::CVector4 xp = pt[0] - ray.p;

        float xpn = math::SVector4::Dot(xp, nml);
        float vn = math::SVector4::Dot(ray.v, nml);

        // �J�����O�Ɣ��U���O��
        if (-0.00001f <= vn) {
            return IZ_FALSE;
        }

        IZ_FLOAT t = xpn / vn;

        // �������̃��C�͖���
        if (t < 0) {
            return IZ_FALSE;
        }

        // �R�p�`�̕��ʂւ̎ˉe
        auto p = ray.p + t * ray.v;

        if (!IsIn(p, pt[0], pt[1], nml)) {
            return IZ_FALSE;
        }
        if (!IsIn(p, pt[1], pt[2], nml)) {
            return IZ_FALSE;
        }
        if (!IsIn(p, pt[2], pt[0], nml)) {
            return IZ_FALSE;
        }
#else
        math::CVector4 p;
        IZ_FLOAT t = 0.0f;

        IZ_BOOL hit = IZ_FALSE;

        hit = isHit(
            ray,
            pt, nml,
            IZ_FALSE,
            p, t);

        if (!hit && isBothSides()) {
            nml = -nml;
            hit = isHit(
                ray,
                pt, nml,
                IZ_TRUE,
                p, t);
        }

        if (!hit) {
            return IZ_FALSE;
        }
#endif

        res.pt = p;
        res.normal = nml;
        res.t = t;

        return IZ_TRUE;
    }

    IZ_BOOL Triangle::isHit(
        const Ray& ray, 
        const math::CVector4* pt,
        const math::CVector4& nml,
        IZ_BOOL isFlipped,
        math::CVector4& retP,
        IZ_FLOAT& retT)
    {
        // �����Ƃ̔���
        math::CVector4 xp = pt[0] - ray.p;

        float xpn = math::SVector4::Dot(xp, nml);
        float vn = math::SVector4::Dot(ray.v, nml);

        // �J�����O�Ɣ��U���O��
        if (-0.00001f <= vn) {
            return IZ_FALSE;
        }

        IZ_FLOAT t = xpn / vn;

        // �������̃��C�͖���
        if (t < 0) {
            return IZ_FALSE;
        }

        // �R�p�`�̕��ʂւ̎ˉe
        auto p = ray.p + t * ray.v;

        if (!IsIn(p, pt[0], pt[1], nml, isFlipped)) {
            return IZ_FALSE;
        }
        if (!IsIn(p, pt[1], pt[2], nml, isFlipped)) {
            return IZ_FALSE;
        }
        if (!IsIn(p, pt[2], pt[0], nml, isFlipped)) {
            return IZ_FALSE;
        }

        retP = p;
        retT = t;

        return IZ_TRUE;
    }
}   // namespace math
}   // namespace izanagi
