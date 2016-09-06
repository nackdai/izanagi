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
        enum Follow {
            Back,
            AsIs,
        };

        void setTarget(
            Follow type,
            ICameraTarget* target,
            const math::CVector3& posOffset,
            const math::CVector3& atOffset);

        void setFollowType(Follow type);

        Follow getFollowType() const;

        virtual void Update(IZ_FLOAT elapsed) override;

        const math::CVector3& getVelocity() const
        {
            return m_velocity;
        }

    protected:
        ICameraTarget* m_target{ nullptr };

        Follow m_followType{ Follow::AsIs };

        math::CVector3 m_posOffset;
        math::CVector3 m_atOffset;
        
        // �΂˒萔.
        IZ_FLOAT m_stiffness{ 1800.0f };

        // �����W��.
        IZ_FLOAT m_damping{ 600.0f };

        math::CVector3 m_velocity;

        // ����.
        IZ_FLOAT m_mass{ 50.0f };
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_CHARACTER_CONTROLLER_H__)

