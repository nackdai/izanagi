#include"engine/AnimationStateMachine.h"

namespace izanagi {
namespace engine {
    std::function<void(const char*, IZ_FLOAT, IZ_FLOAT)> AnimationStateMachine::StateHandler = nullptr;

    AnimationStateMachine* AnimationStateMachine::create(IMemoryAllocator* allocator)
    {
        void* buf = ALLOC(allocator, sizeof(AnimationStateMachine));
        VRETURN_NULL(buf);

        AnimationStateMachine* ret = new(buf)AnimationStateMachine;
        {
            ret->m_Allocator = allocator;

            //ret->m_blendAnm = izanagi::IAnimationBlender::CreateAnmBlender<izanagi::CAnmLinearBlender>(allocator);
            ret->m_blendAnm = izanagi::CAnimationInterp::CreateAnimationInterp(allocator);
            IZ_ASSERT(ret->m_blendAnm);

            ret->AddRef();
        }

        return ret;
    }

    AnimationStateMachine::AnimationStateMachine()
    {
        m_fromEntryTo.AddRef();
        m_toExitFrom.AddRef();

        m_fromEntryTo.setName("FromEntryTo");
        m_toExitFrom.setName("ToExitFrom");

        m_fromEntryTo.disableSetOwnNextNode();
        m_toExitFrom.disableSetOwnNextNode();

        m_entry.AddRef();
        m_exit.AddRef();

        m_entry.setName("entry");
        m_exit.setName("exit");

        m_entry.setWillAutoReturnToPreviousNode(IZ_FALSE);
        m_exit.setWillAutoReturnToPreviousNode(IZ_FALSE);

        m_current = &m_entry;
    }

    AnimationStateMachine::~AnimationStateMachine()
    {
        {
            auto item = m_values.GetTop();

            while (item) {
                auto value = item->GetData();

                item = item->GetNext();

                SAFE_RELEASE(value);
            }
        }

        {
            auto item = m_nodes.GetTop();

            while (item) {
                auto node = item->GetData();

                item = item->GetNext();

                SAFE_RELEASE(node);
            }
        }

        SAFE_RELEASE(m_blendAnm);
        SAFE_RELEASE(m_skl);
    }

    AnimationStateMachineNode* AnimationStateMachine::addNode(
        const char* name,
        izanagi::IAnimation* anm,
        IZ_BOOL isLoopAnm/*= IZ_TRUE*/)
    {
        auto node = getNode(name);

        if (!node) {
            node = AnimationStateMachineNode::create(m_Allocator, name);
            IZ_ASSERT(node);

            m_nodes.AddTail(node->getListItem());

            node->setAnimation(anm, isLoopAnm);
        }

        return node;
    }

    AnimationStateMachineNode* AnimationStateMachine::getNode(const char* name)
    {
        auto item = m_nodes.GetTop();

        while (item) {
            auto node = item->GetData();

            if (node->getKey() == name) {
                return node;
            }

            item = item->GetNext();
        }

        return nullptr;
    }

    AnimationStateMachineBehaviour* AnimationStateMachine::addBehaviour(
        const char* from,
        const char* to,
        izanagi::CAnimationInterp::E_INTERP_TYPE interpType/*= izanagi::CAnimationInterp::E_INTERP_TYPE::E_INTERP_TYPE_SMOOTH*/)
    {
        IZ_ASSERT(from || to);

        AnimationStateMachineNode* fromNode = nullptr;
        AnimationStateMachineNode* toNode = nullptr;

        if (from) {
            IZ_ASSERT(strlen(from) > 0);
            if (m_entry.getName() != from) { 
                fromNode = getNode(from);
            }
        }

        if (to) {
            IZ_ASSERT(strlen(to) > 0);
            if (m_exit.getName() != to) {
                toNode = getNode(to);
            }
        }

        auto ret = addBehaviour(
            fromNode, toNode,
            interpType);

        return ret;
    }

