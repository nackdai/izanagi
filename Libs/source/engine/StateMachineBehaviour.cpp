#include "engine/StateMachineBehaviour.h"

namespace izanagi {
namespace engine {
    StateMachineBehaviour::~StateMachineBehaviour()
    {
        auto item = m_conditions.GetTop();

        while (item) {
            auto cond = item->GetData();

            item = item->GetNext();

            SAFE_RELEASE(cond);
        }
    }

    IZ_BOOL StateMachineBehaviour::addCondition(StateMachineCondition* cond)
    {
        if (!cond->isValid()) {
            return IZ_FALSE;
        }

        auto name = cond->getName();

        if (!isRegistered(name)) {
            m_conditions.AddTail(cond->getListItem());

            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    IZ_BOOL StateMachineBehaviour::isRegistered(const char* name)
    {
        auto item = m_conditions.GetTop();

        while (item) {
            auto cond = item->GetData();

            if (cond->isSame(name)) {
                return IZ_TRUE;
            }

            item = item->GetNext();
        }

        return IZ_FALSE;
    }

    IZ_BOOL StateMachineBehaviour::removeCondition(StateMachineCondition* _cond)
    {
        auto item = m_conditions.GetTop();

        while (item) {
            auto cond = item->GetData();

            if (cond == _cond) {
                item->Leave();
                return IZ_TRUE;
            }

            item = item->GetNext();
        }

        return IZ_FALSE;
    }

    IZ_UINT StateMachineBehaviour::getConditionNum() const
    {
        auto ret = m_conditions.GetItemNum();
        return ret;
    }

    StateMachineCondition* StateMachineBehaviour::getCondition(const char* name)
    {
        auto item = m_conditions.GetTop();

        while (item) {
            auto cond = item->GetData();

            if (cond->isSame(name)) {
                return cond;
            }

            item = item->GetNext();
        }

        return nullptr;
    }

    StateMachineNode::State StateMachineBehaviour::update(IZ_FLOAT delta)
    {
        State ret = State::None;

        if (m_state == State::None) {
            auto item = m_conditions.GetTop();

            while (item) {
                auto cond = item->GetData();

                if (!cond->update()) {
                    // １つでも条件が合わなければ、このビヘイビアは処理中として外に対して通知する.
                    return State::Running;
                }

                item = item->GetNext();
            }

            m_state = State::Enter;
            ret = State::Enter;
        }
        else if (m_state == State::Enter) {
            enter();

            m_state = State::Running;
            ret = State::Running;
        }
        else if (m_state == State::Exit) {
            exit();

            m_state = State::None;
            ret = State::Exit;
        }
        else if (m_state == State::Running) {
            ret = State::Running;
        }

        return ret;
    }
}
}