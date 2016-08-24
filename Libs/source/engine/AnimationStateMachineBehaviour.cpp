#include"engine/AnimationStateMachineBehaviour.h"
#include"engine/AnimationStateMachineNode.h"

namespace izanagi {
namespace engine {
    AniimationStateMachineCondition* AniimationStateMachineCondition::create(
        IMemoryAllocator* allocator,
        const char* name,
        StateMachineCondition::Type type,
        StateMachineCondition::Cmp cmp,
        const izanagi::CValue& value)
    {
        void* buf = ALLOC(allocator, sizeof(AniimationStateMachineCondition));
        VRETURN_NULL(buf);

        AniimationStateMachineCondition* cond = new(buf)AniimationStateMachineCondition;
        {
            cond->m_Allocator = allocator;
            cond->set(name, type, cmp, value);

            cond->AddRef();
        }

        return cond;
    }

    ////////////////////////////////////////////////////////////////////

    AnimationStateMachineBehaviour* AnimationStateMachineBehaviour::create(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(AnimationStateMachineBehaviour));
        VRETURN_NULL(buf);

        AnimationStateMachineBehaviour* b = new(buf)AnimationStateMachineBehaviour;
        {
            b->m_Allocator = allocator;

            b->AddRef();
        }

        return b;
    }

    AnimationStateMachineBehaviour::AnimationStateMachineBehaviour()
    {
        m_item.Init(this);
    }

    AnimationStateMachineBehaviour::~AnimationStateMachineBehaviour()
    {
        SAFE_RELEASE(m_from);
        SAFE_RELEASE(m_to);

        SAFE_RELEASE(m_skl);
        SAFE_RELEASE(m_anm);
    }

    void AnimationStateMachineBehaviour::setTransitionTime(IZ_FLOAT t)
    {
        m_timeline.Init(t, 0.0f);
        m_timeline.EnableLoop(IZ_FALSE);
        m_timeline.AutoReverse(IZ_FALSE);
        m_timeline.Reset();
        m_timeline.Start();

        // TODO
        //m_timeline.SetTimeOverHandler()
    }

    IZ_FLOAT AnimationStateMachineBehaviour::getTransitionTime() const
    {
        auto ret = m_timeline.GetDuration();
        return ret;
    }

    IZ_FLOAT AnimationStateMachineBehaviour::getRatio()
    {
        auto ratio = m_timeline.GetNormalized();
        return ratio;
    }

    IZ_BOOL AnimationStateMachineBehaviour::isTransition()
    {
        auto ret = (m_state == StateMachineNode::State::Enter
            || m_state == StateMachineNode::State::Running);

        return ret;
    }

    AniimationStateMachineCondition* AnimationStateMachineBehaviour::addCondition(
        IMemoryAllocator* allocator,
        const char* name,
        StateMachineCondition::Type type,
        StateMachineCondition::Cmp cmp,
        const izanagi::CValue& value)
    {
        auto cond = getCondition(name);

        if (cond) {
            return (AniimationStateMachineCondition*)cond;
        }

        cond = AniimationStateMachineCondition::create(
            allocator,
            name, type, cmp, value);
        VRETURN_NULL(cond);

        auto result = StateMachineBehaviour::addCondition(cond, IZ_TRUE);

        if (!result) {
            AniimationStateMachineCondition* c = (AniimationStateMachineCondition*)cond;
            SAFE_RELEASE(c);
            cond = nullptr;
        }

        return (AniimationStateMachineCondition*)cond;
    }

    IZ_BOOL AnimationStateMachineBehaviour::addCondition(AniimationStateMachineCondition* cond)
    {
        auto ret = StateMachineBehaviour::addCondition(cond, IZ_TRUE);
        
        if (ret) {
            cond->AddRef();
        }

        return ret;
    }

    StateMachineNode::State AnimationStateMachineBehaviour::update(IZ_FLOAT delta)
    {
        auto state = StateMachineBehaviour::update(delta);

        if (state == State::Running) {
            if (m_timeline.GetDuration() == 0.0f) {
                m_state = State::Exit;
                return State::None;
            }
            else {
                auto f = m_timeline.GetNormalized();
                if (f >= 1.0f) {
                    m_state = State::Exit;
                    return State::None;
                }

                initAnimation();

                IZ_FLOAT t = m_timeline.GetTime();

                if (m_anm && m_skl) {
                    m_skl->ApplyAnimation(t, m_anm);
                }

                m_timeline.Advance(delta);
            }
        }

        return state;
    }

    StateMachineNode* AnimationStateMachineBehaviour::next()
    {
        return m_to;
    }

    void AnimationStateMachineBehaviour::initAnimation()
    {
        if (!m_isInitAnm && m_anm) {
            auto fromAnm = m_from->getAnimation();
            auto toAnm = m_to->getAnimation();

            auto startTime = m_from->getTimeline().GetTime();

            m_anm->SetAnimation(
                m_timeline.GetDuration(),
                izanagi::CAnimationInterp::E_INTERP_TYPE::E_INTERP_TYPE_SMOOTH,
                fromAnm,
                startTime,
                toAnm,
                0.0f);

            m_isInitAnm = IZ_TRUE;
        }
    }

    void AnimationStateMachineBehaviour::enter()
    {
        StateMachineNode::enter();

        m_timeline.Reset();
        m_timeline.Start();
    }

    void AnimationStateMachineBehaviour::exit()
    {
        StateMachineNode::exit();

        // 次のノードのアニメーションに現在の時間を引き渡すためにビヘイビアを教える.
        if (m_to) {
            m_to->setFromBehavour(this);
        }

        SAFE_RELEASE(m_skl);
        SAFE_RELEASE(m_anm);

        m_isInitAnm = IZ_FALSE;
    }

    void AnimationStateMachineBehaviour::setTarget(
        izanagi::CSkeletonInstance* skl,
        izanagi::CAnimationInterp* anm)
    {
        SAFE_REPLACE(m_skl, skl);
        SAFE_REPLACE(m_anm, anm);
    }

    void AnimationStateMachineBehaviour::setNode(
        AnimationStateMachineNode* from,
        AnimationStateMachineNode* to)
    {
        SAFE_REPLACE(m_from, from);
        SAFE_REPLACE(m_to, to);
    }

    AnimationStateMachineNode* AnimationStateMachineBehaviour::getFrom()
    {
        return m_from;
    }

    AnimationStateMachineNode* AnimationStateMachineBehaviour::getTo()
    {
        return m_to;
    }

    void AnimationStateMachineBehaviour::releaseDelegatedContiditon(StateMachineCondition* cond)
    {
        if (cond) {
            AniimationStateMachineCondition* c = (AniimationStateMachineCondition*)cond;
            SAFE_RELEASE(c);
        }
    }
}
}