#include "engine/StateMachineBehaviour.h"

namespace izanagi {
namespace engine {
    StateMachineBehaviourCondition::StateMachineBehaviourCondition()
    {
        m_item.Init(this);
    }

    template <typename _T>
    IZ_BOOL StateMachineBehaviourCondition::onCompare(const izanagi::CValue& value)
    {
        IZ_BOOL ret = IZ_FALSE;

        switch (m_cmp) {
        case StateMachineBehaviourCondition::Cmp::Equal:
            ret = ((_T)value == (_T)m_value);
            break;
        case StateMachineBehaviourCondition::Cmp::NotEqual:
            ret = ((_T)value != (_T)m_value);
            break;
        case StateMachineBehaviourCondition::Cmp::Less:
            ret = ((_T)value < (_T)m_value);
            break;
        case StateMachineBehaviourCondition::Cmp::LessEqual:
            ret = ((_T)value <= (_T)m_value);
            break;
        case StateMachineBehaviourCondition::Cmp::Greater:
            ret = ((_T)value > (_T)m_value);
            break;
        case StateMachineBehaviourCondition::Cmp::GreaterEqual:
            ret = ((_T)value >= (_T)m_value);
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        return ret;
    }

    IZ_BOOL StateMachineBehaviourCondition::compare(const izanagi::CValue& value)
    {
        IZ_BOOL ret = IZ_FALSE;

        if (m_type == StateMachineBehaviourCondition::Type::Int) {
            ret = onCompare<IZ_INT32>(value);
        }
        else if (m_type == StateMachineBehaviourCondition::Type::Float) {
            ret = onCompare<IZ_FLOAT>(value);
        }
        else if (m_type == StateMachineBehaviourCondition::Type::Bool) {
            IZ_BOOL a = value.GetValueAsBool();
            IZ_BOOL b = m_value.GetValueAsBool();

            if (m_cmp == StateMachineBehaviourCondition::Cmp::Equal) {
                ret = (a == b);
            }
            else if (m_cmp == StateMachineBehaviourCondition::Cmp::NotEqual) {
                ret = (a != b);
            }
            else {
                IZ_ASSERT(IZ_FALSE);
            }
        }
        else {
            IZ_ASSERT(IZ_FALSE);
        }

        return ret;
    }

    IZ_BOOL StateMachineBehaviourCondition::compare(
        const char* name,
        const izanagi::CValue& value)
    {
        IZ_BOOL ret = IZ_FALSE;

        if (isSame(name)) {
            ret = compare(value);
        }

        return ret;
    }

    const StateMachineBehaviourCondition::Name& StateMachineBehaviourCondition::getName() const
    {
        return m_name;
    }

    const CKey& StateMachineBehaviourCondition::getKey() const
    {
        return m_key;
    }

    StateMachineBehaviourCondition::Type StateMachineBehaviourCondition::getType() const
    {
        return m_type;
    }

    StateMachineBehaviourCondition::Cmp StateMachineBehaviourCondition::getCmp() const
    {
        return m_cmp;
    }

    const izanagi::CValue& StateMachineBehaviourCondition::getValue() const
    {
        return m_value;
    }

    void StateMachineBehaviourCondition::set(
        StateMachineBehaviourCondition::Type type,
        StateMachineBehaviourCondition::Cmp cmp,
        const izanagi::CValue& value)
    {
        m_type = type;
        m_cmp = cmp;
        m_value = value;
    }

    void StateMachineBehaviourCondition::set(
        const char* name,
        StateMachineBehaviourCondition::Type type,
        StateMachineBehaviourCondition::Cmp cmp,
        const izanagi::CValue& value)
    {
        IZ_ASSERT(strlen(name) <= m_name.GetLen());

        set(type, cmp, value);

        m_name = name;
        m_key.SetValue(m_name.GetKeyValue());
    }

    IZ_BOOL StateMachineBehaviourCondition::isSame(const char* name)
    {
        CKey key(name);
        return m_key == key;
    }

    void StateMachineBehaviourCondition::setBinding(
        StateMachineBehaviourCondition::Binding binding)
    {
        m_binding = binding;
    }

    void StateMachineBehaviourCondition::setCurrentValue(const izanagi::CValue& value)
    {
        m_curValue = value;
    }

    IZ_BOOL StateMachineBehaviourCondition::update()
    {
        IZ_BOOL ret = IZ_FALSE;

        if (m_binding) {
            auto value = m_binding();
            ret = compare(value);
        }
        else {
            ret = compare(m_curValue);
        }

        return ret;
    }

    ////////////////////////////////////////////////////////////////////

    StateMachineBehaviour::~StateMachineBehaviour()
    {
        auto item = m_conditions.GetTop();

        while (item) {
            auto cond = item->GetData();

            item = item->GetNext();
            item->Leave();

            if (cond->isDelegated()) {
                releaseDelegatedContiditon(cond);
            }
        }
    }

    void StateMachineBehaviour::releaseDelegatedContiditon(StateMachineBehaviourCondition* cond)
    {
        if (cond) {
            delete cond;
        }
    }

    IZ_BOOL StateMachineBehaviour::addCondition(
        StateMachineBehaviourCondition* cond,
        IZ_BOOL willDelegate/*= IZ_FALSE*/)
    {
        cond->setIsDelegated(willDelegate);

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

    IZ_BOOL StateMachineBehaviour::removeCondition(StateMachineBehaviourCondition* cond)
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

    StateMachineBehaviourCondition* StateMachineBehaviour::getCondition(IZ_UINT idx)
    {
        IZ_ASSERT(idx < getConditionNum());

        auto item = m_conditions.GetAt(idx);

        if (item) {
            auto cond = item->GetData();
            return cond;
        }

        return nullptr;
    }

    StateMachineBehaviourCondition* StateMachineBehaviour::getCondition(const char* name)
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
                    return State::None;
                }

                item = item->GetNext();
            }

            enter();

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