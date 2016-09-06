#if !defined(__IZANAGI_ENGINE_TARGET_FOLLOW_CAMERA_H__)
#define __IZANAGI_ENGINE_TARGET_FOLLOW_CAMERA_H__

#include "izStd.h"
#include "izMath.h"
#include "izSceneGraph.h"

namespace izanagi {
namespace engine {
    class ICameraTarget {
    protected:
        ICameraTarget() {}
        virtual ~ICameraTarget() {}

    public:
        PURE_VIRTUAL(math::CVector3 getPos() const);

        PURE_VIRTUAL(math::CQuat getDir() const);
    };

    class TargetFollowCamera : public CVectorCamera {
    public:
        TargetFollowCamera() {}
        virtual ~TargetFollowCamera() {}

    public:
        void setTarget(
            ICameraTarget* target,
            const math::CVector3& posOffset,
            const math::CVector3& atOffset);

        virtual void Update(IZ_FLOAT elapsed) override;

    protected:
        ICameraTarget* m_target{ nullptr };

        math::CVector3 m_prevTargetPos;
        math::CQuat m_prevTargetDir;

        math::CVector3 m_posOffset;
        math::CVector3 m_atOffset;

        IZ_FLOAT m_radius{ 0.0f };
    };

    class SpringArmedCamera : public TargetFollowCamera {
    public:
        SpringArmedCamera() {}
        virtual ~SpringArmedCamera() {}

    public:
        virtual void Update(IZ_FLOAT elapsed) override;

        const math::CVector3& getVelocity() const
        {
            return m_velocity;
        }

    protected:
        // ÇŒÇÀíËêî.
        IZ_FLOAT m_springRate{ 1800.0f };

        // å∏êäåWêî.
        IZ_FLOAT m_damping{ 600.0f };

        math::CVector3 m_velocity;

        // éøó .
        IZ_FLOAT m_mass{ 50.0f };

        math::CVector3 m_sum;
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_CHARACTER_CONTROLLER_H__)

