#if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_CONDITION_H__)
#define __IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_CONDITION_H__

#include "engine/StateMachineCondition.h"

namespace izanagi {
namespace engine {
    class AnimationStateMachineNode;

    /**
     */
    class AnimationStateMachineConditionValue : public StateMachineConditionValue {
        friend class AnimationStateMachineBehaviour;
        friend class AnimationStateMachine;

    private:
        static AnimationStateMachineConditionValue* create(
            IMemoryAllocator* allocator,
            const char* name);

    private:
        AnimationStateMachineConditionValue();
        virtual ~AnimationStateMachineConditionValue() {}

    private:
        izanagi::CStdList<AnimationStateMachineConditionValue>::Item* getListItem()
        {
            return &m_item;
        }

    private:
        izanagi::CStdList<AnimationStateMachineConditionValue>::Item m_item;
    };

    /**
     */
    class AnimationStateMachineCondition : public StateMachineCondition {
        friend class AnimationStateMachineBehaviour;

    private:
        static AnimationStateMachineCondition* create(
            IMemoryAllocator* allocator,
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp,
            const izanagi::CValue& threshold);

        // ‚Ó‚³‚®‚½‚ß.
        IZ_BOOL setTargetValue(StateMachineConditionValue* target);

    private:
        AnimationStateMachineCondition() {}
        virtual ~AnimationStateMachineCondition() {}
    };

}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_BEHAVIOUR_H__)
