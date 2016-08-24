#include "engine/StateMachineCondition.h"

namespace izanagi {
namespace engine {
    StateMachineCondition::StateMachineCondition()
    {
        m_item.Init(this);
    }

    template <typename _T>
    IZ_BOOL StateMachineCondition::onCompare(const izanagi::CValue& value)
    {
        IZ_BOOL ret = IZ_FALSE;

        switch (m_cmp) {
        case StateMachineCondition::Cmp::Equal:
            ret = ((_T)value == (_T)m_value);
            break;
        case StateMachineCondition::Cmp::NotEqual:
            ret = ((_T)value != (_T)m_value);
            break;
        case StateMachineCondition::Cmp::Less:
            ret = ((_T)value < (_T)m_value);
            break;
        case StateMachineCondition::Cmp::LessEqual:
            ret = ((_T)value <= (_T)m_value);
            break;
        case StateMachineCondition::Cmp::Greater:
            ret = ((_T)value > (_T)m_value);
            break;
        case StateMachineCondition::Cmp::GreaterEqual:
            ret = ((_T)value >= (_T)m_value);
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }

        return ret;
    }

    IZ_BOOL StateMachineCondition::compare(const izanagi::CValue& value)
    {
        IZ_BOOL ret = IZ_FALSE;

        if (m_type == StateMachineCondition::Type::Int) {
            ret = onCompare<IZ_INT32>(value);
        }
        else if (m_type == StateMachineCondition::Type::Float) {
            ret = onCompare<IZ_FLOAT>(value);
        }
        else if (m_type == StateMachineCondition::Type::Bool) {
            IZ_BOOL a = value.GetValueAsBool();
            IZ_BOOL b = m_value.GetValueAsBool();

            if (m_cmp == StateMachineCondition::Cmp::Equal) {
                ret = (a == b);
            }
            else if (m_cmp == StateMachineCondition::Cmp::NotEqual) {
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

    IZ_BOOL StateMachineCondition::compare(
        const char* name,
        const izanagi::CValue& value)
    {
        IZ_BOOL ret = IZ_FALSE;

        if (isSame(name)) {
            ret = compare(value);
        }

        return ret;
    }

    const StateMachineCondition::Name& StateMachineCondition::getName() const
    {
        return m_name;
    }

    const CKey& StateMachineCondition::getKey() const
    {
        return m_key;
    }

    StateMachineCondition::Type StateMachineCondition::getType() const
    {
        return m_type;
    }

    StateMachineCondition::Cmp StateMachineCondition::getCmp() const
    {
        return m_cmp;
    }

    const izanagi::CValue& StateMachineCondition::getValue() const
    {
        return m_value;
    }

    void StateMachineCondition::set(
        StateMachineCondition::Type type,
        StateMachineCondition::Cmp cmp,
        const izanagi::CValue& value)
    {
        m_type = type;
        m_cmp = cmp;
        m_value = value;
    }

    void StateMachineCondition::set(
        const char* name,
        StateMachineCondition::Type type,
        StateMachineCondition::Cmp cmp,
        const izanagi::CValue& value)
    {
        IZ_ASSERT(strlen(name) <= m_name.GetLen());

        set(type, cmp, value);

        m_name = name;
        m_key.SetValue(m_name.GetKeyValue());
    }

    IZ_BOOL StateMachineCondition::isSame(const char* name)
    {
        CKey key(name);
        return m_key == key;
    }

    void StateMachineCondition::setBinding(
        StateMachineCondition::Binding binding)
    {
        m_binding = binding;
    }

    void StateMachineCondition::setCurrentValue(const izanagi::CValue& value)
    {
        m_curValue = value;
    }

    IZ_BOOL StateMachineCondition::update()
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
}
}