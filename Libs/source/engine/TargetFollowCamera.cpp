#include "engine/TargetFollowCamera.h"

namespace izanagi {
namespace engine {
    void TargetFollowCamera::setTarget(
        ICameraTarget* target,
        const math::CVector3& posOffset,
        const math::CVector3& atOffset)
    {
        m_target = target;

        if (m_target) {
            m_prevTargetPos = m_target->getPos();
            m_prevTargetDir = m_target->getDir();
        }

        m_posOffset = posOffset;
        m_atOffset = atOffset;

        auto camPos = m_prevTargetPos + m_posOffset;
        auto camAt = m_prevTargetPos + m_atOffset;

        SetPos(math::CVector4(camPos.x, camPos.y, camPos.z, 1.0f));
        SetAt(math::CVector4(camAt.x, camAt.y, camAt.z, 1.0f));

        m_radius = math::SVector3::Length2(camPos, camAt);
    }

    void TargetFollowCamera::Update(IZ_FLOAT elapsed)
    {
        if (m_target) {
            auto curTargetPos = m_target->getPos();
            auto diff = curTargetPos - m_prevTargetPos;

            izanagi::math::CVector4 camPos(GetParam().pos);
            camPos += diff;
            SetPos(camPos);

            izanagi::math::CVector4 camAt(GetParam().ref);
            camAt += diff;
            SetAt(camAt);

            m_prevTargetPos = curTargetPos;
        }

        CVectorCamera::Update(elapsed);
    }

    // NOTE
    // http://memeplex.blog.shinobi.jp/xna/%E3%83%90%E3%83%8D%E4%BB%98%E3%81%8D%E3%82%AB%E3%83%A1%E3%83%A9%E3%80%80%E3%81%9D%E3%81%AE%EF%BC%92%EF%BC%88%E3%82%BD%E3%83%BC%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89%EF%BC%89
    // http://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q1378767193
    // http://jsciencer.com/daigakubuturicate/syotourikigaku/4351/
    // http://toshi1.civil.saga-u.ac.jp/aramakig/text/j1.pdf

    void SpringArmedCamera::Update(IZ_FLOAT elapsed)
    {
        if (m_target) {
            auto targetPos = m_target->getPos();
            auto targetDir = m_target->getDir();

#if 0
            auto transform = targetDir.AsMatrix();

            auto posOffset = m_posOffset * transform;
#else
            auto posOffset = m_posOffset;
#endif

            // 理想のカメラ位置.
            auto desiredPos = targetPos + posOffset;

            const auto& pos = GetParam().pos;
            izanagi::math::CVector3 position(pos.x, pos.y, pos.z);

            auto stretch = position - desiredPos;

            // ばねの減衰運動方程式.
            // F = ma = -kz - cv

            auto force = -m_springRate * stretch - m_damping * m_velocity;
            auto f = force.Length();

            // 加速度.
            auto accel = force / m_mass;

            // 速度.
            m_velocity += accel * elapsed;

            // 移動量.
            auto moveDistance = m_velocity * elapsed;

            position += moveDistance;

            SetPos(math::CVector4(position.x, position.y, position.z));

            math::CVector4 at(GetParam().ref);
            at += moveDistance;
            SetAt(at);

            m_prevTargetPos = targetPos;
        }

        CVectorCamera::Update(elapsed);
    }
}
}