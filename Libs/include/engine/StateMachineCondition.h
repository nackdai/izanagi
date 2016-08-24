#if !defined(__IZANAGI_ENGINE_STATE_MACHINE_CONDITION_H__)
#define __IZANAGI_ENGINE_STATE_MACHINE_CONDITION_H__

#include <functional>
#include "izStd.h"

namespace izanagi {
namespace engine {
    /**
    */
    class StateMachineCondition {
        friend class StateMachineBehaviour;

    public:
        StateMachineCondition();
        virtual ~StateMachineCondition() {}

        NO_COPIABLE(StateMachineCondition);

    public:
        enum Type {
            Int,
            Float,
            Bool,
        };

        enum Cmp {
            Equal,
            NotEqual,
            Less,
            LessEqual,
            Greater,
            GreaterEqual,
        };

        using Name = izanagi::CStdString < IZ_CHAR, 15 >;

        using Binding = std::function < const izanagi::CValue&() >;

        IZ_BOOL compare(const izanagi::CValue& value);

        IZ_BOOL compare(
            const char* name,
            const izanagi::CValue& value);

        const Name& getName() const;

        const CKey& getKey() const;

        StateMachineCondition::Type getType() const;
        StateMachineCondition::Cmp getCmp() const;

        const izanagi::CValue& getValue() const;

        void set(
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp,
            const izanagi::CValue& value);

        void set(
            const char* name,
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp,
            const izanagi::CValue& value);

        IZ_BOOL isSame(const char* name);

        void setBinding(Binding binding);

        void setCurrentValue(const izanagi::CValue& value);

        IZ_BOOL update();

    private:
        izanagi::CStdList<StateMachineCondition>::Item* getListItem()
        {
            return &m_item;
        }

        template <typename _T>
        IZ_BOOL onCompare(const izanagi::CValue& value);

    private:
        izanagi::CStdList<StateMachineCondition>::Item m_item;

        Name m_name;
        izanagi::CKey m_key;

        StateMachineCondition::Type m_type{ StateMachineCondition::Type::Bool };
        StateMachineCondition::Cmp m_cmp{ StateMachineCondition::Cmp::Equal };
        izanagi::CValue m_value;

        izanagi::CValue m_curValue;

        Binding m_binding{ nullptr };
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_STATE_MACHINE_CONDITION_H__)
