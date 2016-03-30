#include "collision/ColFrustum.h"
#include "collision/ColBoundingVolume.h"

namespace izanagi
{
namespace col
{
    Frustum::Frustum()
    {
        // NOTE
        // http://ruh.li/CameraViewFrustum.html

        // Clip���W�n�ł�ViewFrustum�̖�.

        m_planesInClipCoord[Plane::Left].Set(1.0f, 0.0f, 0.0f, 1.0f);
        m_planesInClipCoord[Plane::Right].Set(-1.0f, 0.0f, 0.0f, 1.0f);

        m_planesInClipCoord[Plane::Top].Set(0.0f, -1.0f, 0.0f, 1.0f);
        m_planesInClipCoord[Plane::Bottom].Set(0.0f, 1.0f, 0.0f, 1.0f);

        m_planesInClipCoord[Plane::Near].Set(0.0f, 0.0f, 1.0f, 0.0f);
        m_planesInClipCoord[Plane::Far].Set(0.0f, 0.0f, -1.0f, 1.0f);
    }

    Frustum::Frustum(const Frustum& rhs)
    {
        *this = rhs;
    }

    const Frustum& Frustum::operator = (const Frustum& rhs)
    {
        memcpy(m_planes, rhs.m_planes, sizeof(m_planes));
        return *this;
    }

    void Frustum::initialize(
        IZ_FLOAT fNear,
        IZ_FLOAT fFar,
        IZ_FLOAT verticalFOV,
        IZ_FLOAT aspect)
    {
        // NOTE
        // http://ramblingsofagamedevstudent.blogspot.jp/2013/01/graphics-programming-frustum-culling.html
        // OpenGL�x�[�X�i�E����W�n�j�Ȃ̂ŁAZ�̌����𔽑΂ɂ���K�v������.

        // NOTE
        // ��p�������ł͂Ȃ��A�����Ȃ̂ŁA�A�X�y�N�g��� w/h �ƂȂ��Ă���.
        // ���̂��߁A3D�O���t�B�b�N�X���w�Ƃ͏c�A���̖ʂ̎�������ւ��.

        IZ_FLOAT e = 1.0f / ::tanf(verticalFOV * 0.5f);

        m_planes[Plane::Near].Set(0.0f, 0.0f, 1.0f, -fNear);
        m_planes[Plane::Far].Set(0.0f, 0.0f, -1.0f, fFar);

        //IZ_FLOAT normalizedFactor = 1.0f / sqrtf(e * e + 1);
        IZ_FLOAT normalizedFactor = 1.0f / sqrtf(e * e + aspect * aspect);

        m_planes[Plane::Left].Set(e * normalizedFactor, 0.0f, aspect * normalizedFactor, 0.0f);
        m_planes[Plane::Right].Set(-e * normalizedFactor, 0.0f, aspect * normalizedFactor, 0.0f);

        //normalizedFactor = 1.0f / sqrtf(e * e + aspect * aspect);
        normalizedFactor = 1.0f / sqrtf(e * e + 1);

        //m_planes[Plane::Top].Set(0.0f, -e * normalizedFactor, aspect * normalizedFactor, 0.0f);
        //m_planes[Plane::Bottom].Set(0.0f, e * normalizedFactor, aspect * normalizedFactor, 0.0f);
        m_planes[Plane::Top].Set(0.0f, -e * normalizedFactor, normalizedFactor, 0.0f);
        m_planes[Plane::Bottom].Set(0.0f, e * normalizedFactor, normalizedFactor, 0.0f);

        math::SVector4::Normalize(m_planes[Plane::Left], m_planes[Plane::Left]);
        math::SVector4::Normalize(m_planes[Plane::Right], m_planes[Plane::Right]);
        math::SVector4::Normalize(m_planes[Plane::Top], m_planes[Plane::Top]);
        math::SVector4::Normalize(m_planes[Plane::Bottom], m_planes[Plane::Bottom]);
    }

    void Frustum::initialize(
        Plane idx,
        const math::SVector4& plane)
    {
        IZ_ASSERT(idx < Plane::Num);

        m_planesInClipCoord[idx].Set(plane);
    }

    void Frustum::initialize(const math::SVector4 plane[Plane::Num])
    {
        for (IZ_UINT i = 0; i < Plane::Num; i++) {
            initialize((Plane)i, plane[i]);
        }
    }

    // �o�E���f�B���O�{�b�N�X�Ƃ̌�������.
    IZ_BOOL Frustum::isIntersect(
        const BoundingVolume* const volume,
        const math::SMatrix44& mtxW2V)
    {
        // �{�b�N�X�̒��S���擾.
        auto center = volume->getCenter();

        // �J�������W�n�ɕϊ�.
        math::SMatrix44::Apply(center, center, mtxW2V);

        for (IZ_UINT i = 0; i < COUNTOF(m_planes); i++) {
            math::CVector4 nml(
                m_planes[i].x,
                m_planes[i].y,
                m_planes[i].z,
                0.0f);

            // �������a���v�Z.
            auto radius = volume->computeRadius(mtxW2V, nml);

            auto dot = math::SVector4::Dot(m_planes[i], center);

            if (dot <= -radius) {
                return IZ_FALSE;
            }
        }

        return IZ_TRUE;
    }

    void Frustum::computePlane(const math::SMatrix44& mtxW2C)
    {
        // Clip���W�n�̖ʂ�World���W�n�ɕϊ�����.

        math::SMatrix44 mtxTransposeW2C;
        math::SMatrix44::Transpose(mtxTransposeW2C, mtxW2C);

        for (IZ_UINT i = 0; i < Plane::Num; i++) {
            math::SMatrix44::Apply(m_planes[i], m_planesInClipCoord[i], mtxTransposeW2C);

            auto length = math::SVector3::Length((const math::SVector3&)m_planes[i]);
            math::SVector4::Div(m_planes[i], m_planes[i], length);
        }
    }

    // �o�E���f�B���O�{�b�N�X�Ƃ̌�������.
    IZ_BOOL Frustum::isIntersect(const BoundingVolume* const volume)
    {
        // �{�b�N�X�̒��S���擾.
        auto center = volume->getCenter();

        // World���W�n�Ŕ��菈�����s��.

        for (IZ_UINT i = 0; i < COUNTOF(m_planes); i++) {
            math::CVector4 nml(
                m_planes[i].x,
                m_planes[i].y,
                m_planes[i].z,
                0.0f);

            // �������a���v�Z.
            auto radius = volume->computeRadius(nml);

            auto dot = math::SVector4::Dot(m_planes[i], center);

            if (dot <= -radius) {
                return IZ_FALSE;
            }
        }

        return IZ_TRUE;
    }
}   // namespace math
}   // namespace izanagi
