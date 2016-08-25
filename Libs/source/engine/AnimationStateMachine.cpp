#include"engine/AnimationStateMachine.h"

namespace izanagi {
namespace engine {
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

        m_fromEntryTo.m_tag.SetString("FromEntryTo");
        m_toExitFrom.m_tag.SetString("ToExitFrom");

        m_entry.AddRef();
        m_exit.AddRef();

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
        const char* to)
    {
        auto fromNode = getNode(from);
        auto toNode = getNode(to);

        auto ret = addBehaviour(
            fromNode, toNode);

        return ret;
    }

    AnimationStateMachineBehaviour* AnimationStateMachine::addBehaviour(
        AnimationStateMachineNode* from,
        AnimationStateMachineNode* to)
    {
        AnimationStateMachineBehaviour* ret = nullptr;

        if (from && to) {
            IZ_ASSERT(isRegistered(from) && isRegistered(to));
            IZ_ASSERT(from != to);

            ret = AnimationStateMachineBehaviour::create(m_Allocator);
            IZ_ASSERT(ret);

            auto result = from->addBehaviour(ret, to);

            if (result) {
                // fromƒm[ƒh‚ÉˆÏ÷‚·‚é.
                ret->Release();
            }
            else {
                SAFE_RELEASE(ret);
            }
        }

        IZ_ASSERT(ret);

        return ret;
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

    IZ_BOOL AnimationStateMachine::setFromEntryTo(const char* name)
    {
        auto node = getNode(name);

        auto ret = setFromEntryTo(node);

        return ret;
    }

    IZ_BOOL AnimationStateMachine::setFromEntryTo(AnimationStateMachineNode* node)
    {
        IZ_BOOL ret = IZ_FALSE;

        if (isRegistered(node)) {
            ret = m_entry.addBehaviour(&m_fromEntryTo, node);
        }

        return ret;
    }

    IZ_BOOL AnimationStateMachine::setToExitFrom(const char* name)
    {
        auto node = getNode(name);

        auto ret = setToExitFrom(node);

        return ret;
    }

    IZ_BOOL AnimationStateMachine::setToExitFrom(AnimationStateMachineNode* node)
    {
        IZ_BOOL ret = IZ_FALSE;

        if (isRegistered(node)) {
            ret = node->addBehaviour(&m_toExitFrom, &m_exit);
        }

        return ret;
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
}
}