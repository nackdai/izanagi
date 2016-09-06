#include "engine/TargetFollowCamera.h"

namespace izanagi {
namespace engine {
    void TargetFollowCamera::setTarget(
        Follow type,
        ICameraTarget* target,
        const math::CVector3& posOffset,
        const math::CVector3& atOffset)
    {
        IZ_ASSERT(target);

        m_followType = type;

        m_target = target;

        auto targetPos = m_target->getPos();
        auto targetDir = m_target->getDir();

        m_posOffset = posOffset;
        m_atOffset = atOffset;

        auto camPos = targetPos + m_posOffset;
        auto camAt = targetPos + m_atOffset;

        SetPos(math::CVector4(camPos.x, camPos.y, camPos.z, 1.0f));
        SetAt(math::CVector4(camAt.x, camAt.y, camAt.z, 1.0f));
    }

    void TargetFollowCamera::setFollowType(Follow type)
    {
        m_followType = type;
    }

    TargetFollowCamera::Follow TargetFollowCamera::getFollowType() const
    {
        return m_followType;
    }

    // NOTE
    // http://memeplex.blog.shinobi.jp/xna/%E3%83%90%E3%83%8D%E4%BB%98%E3%81%8D%E3%82%AB%E3%83%A1%E3%83%A9%E3%80%80%E3%81%9D%E3%81%AE%EF%BC%92%EF%BC%88%E3%82%BD%E3%83%BC%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%89%EF%BC%89
    // http://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q1378767193
    // http://jsciencer.com/daigakubuturicate/syotourikigaku/4351/
    // http://toshi1.civil.saga-u.ac.jp/aramakig/text/j1.pdf

    void TargetFollowCamera::Update(IZ_FLOAT elapsed)
    {
        if (m_target) {
            auto targetPos = m_target->getPos();
            auto targetDir = m_target->getDir();

            auto transform = targetDir.AsMatrix();

            auto posOffset = m_posOffset;

            if (m_followType == Follow::Back) {
                posOffset *= transform;
            }

            // —‘z‚ÌƒJƒƒ‰ˆÊ’u.
            auto desiredPos = targetPos + posOffset;

            const auto& pos = GetParam().pos;
            izanagi::math::CVector3 position(pos.x, pos.y, pos.z);

            auto stretch = position - desiredPos;

            // ‚Î‚Ë‚ÌŒ¸Š‰^“®•û’öŽ®.
            // F = ma = -kx - cv
            //  x : ‚Î‚Ë‚ÌL‚Ñ.
            //  k : ‚Î‚Ë’è”.
            //  c : Œ¸ŠŒW”.
            //  v : ‘¬“x.

            auto force = -m_stiffness * stretch - m_damping * m_velocity;
            auto f = force.Length();

            if (f > 0.0f) {
                // ‰Á‘¬“x.
                auto accel = force / m_mass;

                // ‘¬“x.
                m_velocity += accel * elapsed;

                // ˆÚ“®—Ê.
                auto moveDistance = m_velocity * elapsed;

                position += moveDistance;

                SetPos(math::CVector4(position.x, position.y, position.z));

                if (m_followType == Follow::Back) {
                    auto atOffset = m_atOffset * transform;
                    auto desiredAt = targetPos + atOffset;
                    SetAt(math::CVector4(desiredAt.x, desiredAt.y, desiredAt.z));
                }
                else {
                    math::CVector4 at(GetParam().ref);
                    at += moveDistance;
                    SetAt(at);
                }
            }
        }

        CVectorCamera::Update(elapsed);
    }
}
}