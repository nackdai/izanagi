#if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_H__)
#define __IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_H__

#include "engine/AnimationStateMachineNode.h"
#include "engine/AnimationStateMachineCondition.h"

namespace izanagi {
namespace engine {

    class AnimationStateMachine : public AnimationStateMachineNode {
    public:
        static AnimationStateMachine* create(IMemoryAllocator* allocator);

        static std::function<void(const char*, IZ_FLOAT, IZ_FLOAT)> StateHandler;

    private:
        AnimationStateMachine();
        ~AnimationStateMachine();

    public:
        AnimationStateMachineNode* addNode(
            const char* name,
            izanagi::IAnimation* anm,
            IZ_BOOL isLoopAnm = IZ_TRUE);

        AnimationStateMachineNode* getNode(const char* name);

        AnimationStateMachineBehaviour* addBehaviour(
            const char* from,
            const char* to,
            izanagi::CAnimationInterp::E_INTERP_TYPE interpType = izanagi::CAnimationInterp::E_INTERP_TYPE::E_INTERP_TYPE_SMOOTH);

        AnimationStateMachineBehaviour* addBehaviour(
            AnimationStateMachineNode* from,
            AnimationStateMachineNode* to,
            izanagi::CAnimationInterp::E_INTERP_TYPE interpType = izanagi::CAnimationInterp::E_INTERP_TYPE::E_INTERP_TYPE_SMOOTH);

        AnimationStateMachineConditionValue* addConditionValue(const char* name);

        IZ_BOOL setConditionValue(
            const char* name,
            const izanagi::CValue& value);

        AnimationStateMachineBehaviour* getBehaviour(
            const char* from,
            const char* to);

        AnimationStateMachineBehaviour* getBehaviour(
            AnimationStateMachineNode* from,
            AnimationStateMachineNode* to);

        IZ_BOOL removeBehaviour(
            const char* from,
            const char* to);

        IZ_BOOL removeBehaviour(
            AnimationStateMachineNode* from,
            AnimationStateMachineNode* to);

        IZ_BOOL removeBehaviour(AnimationStateMachineBehaviour* behaviour);

        IZ_BOOL removeNode(const char* name);

        IZ_BOOL removeNode(AnimationStateMachineNode* node);

        IZ_BOOL isRegistered(AnimationStateMachineNode* node);

        void update(
            IZ_FLOAT delta,
            izanagi::CSkeletonInstance* skl);

        virtual izanagi::IAnimation* getAnimation() override;

        virtual void notifyInitializedInBehavour() override;

    private:
        virtual StateMachineNode::State update(IZ_FLOAT delta) override;

        virtual void setFromBehavour(AnimationStateMachineBehaviour* behaviour) override;

        virtual StateMachineNode* next() override;

        virtual const animation::CTimeline& getTimeline() override;

        AnimationStateMachineNode* getNode();

    private:
        AnimationStateMachineNode m_entry;
        AnimationStateMachineNode m_exit;

        StateMachineNode* m_current{ nullptr };

        AnimationStateMachineBehaviour m_fromEntryTo;
        AnimationStateMachineTransition m_toExitFrom;

        izanagi::CAnimationInterp* m_blendAnm{ nullptr };

        izanagi::CStdList<AnimationStateMachineNode> m_nodes;
        izanagi::CStdList<AnimationStateMachineConditionValue> m_values;
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_H__)
