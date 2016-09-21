#if !defined(__IZANAGI_ENGINE_STATE_MACHINE_BEHAVIOUR_H__)
#define __IZANAGI_ENGINE_STATE_MACHINE_BEHAVIOUR_H__

#include <functional>
#include "izStd.h"

#include "engine/StateMachineCondition.h"
#include "engine/StateMachineNode.h"

namespace izanagi {
namespace engine {
    /** Behaviour to transit between two nodes.
     */
    class StateMachineBehaviour : public StateMachineNode {
    protected:
        StateMachineBehaviour() {}
        virtual ~StateMachineBehaviour();

        NO_COPIABLE(StateMachineBehaviour);

    public:
        IZ_BOOL addCondition(StateMachineCondition* cond);

        IZ_BOOL isRegisteredConditionByName(const char* name);

        IZ_BOOL removeCondition(StateMachineCondition* _cond);

        IZ_UINT getConditionNum() const;

        StateMachineCondition* getConditionByName(const char* name);

        virtual StateMachineNode::State update(IZ_FLOAT delta) override;

        virtual IZ_BOOL isNode() const override
        {
            return IZ_FALSE;
        }

    protected:
        izanagi::CStdList<StateMachineCondition> m_conditions;
    };
}   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_STATE_MACHINE_BEHAVIOUR_H__)
