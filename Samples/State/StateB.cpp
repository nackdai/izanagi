#include "StateB.h"
#include "StateManager.h"

// 更新.
IZ_BOOL CStateB::Update()
{
    if (m_IsBack) {
        m_IsBack = IZ_FALSE;

        CStateManager::GetInstance().ChangePrevState();
    }

    return IZ_TRUE;
}

IZ_UINT CStateB::GetIndex()
{
    return 1;
}

// キー押下
IZ_BOOL CStateB::OnKeyDown(IZ_UINT nChar)
{
    if (nChar == VK_BACK) {
        m_IsBack = IZ_TRUE;
    }
    else if (nChar == VK_UP) {
        CStateManager::GetInstance().ChangeState(State_A);
    }
    else if (nChar == VK_DOWN) {
        CStateManager::GetInstance().ChangeState(State_C);
    }

    return IZ_TRUE;
}
