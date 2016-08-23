#include"engine/StateMachineNode.h"
#include"engine/StateMachineBehaviour.h"

namespace izanagi {
namespace engine {
    StateMachineNode::StateMachineNode()
    {
    }

    StateMachineNode::~StateMachineNode()
    {
    }

    void StateMachineNode::setEnterFunc(StateMachineNode::HookFunc func)
    {
        m_enterFunc = func;
    }

    void StateMachineNode::setExitFunc(StateMachineNode::HookFunc func)
    {
        m_exitFunc = func;
    }

    void StateMachineNode::enter()
    {
        if (m_enterFunc) {
            m_enterFunc();
        }
    }

    void StateMachineNode::exit()
    {
        if (m_exitFunc) {
            m_exitFunc();
        }
    }
}
}