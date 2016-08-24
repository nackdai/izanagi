#if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_BEHAVIOUR_H__)
#define __IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_BEHAVIOUR_H__

#include "engine/StateMachineBehaviour.h"
#include "izAnimation.h"
#include "izSceneGraph.h"

namespace izanagi {
namespace engine {
    class AnimationStateMachineNode;

    /**
     */
    class AniimationStateMachineCondition : public StateMachineCondition, public CObject {
        friend class AnimationStateMachineBehaviour;

    private:
        static AniimationStateMachineCondition* create(
            IMemoryAllocator* allocator,
            const char* name,
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp,
            const izanagi::CValue& value);

    private:
        AniimationStateMachineCondition() {}
        virtual ~AniimationStateMachineCondition() {}

        IZ_DEFINE_INTERNAL_RELEASE();

    private:
        IMemoryAllocator* m_Allocator{ nullptr };
    };

    /**
     */
    class AnimationStateMachineBehaviour : public StateMachineBehaviour, public CObject {
        friend class AnimationStateMachineNode;
        friend class AnimationStateMachine;

    public:
        static AnimationStateMachineBehaviour* create(IMemoryAllocator* allocator);

    private:
        AnimationStateMachineBehaviour();
        virtual ~AnimationStateMachineBehaviour();

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        void setTransitionTime(IZ_FLOAT t);

        // ÉmÅ[Éhä‘ÇÃëJà⁄Ç…Ç©Ç©ÇÈéûä‘.
        IZ_FLOAT getTransitionTime() const;

        AniimationStateMachineCondition* addCondition(
            IMemoryAllocator* allocator,
            const char* name,
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp,
            const izanagi::CValue& value);

        IZ_BOOL addCondition(AniimationStateMachineCondition* cond);

    private:
        virtual StateMachineNode::State update(IZ_FLOAT delta) override;

        virtual StateMachineNode* next() override;

        void initAnimation();

        virtual void enter() override;
        virtual void exit() override;

        void setTarget(
            izanagi::CSkeletonInstance* skl,
            izanagi::CAnimationInterp* anm);

        void setNode(
            AnimationStateMachineNode* from,
            AnimationStateMachineNode* to);

        AnimationStateMachineNode* getFrom();
        AnimationStateMachineNode* getTo();

    private:
        izanagi::CStdList<AnimationStateMachineBehaviour>::Item* getListItem()
        {
            return &m_item;
        }

        const animation::CTimeline getTimeline() const
        {
            return m_timeline;
        }

    private:
        IMemoryAllocator* m_Allocator{ nullptr };

        animation::CTimeline m_timeline;

        AnimationStateMachineNode* m_from{ nullptr };
        AnimationStateMachineNode* m_to{ nullptr };

        izanagi::CSkeletonInstance* m_skl{ nullptr };
        izanagi::CAnimationInterp* m_anm{ nullptr };

        IZ_BOOL m_isInitAnm{ IZ_FALSE };

        izanagi::CStdList<AnimationStateMachineBehaviour>::Item m_item;

        CStdString<char, 15> m_tag;
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_BEHAVIOUR_H__)
