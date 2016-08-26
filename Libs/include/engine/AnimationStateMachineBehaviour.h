#if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_BEHAVIOUR_H__)
#define __IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_BEHAVIOUR_H__

#include "engine/StateMachineBehaviour.h"
#include "engine/AnimationStateMachineCondition.h"
#include "izAnimation.h"
#include "izSceneGraph.h"

namespace izanagi {
namespace engine {
    class AnimationStateMachineNode;

    /**
     */
    class AnimationStateMachineBehaviour : public StateMachineBehaviour {
        friend class AnimationStateMachineNode;
        friend class AnimationStateMachine;

    private:
        static AnimationStateMachineBehaviour* create(IMemoryAllocator* allocator);

    protected:
        AnimationStateMachineBehaviour();
        virtual ~AnimationStateMachineBehaviour();

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        virtual AnimationStateMachineCondition* addCondition(
            IMemoryAllocator* allocator,
            AnimationStateMachineConditionValue* target,
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp,
            const izanagi::CValue& threshold);

        void setTransitionTime(IZ_FLOAT t);

        // ノード間の遷移にかかる時間.
        IZ_FLOAT getTransitionTime() const;

        void setName(const char* name);

        const char* getName() const;

    protected:
        virtual StateMachineNode::State update(IZ_FLOAT delta) override;

        virtual StateMachineNode* next() override;

        void initAnimation();

        virtual void enter() override;
        virtual void exit() override;

        void setTarget(
            izanagi::CSkeletonInstance* skl,
            izanagi::CAnimationInterp* anm);

        virtual void setNode(
            AnimationStateMachineNode* from,
            AnimationStateMachineNode* to);

        AnimationStateMachineNode* getFrom();
        AnimationStateMachineNode* getTo();

    private:
        void setAnmInterpType(izanagi::CAnimationInterp::E_INTERP_TYPE type)
        {
            m_anmInterpType = type;
        }

        izanagi::CAnimationInterp::E_INTERP_TYPE getAnmInterpType() const
        {
            return m_anmInterpType;
        }

        izanagi::CStdList<AnimationStateMachineBehaviour>::Item* getListItem()
        {
            return &m_item;
        }

        const animation::CTimeline getTimeline() const
        {
            return m_timeline;
        }

        void disableSetOwnNextNode()
        {
            m_canSetOwnNextNode = IZ_FALSE;
        }

    protected:
        IMemoryAllocator* m_Allocator{ nullptr };

        animation::CTimeline m_timeline;

        AnimationStateMachineNode* m_from{ nullptr };
        AnimationStateMachineNode* m_to{ nullptr };

        izanagi::CSkeletonInstance* m_skl{ nullptr };
        izanagi::CAnimationInterp* m_anm{ nullptr };

        IZ_BOOL m_isInitAnm{ IZ_FALSE };

        izanagi::CStdList<AnimationStateMachineBehaviour>::Item m_item;

        CStdString<char, 31> m_tag;

        IZ_BOOL m_canSetOwnNextNode{ IZ_TRUE };

        izanagi::CAnimationInterp::E_INTERP_TYPE m_anmInterpType{ izanagi::CAnimationInterp::E_INTERP_TYPE::E_INTERP_TYPE_SMOOTH };
    };

    // TODO
    // 名前をどうするか.
    // アニメーション終了時に強制遷移.
    // アニメーションが終了するまでは無条件に待つ.
    /**
     */
    class AnimationStateMachineTransition : public AnimationStateMachineBehaviour {
        friend class AnimationStateMachine;

    private:
        AnimationStateMachineTransition() : m_handler(this) {}
        virtual ~AnimationStateMachineTransition() {}

    private:
        virtual AnimationStateMachineCondition* addCondition(
            IMemoryAllocator* allocator,
            AnimationStateMachineConditionValue* target,
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp,
            const izanagi::CValue& threshold) override
        {
            return nullptr;
        }

        virtual void setNode(
            AnimationStateMachineNode* from,
            AnimationStateMachineNode* to) override;

        virtual StateMachineNode::State update(IZ_FLOAT delta) override;

    private:
        class Handler : public animation::CTimeline::CTimeOverHandler {
        public:
            Handler(AnimationStateMachineTransition* p) : m_behaviour(p) {}
            virtual ~Handler() {}
            virtual void Handle(const animation::CTimeline& timeline) override;

            AnimationStateMachineTransition* m_behaviour;
        };

        Handler m_handler;

        IZ_BOOL m_isFinishFromNode{ IZ_FALSE };
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_ANIIMATION_STATE_MACHINE_BEHAVIOUR_H__)