    AnimationStateMachineBehaviour* AnimationStateMachine::addBehaviour(
        AnimationStateMachineNode* from,
        AnimationStateMachineNode* to,
        izanagi::CAnimationInterp::E_INTERP_TYPE interpType/*= izanagi::CAnimationInterp::E_INTERP_TYPE::E_INTERP_TYPE_SMOOTH*/)
    {
        IZ_BOOL canReturn = IZ_FALSE;

        AnimationStateMachineBehaviour* ret = nullptr;

        if (!from && to) {
            // from ‚ª null -> entry.
            from = &m_entry;
            ret = &m_fromEntryTo;
        }
        else if (from && !to) {
            // to ‚ª null -> exit.
            to = &m_exit;
            ret = &m_toExitFrom;
        }

        if (from && to) {
            // TODO
            //IZ_ASSERT(isRegistered(from) && isRegistered(to));
            IZ_ASSERT(from != to);

            if (!ret) {
                ret = AnimationStateMachineBehaviour::create(m_Allocator);
                IZ_ASSERT(ret);

                canReturn = IZ_TRUE;
            }

            auto result = from->addBehaviour(ret, to);

            if (canReturn) {
                if (result) {
                    // fromƒm[ƒh‚ÉˆÏ÷‚·‚é.
                    ret->Release();
                }
                else {
                    SAFE_RELEASE(ret);
                }
            }
        }

        IZ_ASSERT(ret);

        ret->setAnmInterpType(interpType);

        return (canReturn ? ret : nullptr);
    }

    AnimationStateMachineConditionValue* AnimationStateMachine::addConditionValue(const char* name)
    {
        auto item = m_values.GetTop();

        while (item) {
            auto value = item->GetData();

            if (value->isSame(name)) {
                return value;
            }

            item = item->GetNext();
        }

        auto value = AnimationStateMachineConditionValue::create(
            m_Allocator,
            name);
        IZ_ASSERT(value);

        m_values.AddTail(value->getListItem());

        return value;
    }

    IZ_BOOL AnimationStateMachine::setConditionValue(
        const char* name,
        const izanagi::CValue& value)
    {
        auto item = m_values.GetTop();

        while (item) {
            auto target = item->GetData();

            if (target->isSame(name)) {
                target->setValue(value);
                return IZ_TRUE;
            }

            item = item->GetNext();
        }

        return IZ_FALSE;
    }

    AnimationStateMachineBehaviour* AnimationStateMachine::getBehaviour(
        const char* from,
        const char* to)
    {
        auto fromNode = getNode(from);
        auto toNode = getNode(to);

        auto ret = getBehaviour(fromNode, toNode);

        return ret;
    }

    AnimationStateMachineBehaviour* AnimationStateMachine::getBehaviour(
        AnimationStateMachineNode* from,
        AnimationStateMachineNode* to)
    {
        AnimationStateMachineBehaviour* ret = nullptr;

        if (from && to) {
            IZ_ASSERT(isRegistered(from) && isRegistered(to));

            ret = from->getBehaviour(to);
        }

        IZ_ASSERT(ret);

        return ret;
    }

    IZ_BOOL AnimationStateMachine::removeBehaviour(
        const char* from,
        const char* to)
    {
        auto fromNode = getNode(from);
        auto toNode = getNode(to);

        auto ret = removeBehaviour(fromNode, toNode);

        return ret;
    }

    IZ_BOOL AnimationStateMachine::removeBehaviour(
        AnimationStateMachineNode* from,
        AnimationStateMachineNode* to)
    {
        IZ_BOOL result = IZ_FALSE;

        if (from && to) {
            IZ_ASSERT(isRegistered(from) && isRegistered(to));

            result = from->removeBehaviour(to);
        }

        return result;
    }

    IZ_BOOL AnimationStateMachine::removeBehaviour(AnimationStateMachineBehaviour* behaviour)
    {
        auto item = m_nodes.GetTop();

        while (item) {
            auto node = item->GetData();

            if (node->removeBehaviour(behaviour)) {
                return IZ_TRUE;
            }

            item = item->GetNext();
        }

        return IZ_FALSE;
    }

