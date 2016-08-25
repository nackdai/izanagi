#include"engine/AnimationStateMachineCondition.h"

namespace izanagi {
namespace engine {

    AnimationStateMachineConditionValue* AnimationStateMachineConditionValue::create(
        IMemoryAllocator* allocator,
        const char* name)
    {
        void* buf = ALLOC(allocator, sizeof(AnimationStateMachineConditionValue));
        VRETURN_NULL(buf);

        AnimationStateMachineConditionValue* value = new(buf)AnimationStateMachineConditionValue;
        {
            value->m_Allocator = allocator;
            value->setName(name);

            value->AddRef();
        }

        return value;
    }

    AnimationStateMachineConditionValue::AnimationStateMachineConditionValue()
    {
        m_item.Init(this);
    }

    /////////////////////////////////////////////////////////////

    AnimationStateMachineCondition* AnimationStateMachineCondition::create(
        IMemoryAllocator* allocator,
        StateMachineCondition::Type type,
        StateMachineCondition::Cmp cmp,
        const izanagi::CValue& threshold)
    {
        void* buf = ALLOC(allocator, sizeof(AnimationStateMachineCondition));
        VRETURN_NULL(buf);

        AnimationStateMachineCondition* cond = new(buf)AnimationStateMachineCondition;
        {
            cond->m_Allocator = allocator;
            cond->set(type, cmp, threshold);

            cond->AddRef();
        }

        return cond;
    }

    IZ_BOOL AnimationStateMachineCondition::setTargetValue(StateMachineConditionValue* target)
    {
        return StateMachineCondition::setTargetValue(target);
    }
}
}