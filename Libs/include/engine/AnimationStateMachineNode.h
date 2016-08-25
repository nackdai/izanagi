#if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_NODE_H__)
#define __IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_NODE_H__

#include "engine/StateMachineNode.h"
#include "engine/AnimationStateMachineBehaviour.h"
#include "izAnimation.h"

namespace izanagi {
namespace engine {
    /**
     */
    class AnimationStateMachineNode :public StateMachineNode {
        friend class AnimationStateMachine;
        friend class AnimationStateMachineBehaviour;

    private:
        static AnimationStateMachineNode* create(
            IMemoryAllocator* allocator,
            const char* name);

    private:
        AnimationStateMachineNode();
        virtual ~AnimationStateMachineNode();

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        using Name = izanagi::CStdString < IZ_CHAR, 15 >;

        using HookFunc = std::function<void(void)>;

        const Name& getName() const;

        void setName(const char* name);

        void setAnimation(
            izanagi::IAnimation* anm,
            IZ_BOOL isLoopAnm);

        virtual izanagi::IAnimation* getAnimation();

    private:
        IZ_BOOL addBehaviour(
            AnimationStateMachineBehaviour* behaviour,
            AnimationStateMachineNode* to);

        IZ_BOOL removeBehaviour(AnimationStateMachineBehaviour* behaviour);
        IZ_BOOL removeBehaviour(AnimationStateMachineNode* to);

        AnimationStateMachineBehaviour* getBehaviour(AnimationStateMachineNode* to);

        virtual State update(IZ_FLOAT delta) override;

        virtual StateMachineNode* next() override;

        virtual void enter() override;
        virtual void exit() override;

        IZ_BOOL updateAnimation(IZ_FLOAT delta);

        void setTarget(izanagi::CSkeletonInstance* skl);

        const animation::CTimeline& getTimeline() const
        {
            return m_timeline;
        }

        izanagi::CKey& getKey()
        {
            return m_key;
        }

        izanagi::CStdList<AnimationStateMachineNode>::Item* getListItem()
        {
            return &m_item;
        }

        void setFromBehavour(AnimationStateMachineBehaviour* behaviour)
        {
            m_fromBehaviour = behaviour;
        }

    private:
        IMemoryAllocator* m_Allocator{ nullptr };

        AnimationStateMachineNode::Name m_name;
        izanagi::CKey m_key;

        HookFunc m_enterFunc{ nullptr };
        HookFunc m_exitFunc{ nullptr };

        izanagi::CSkeletonInstance* m_skl{ nullptr };
        izanagi::IAnimation* m_anm{ nullptr };

        IZ_BOOL m_isLoopAnm{ IZ_TRUE };

        izanagi::CStdList<AnimationStateMachineBehaviour> m_behaviours;

        izanagi::CStdList<AnimationStateMachineNode>::Item m_item;

        AnimationStateMachineBehaviour* m_currentBehaviour{ nullptr };

        AnimationStateMachineBehaviour* m_fromBehaviour{ nullptr };

        animation::CTimeline m_timeline;
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_NODE_H__)
