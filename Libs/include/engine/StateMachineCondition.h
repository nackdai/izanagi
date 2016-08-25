#if !defined(__IZANAGI_ENGINE_STATE_MACHINE_CONDITION_H__)
#define __IZANAGI_ENGINE_STATE_MACHINE_CONDITION_H__

#include <functional>
#include "izStd.h"

namespace izanagi {
namespace engine {
    class StateMachineConditionValue : public CObject {
    public:
        /** Value's name.
         */
        using Name = izanagi::CStdString < IZ_CHAR, 15 >;

    protected:
        StateMachineConditionValue() {}
        virtual ~StateMachineConditionValue() {}

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        const char* getName() const;

        IZ_UINT getNameLength() const;

        void setName(const char* name);

        IZ_BOOL isSame(const char* name) const;

        izanagi::CValue getValue() const;

        void setValue(const izanagi::CValue& value);

    protected:
        IMemoryAllocator* m_Allocator{ nullptr };

        Name m_name;
        izanagi::CKey m_key;

        izanagi::CValue m_value;
    };

    /** Condition about behavoiour.
     */
    class StateMachineCondition : public CObject {
        friend class StateMachineBehaviour;

    protected:
        StateMachineCondition();
        virtual ~StateMachineCondition();

        NO_COPIABLE(StateMachineCondition);

        IZ_DEFINE_INTERNAL_RELEASE();

    public:
        /** Value type.
         */
        enum Type {
            Int,
            Float,
            Bool,
        };

        /** Compare way between values.
         */
        enum Cmp {
            Equal,
            NotEqual,
            Less,
            LessEqual,
            Greater,
            GreaterEqual,
        };

        /** Compare target value.
         */
        IZ_BOOL compare(const izanagi::CValue& value);

        /** Get key of condition's name.
         */
        const CKey& getKey() const;

        /** Get condition value type.
         */
        StateMachineCondition::Type getType() const;

        /** Get compare way.
         */
        StateMachineCondition::Cmp getCmp() const;

        /** Get condition's threshold.
         */
        const izanagi::CValue& getThreshold() const;

        void setThreshold(const izanagi::CValue& threshold);

        void set(
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp,
            const izanagi::CValue& threshold);

        void set(
            StateMachineCondition::Type type,
            StateMachineCondition::Cmp cmp);

        IZ_BOOL isValid() const;

        const char* getName() const;

        IZ_BOOL isSame(const char* name) const;

        IZ_BOOL setTargetValue(StateMachineConditionValue* target);

        StateMachineConditionValue* getTargetValue();

        IZ_BOOL update();

    private:
        izanagi::CStdList<StateMachineCondition>::Item* getListItem()
        {
            return &m_item;
        }

        template <typename _T>
        IZ_BOOL onCompare(const izanagi::CValue& value);

    protected:
        IMemoryAllocator* m_Allocator{ nullptr };

        izanagi::CStdList<StateMachineCondition>::Item m_item;

        StateMachineCondition::Type m_type{ StateMachineCondition::Type::Bool };
        StateMachineCondition::Cmp m_cmp{ StateMachineCondition::Cmp::Equal };
        izanagi::CValue m_threshold;

        StateMachineConditionValue* m_target{ nullptr };
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_STATE_MACHINE_CONDITION_H__)
