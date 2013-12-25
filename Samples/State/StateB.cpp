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
IZ_BOOL CStateB::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_BACK) {
        m_IsBack = IZ_TRUE;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        CStateManager::GetInstance().ChangeState(State_A);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        CStateManager::GetInstance().ChangeState(State_C);
    }

    return IZ_TRUE;
}
