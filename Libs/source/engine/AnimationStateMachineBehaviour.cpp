#include"engine/AnimationStateMachineBehaviour.h"
#include"engine/AnimationStateMachineNode.h"
#include"engine/AnimationStateMachine.h"

namespace izanagi {
namespace engine {
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
        SAFE_RELEASE(m_skl);
        SAFE_RELEASE(m_anm);
    }

    void AnimationStateMachineBehaviour::setTransitionTime(IZ_FLOAT t)
    {
        IZ_ASSERT(m_to);

        // ‘JˆÚæƒAƒjƒ‚ÌŽžŠÔ‚ð’´‚¦‚È‚¢.
        auto anm = m_to->getAnimation();
        if (anm) {
            auto anmTime = anm->GetAnimationTime();
            t = (t > anmTime ? anmTime : t);
        }

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

    void AnimationStateMachineBehaviour::setName(const char* name)
    {
        m_tag = name;
    }

    const char* AnimationStateMachineBehaviour::getName() const
    {
        return m_tag.GetString();
    }

    AnimationStateMachineCondition* AnimationStateMachineBehaviour::addCondition(
        IMemoryAllocator* allocator,
        AnimationStateMachineConditionValue* target,
        StateMachineCondition::Type type,
        StateMachineCondition::Cmp cmp,
        const izanagi::CValue& threshold)
    {
        IZ_ASSERT(target);
        IZ_ASSERT(target->getNameLength() > 0);

        auto item = m_conditions.GetTop();

        while (item) {
            auto cond = item->GetData();

            if (cond->isSame(target->getName())) {
                return (AnimationStateMachineCondition*)cond;
            }

            item = item->GetNext();
        }

        IZ_BOOL result = IZ_FALSE;

        auto cond = AnimationStateMachineCondition::create(
            allocator,
            type, cmp, threshold);
        VGOTO(result = (cond != nullptr), __EXIT__);

        cond->setTargetValue(target);

        result = StateMachineBehaviour::addCondition(cond);

    __EXIT__:
        if (!result) {
            SAFE_RELEASE(cond);
        }

        return cond;
    }

    StateMachineNode::State AnimationStateMachineBehaviour::update(IZ_FLOAT delta)
    {
        auto state = StateMachineBehaviour::update(delta);

        if (state == State::Running) {
            if (m_timeline.GetDuration() == 0.0f) {
                // ‘JˆÚŽžŠÔ‚ªŽw’è‚³‚ê‚Ä‚¢‚È‚¢ = –³ðŒ‚Å‘JˆÚ.
                m_state = State::Exit;
                return State::None;
            }
            else {
                auto f = m_timeline.GetNormalized();
                if (f >= 1.0f) {
                    // ‘JˆÚŽžŠÔ‚ð’´‚¦‚½‚Ì‚Å”²‚¯‚é.
                    m_state = State::Exit;
                    return State::None;
                }

                initAnimation();

                IZ_FLOAT t = m_timeline.GetTime();

                // TODO
                // time ‚ª duration ‚ð’´‚¦‚½‚Æ‚«.

                if (AnimationStateMachine::StateHandler) {
                    AnimationStateMachine::StateHandler(
                        getName(),
                        t,
                        m_timeline.GetDuration());
                }

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
                m_anmInterpType,
                fromAnm,
                startTime,
                toAnm,
                0.0f);

            m_isInitAnm = IZ_TRUE;

            m_from->notifyInitializedInBehavour();
            m_to->notifyInitializedInBehavour();
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

        // ŽŸ‚Ìƒm[ƒh‚ÌƒAƒjƒ[ƒVƒ‡ƒ“‚ÉŒ»Ý‚ÌŽžŠÔ‚ðˆø‚«“n‚·‚½‚ß‚ÉƒrƒwƒCƒrƒA‚ð‹³‚¦‚é.
        if (m_to && m_canSetOwnNextNode) {
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
        m_from = from;
        m_to = to;
    }

    AnimationStateMachineNode* AnimationStateMachineBehaviour::getFrom()
    {
        return m_from;
    }

    AnimationStateMachineNode* AnimationStateMachineBehaviour::getTo()
    {
        return m_to;
    }

    ////////////////////////////////////////////////////////////////////////

    void AnimationStateMachineTransition::Handler::Handle(const animation::CTimeline& timeline)
    {
        IZ_ASSERT(m_behaviour);

        m_behaviour->m_isFinishFromNode = IZ_TRUE;
    }

    void AnimationStateMachineTransition::setNode(
        AnimationStateMachineNode* from,
        AnimationStateMachineNode* to)
    {
        m_from = from;
        m_to = to;

        if (m_from) {
            m_from->m_timeline.SetTimeOverHandler(&m_handler);
        }
    }

    StateMachineNode::State AnimationStateMachineTransition::update(IZ_FLOAT delta)
    {
        IZ_ASSERT(m_from);

        if (m_isFinishFromNode) {
            m_isFinishFromNode = IZ_FALSE;
            m_state = State::Enter;
            return State::Enter;
        }
        else {
            if (m_state == State::Enter) {
                m_state = State::None;
                return State::Exit;
            }
            else {
                return State::Running;
            }
        }
    }
}
}