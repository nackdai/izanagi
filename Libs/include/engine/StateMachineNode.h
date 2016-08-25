#if !defined(__IZANAGI_ENGINE_STATE_MACHINE_NODE_H__)
#define __IZANAGI_ENGINE_STATE_MACHINE_NODE_H__

#include <functional>
#include "izStd.h"

namespace izanagi {
    namespace engine {
        class StateMachineBehaviour;

        class StateMachineNode : public CObject {
            friend class AnimationStateMachine;

        protected:
            StateMachineNode();
            virtual ~StateMachineNode();

            NO_COPIABLE(StateMachineNode);

        public:
            using HookFunc = std::function<void(void)>;

            void setEnterFunc(HookFunc func);

            void setExitFunc(HookFunc func);

            virtual void enter();

            virtual void exit();

            enum State {
                None,
                Enter,
                Running,
                Exit,
            };

            PURE_VIRTUAL(StateMachineNode::State update(IZ_FLOAT delta));

            PURE_VIRTUAL(StateMachineNode* next());

            virtual IZ_BOOL isNode() const
            {
                return IZ_TRUE;
            }

            StateMachineNode::State getState() const
            {
                return m_state;
            }

        protected:
            HookFunc m_enterFunc{ nullptr };
            HookFunc m_exitFunc{ nullptr };

            //izanagi::CStdList<StateMachineBehaviour> m_behaviours;

            StateMachineNode::State m_state{ StateMachineNode::State::None };
        };

    }   // namespace engine
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_ENGINE_STATE_MACHINE_BEHAVIOUR_H__)
