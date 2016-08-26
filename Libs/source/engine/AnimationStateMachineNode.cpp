#include"engine/AnimationStateMachineNode.h"
#include"engine/AnimationStateMachine.h"

namespace izanagi {
namespace engine {
    AnimationStateMachineNode* AnimationStateMachineNode::create(
        IMemoryAllocator* allocator,
        const char* name)
    {
        void* buf = ALLOC(allocator, sizeof(AnimationStateMachineNode));
        VRETURN_NULL(buf);

        AnimationStateMachineNode* ret = new(buf)AnimationStateMachineNode;
        {
            ret->m_Allocator = allocator;
            ret->setName(name);

            ret->AddRef();
        }

        return ret;
    }

    AnimationStateMachineNode::AnimationStateMachineNode()
    {
        m_item.Init(this);

        m_state = StateMachineNode::State::Enter;
    }

    AnimationStateMachineNode::~AnimationStateMachineNode()
    {
        SAFE_RELEASE(m_skl);
        SAFE_RELEASE(m_anm);

        auto item = m_behaviours.GetTop();

        while (item) {
            auto b = item->GetData();

            item = item->GetNext();

            SAFE_RELEASE(b);
        }
    }

    const AnimationStateMachineNode::Name& AnimationStateMachineNode::getName() const
    {
        return m_name;
    }

    void AnimationStateMachineNode::setName(const char* name)
    {
        IZ_ASSERT(strlen(name) <= m_name.GetLen());
        m_name = name;
        m_key.SetValue(m_name.GetKeyValue());
    }

    void AnimationStateMachineNode::setAnimation(
        izanagi::IAnimation* anm,
        IZ_BOOL isLoopAnm)
    {
        SAFE_REPLACE(m_anm, anm);

        m_isLoopAnm = isLoopAnm;

        if (m_anm) {
            m_timeline.Init(
                m_anm->GetAnimationTime(),
                0.0f);
            m_timeline.EnableLoop(m_isLoopAnm);
            m_timeline.AutoReverse(IZ_FALSE);
            m_timeline.Reset();
            m_timeline.Start();
        }
    }

    izanagi::IAnimation* AnimationStateMachineNode::getAnimation()
    {
        return m_anm;
    }

    IZ_BOOL AnimationStateMachineNode::addBehaviour(
        AnimationStateMachineBehaviour* behaviour,
        AnimationStateMachineNode* to)
    {
        auto b = getBehaviour(to);

        if (b) {
            return IZ_FALSE;
        }

        behaviour->setNode(this, to);

        auto ret = m_behaviours.AddTail(behaviour->getListItem());

        if (ret) {
            behaviour->AddRef();
        }

        return ret;
    }

    IZ_BOOL AnimationStateMachineNode::removeBehaviour(AnimationStateMachineBehaviour* behaviour)
    {
        if (behaviour->getListItem()->GetList() == &m_behaviours) {
            behaviour->getListItem()->Leave();
            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    IZ_BOOL AnimationStateMachineNode::removeBehaviour(AnimationStateMachineNode* to)
    {
        auto behaviour = getBehaviour(to);

        if (behaviour) {
            SAFE_RELEASE(behaviour);
            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    AnimationStateMachineBehaviour* AnimationStateMachineNode::getBehaviour(AnimationStateMachineNode* to)
    {
        auto item = m_behaviours.GetTop();

        while (item) {
            auto behaviour = item->GetData();

            if (behaviour->getTo() == to) {
                return behaviour;
            }

            item = item->GetNext();
        }

        return nullptr;
    }

    StateMachineNode::State AnimationStateMachineNode::update(IZ_FLOAT delta)
    {
        if (m_state == State::Enter) {
            m_currentBehaviour = nullptr;

            enter();
            m_state = State::Running;
            return State::Enter;
        }
        else if (m_state == State::Exit) {
            exit();
            m_state = State::Enter;
            return State::Exit;
        }
        else {
            // TODO
            // behaviourの優先度.

            auto item = m_behaviours.GetTop();

            while (item) {
                auto behaviour = item->GetData();

                auto state = behaviour->update(delta);

                if (state == State::Enter) {
                    // ビヘイビアが起動したのでノードから抜ける.
                    m_currentBehaviour = behaviour;

                    m_state = State::Exit;

                    return State::None;
                }

                item = item->GetNext();
            }

            if (updateAnimation(delta)) {
                m_state = State::Running;
                return State::Running;
            }
            else {
                if (m_callbackIfNoneLoopAnimationOver) {
                    m_callbackIfNoneLoopAnimationOver();
                }

                if (m_willAutoReturnToPreviousNode) {
                    // 前のノードに戻るビヘイビアを探す.
                    if (m_fromBehaviour) {
                        auto to = m_fromBehaviour->getFrom();
                        auto behaviour = getBehaviour(to);

                        if (behaviour) {
                            m_currentBehaviour = behaviour;
                        }
                    }

                    m_state = State::Exit;
                    return State::None;
                }
                else {
                    return State::None;
                }
            }
        }
    }

    StateMachineNode* AnimationStateMachineNode::next()
    {
        return m_currentBehaviour;
    }

    void AnimationStateMachineNode::enter()
    {
        StateMachineNode::enter();

        m_timeline.Reset();
        m_timeline.Start();

        // 前のアニメーションから現在の時間を引き継ぐ.
        if (m_fromBehaviour) {
            auto t = m_fromBehaviour->getTimeline().GetTime();
            auto anmTime = getAnimation()->GetAnimationTime();

            t = (t > anmTime ? anmTime : t);

            auto interp = m_fromBehaviour->getAnmInterpType();
            if (interp == izanagi::CAnimationInterp::E_INTERP_TYPE::E_INTERP_TYPE_FROZEN) {
                t = 0.0f;
            }

            m_timeline.OverrideTimeForcibly(t);
        }
    }

    void AnimationStateMachineNode::exit()
    {
        StateMachineNode::exit();

        SAFE_RELEASE(m_skl);
        m_fromBehaviour = nullptr;
    }

    IZ_BOOL AnimationStateMachineNode::updateAnimation(IZ_FLOAT delta)
    {
        if (m_anm && m_skl) {
            if (!m_isLoopAnm) {
                // アニメーションがループしない場合は、アニメーションが終了した時点でノードから抜ける.
                auto f = m_timeline.GetNormalized();
                if (f >= 1.0f) {
                    return IZ_FALSE;
                }
            }
            
            auto t = m_timeline.GetTime();

            // TODO
            // time が duration を超えたとき.

            if (AnimationStateMachine::StateHandler) {
                AnimationStateMachine::StateHandler(
                    m_name.GetString(),
                    t,
                    m_timeline.GetDuration());
            }

            m_skl->ApplyAnimation(t, m_anm);

            m_timeline.Advance(delta);

            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    void AnimationStateMachineNode::setTarget(izanagi::CSkeletonInstance* skl)
    {
        SAFE_REPLACE(m_skl, skl);
    }
}
}