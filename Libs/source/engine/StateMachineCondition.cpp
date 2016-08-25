#include "engine/StateMachineCondition.h"

namespace izanagi {
namespace engine {

    const char* StateMachineConditionValue::getName() const
    {
        return m_name.GetString();
    }

    IZ_UINT StateMachineConditionValue::getNameLength() const
    {
        auto name = getName();
        return strlen(name);
    }

    void StateMachineConditionValue::setName(const char* name)
    {
        m_name = name;
        m_key.SetValue(m_name.GetKeyValue());
    }

    IZ_BOOL StateMachineConditionValue::isSame(const char* name) const
    {
        CKey key(name);
        return m_key == key;
    }

    izanagi::CValue StateMachineConditionValue::getValue() const
    {
        return std::move(m_value);
    }

    void StateMachineConditionValue::setValue(const izanagi::CValue& value)
    {
        m_value = value;
    }

    //////////////////////////////////////////////////////////////////

    StateMachineCondition::StateMachineCondition()
    {
        m_item.Init(this);
    }

    StateMachineCondition::~StateMachineCondition()
    {
        SAFE_RELEASE(m_target);
    }

    template <typename _T>
    IZ_BOOL StateMachineCondition::onCompare(const izanagi::CValue& value)
    {
        IZ_BOOL ret = IZ_FALSE;

        switch (m_cmp) {
        case StateMachineCondition::Cmp::Equal:
            ret = ((_T)value == (_T)m_threshold);
            break;
        case StateMachineCondition::Cmp::NotEqual:
            ret = ((_T)value != (_T)m_threshold);
            break;
        case StateMachineCondition::Cmp::Less:
            ret = ((_T)value < (_T)m_threshold);
            break;
        case StateMachineCondition::Cmp::LessEqual:
            ret = ((_T)value <= (_T)m_threshold);
            break;
        case StateMachineCondition::Cmp::Greater:
            ret = ((_T)value >(_T)m_threshold);
            break;
        case StateMachineCondition::Cmp::GreaterEqual:
            ret = ((_T)value >= (_T)m_threshold);
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
            IZ_BOOL b = m_threshold.GetValueAsBool();

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

    StateMachineCondition::Type StateMachineCondition::getType() const
    {
        return m_type;
    }

    StateMachineCondition::Cmp StateMachineCondition::getCmp() const
    {
        return m_cmp;
    }

    const izanagi::CValue& StateMachineCondition::getThreshold() const
    {
        return m_threshold;
    }

    void StateMachineCondition::set(
        StateMachineCondition::Type type,
        StateMachineCondition::Cmp cmp,
        const izanagi::CValue& threshold)
    {
        m_type = type;
        m_cmp = cmp;
        m_threshold = threshold;
    }

    void StateMachineCondition::set(
        StateMachineCondition::Type type,
        StateMachineCondition::Cmp cmp)
    {
        m_type = type;
        m_cmp = cmp;
    }

    IZ_BOOL StateMachineCondition::isValid() const
    {
        return (m_target != nullptr);
    }

    const char* StateMachineCondition::getName() const
    {
        IZ_ASSERT(m_target);

        return m_target->getName();
    }

    IZ_BOOL StateMachineCondition::isSame(const char* name) const
    {
        IZ_ASSERT(m_target);

        return m_target->isSame(name);
    }

    IZ_BOOL StateMachineCondition::setTargetValue(StateMachineConditionValue* target)
    {
        IZ_ASSERT(target);

        if (target->getNameLength() == 0) {
            return IZ_FALSE;
        }

        SAFE_REPLACE(m_target, target);
        return IZ_TRUE;
    }

    StateMachineConditionValue* StateMachineCondition::getTargetValue()
    {
        return m_target;
    }

    IZ_BOOL StateMachineCondition::update()
    {
        if (!m_target) {
            return IZ_TRUE;
        }

        auto ret = compare(m_target->getValue());
        return ret;
    }
}
}