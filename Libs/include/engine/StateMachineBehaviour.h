#if !defined(__IZANAGI_ENGINE_STATE_MACHINE_BEHAVIOUR_H__)
#define __IZANAGI_ENGINE_STATE_MACHINE_BEHAVIOUR_H__

#include <functional>
#include "izStd.h"

#include "engine/StateMachineNode.h"

namespace izanagi {
    namespace engine {
        /**
        */
        class StateMachineBehaviourCondition {
            friend class StateMachineBehaviour;

        public:
            StateMachineBehaviourCondition();
            virtual ~StateMachineBehaviourCondition() {}

            NO_COPIABLE(StateMachineBehaviourCondition);

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

            StateMachineBehaviourCondition::Type getType() const;
            StateMachineBehaviourCondition::Cmp getCmp() const;

            const izanagi::CValue& getValue() const;

            void set(
                StateMachineBehaviourCondition::Type type,
                StateMachineBehaviourCondition::Cmp cmp,
                const izanagi::CValue& value);

            void set(
                const char* name,
                StateMachineBehaviourCondition::Type type,
                StateMachineBehaviourCondition::Cmp cmp,
                const izanagi::CValue& value);

            IZ_BOOL isSame(const char* name);

            void setBinding(Binding binding);

            void setCurrentValue(const izanagi::CValue& value);

            IZ_BOOL update();

        private:
            izanagi::CStdList<StateMachineBehaviourCondition>::Item* getListItem()
            {
                return &m_item;
            }

            void setIsDelegated(IZ_BOOL b)
            {
                m_isDelegated = b;
            }

            IZ_BOOL isDelegated() const
            {
                return m_isDelegated;
            }

            template <typename _T>
            IZ_BOOL onCompare(const izanagi::CValue& value);

        private:
            izanagi::CStdList<StateMachineBehaviourCondition>::Item m_item;

            Name m_name;
            izanagi::CKey m_key;

            StateMachineBehaviourCondition::Type m_type{ StateMachineBehaviourCondition::Type::Bool };
            StateMachineBehaviourCondition::Cmp m_cmp{ StateMachineBehaviourCondition::Cmp::Equal };
            izanagi::CValue m_value;

            izanagi::CValue m_curValue;

            IZ_BOOL m_isDelegated{ IZ_FALSE };

            Binding m_binding{ nullptr };
        };

        /**
        */
        class StateMachineBehaviour : public StateMachineNode {
        public:
            StateMachineBehaviour() {}
            virtual ~StateMachineBehaviour();

            NO_COPIABLE(StateMachineBehaviour);

        public:
            IZ_BOOL addCondition(
                StateMachineBehaviourCondition* cond,
                IZ_BOOL willDelegate = IZ_FALSE);

            IZ_BOOL isRegistered(const char* name);

            IZ_BOOL removeCondition(StateMachineBehaviourCondition* cond);
            IZ_BOOL removeCondition(const char* name);

            IZ_UINT getConditionNum() const;

            StateMachineBehaviourCondition* getCondition(IZ_UINT idx);
            StateMachineBehaviourCondition* getCondition(const char* name);

            virtual StateMachineNode::State update(IZ_FLOAT delta) override;

            virtual IZ_BOOL isNode() const override
            {
                return IZ_FALSE;
            }

        protected:
            virtual void releaseDelegatedContiditon(StateMachineBehaviourCondition* cond);

        private:
            izanagi::CStdList<StateMachineBehaviourCondition> m_conditions;
        };

    }   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_STATE_MACHINE_BEHAVIOUR_H__)
