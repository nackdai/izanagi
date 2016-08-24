#include "engine/StateMachineBehaviour.h"

namespace izanagi {
namespace engine {
    StateMachineBehaviour::~StateMachineBehaviour()
    {
    }

    IZ_BOOL StateMachineBehaviour::addCondition(StateMachineCondition* cond)
    {
        auto name = cond->getName();

        if (!isRegistered(name.GetString())) {
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

    IZ_BOOL StateMachineBehaviour::removeCondition(StateMachineCondition* cond)
    {
        auto item = m_conditions.GetTop();

        while (item) {
            auto _cond = item->GetData();

            if (_cond == cond) {
                item->Leave();
                return IZ_TRUE;
            }

            item = item->GetNext();
        }

        return IZ_FALSE;
    }

    IZ_BOOL StateMachineBehaviour::removeCondition(const char* name)
    {
        auto item = m_conditions.GetTop();

        while (item) {
            auto cond = item->GetData();

            if (cond->isSame(name)) {
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

    StateMachineCondition* StateMachineBehaviour::getCondition(IZ_UINT idx)
    {
        IZ_ASSERT(idx < getConditionNum());

        auto item = m_conditions.GetAt(idx);

        if (item) {
            auto cond = item->GetData();
            return cond;
        }

        return nullptr;
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