    IZ_BOOL AnimationStateMachine::removeNode(const char* name)
    {
        auto node = getNode(name);

        auto ret = removeNode(node);

        return ret;
    }

    IZ_BOOL AnimationStateMachine::removeNode(AnimationStateMachineNode* node)
    {
        if (isRegistered(node)) {
            node->getListItem()->Leave();
            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    IZ_BOOL AnimationStateMachine::isRegistered(AnimationStateMachineNode* node)
    {
        auto ret = (node->getListItem()->GetList() == &m_nodes);
        return ret;
    }

    StateMachineNode::State AnimationStateMachine::update(IZ_FLOAT delta)
    {
        if (m_current == &m_exit) {
            m_current = &m_entry;
        }

        auto prevExitNode = m_toExitFrom.getFrom();
        if (prevExitNode) {
            // exit‚ÌŽè‘O‚Ìƒm[ƒh‚Í‚»‚Ì‘O‚Ìƒm[ƒh‚ÉŽ©“®‚Å–ß‚ç‚¹‚È‚¢.
            // -> ‹­§“I‚Éexit‚ÉŒü‚©‚í‚¹‚é.
            prevExitNode->setWillAutoReturnToPreviousNode(IZ_FALSE);
        }

        while (m_current) {
            if (m_current->isNode()) {
                AnimationStateMachineNode* node = (AnimationStateMachineNode*)m_current;
                node->setTarget(m_skl);
            }
            else {
                AnimationStateMachineBehaviour* behaviour = (AnimationStateMachineBehaviour*)m_current;
                behaviour->setTarget(m_skl, m_blendAnm);
            }

            auto state = m_current->update(delta);

            if (state == StateMachineNode::State::Exit) {
                // ŽŸ‚ÉˆÚ‚é.
                auto next = m_current->next();

                m_current = next;
            }
            else if (state == StateMachineNode::State::Running) {
                return StateMachineNode::State::Running;
            }

            if (m_current == &m_exit) {
                break;
            }
        }

        return StateMachineNode::State::Exit;
    }

    void AnimationStateMachine::update(
        IZ_FLOAT delta,
        izanagi::CSkeletonInstance* skl)
    {
        SAFE_REPLACE(m_skl, skl);
        update(delta);
    }

    izanagi::IAnimation* AnimationStateMachine::getAnimation()
    {
        auto node = getNode();
        return node->getAnimation();
    }

    const animation::CTimeline& AnimationStateMachine::getTimeline()
    {
        auto node = getNode();
        return node->getTimeline();
    }

    AnimationStateMachineNode* AnimationStateMachine::getNode()
    {
        AnimationStateMachineNode* node = nullptr;

        if (m_current == &m_entry) {
            node = m_fromEntryTo.getTo();
        }
        else if (m_current == &m_exit) {
            node = m_toExitFrom.getFrom();
        }
        else {
            node = (AnimationStateMachineNode*)m_current;
        }

        IZ_ASSERT(node);

        return node;
    }

    void AnimationStateMachine::setFromBehavour(AnimationStateMachineBehaviour* behaviour)
    {
        auto node = m_fromEntryTo.getTo();
        IZ_ASSERT(node);

        m_fromBehaviour = behaviour;

        node->setFromBehavour(behaviour);
    }

    StateMachineNode* AnimationStateMachine::next()
    {
        // “ü‚Á‚Ä‚«‚½ƒm[ƒh‚É‘JˆÚ‚·‚éƒrƒwƒCƒrƒA‚ð’T‚·.
        IZ_ASSERT(m_fromBehaviour != nullptr);

        auto from = m_fromBehaviour->getFrom();
        IZ_ASSERT(from != nullptr);

        auto item = m_behaviours.GetTop();

        while (item) {
            auto behaviour = item->GetData();

            auto to = behaviour->getTo();

            if (from == to) {
                return behaviour;
            }

            item = item->GetNext();
        }

        IZ_ASSERT(IZ_FALSE);

        return nullptr;
    }

    void AnimationStateMachine::notifyInitializedInBehavour()
    {
        m_current = &m_entry;
    }
}
}