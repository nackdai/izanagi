#include "StateC.h"
#include "StateManager.h"

// 更新.
IZ_BOOL CStateC::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device)
{
    if (m_IsBack) {
        m_IsBack = IZ_FALSE;

        CStateManager::GetInstance().ChangePrevState();
    }

    return IZ_TRUE;
}

IZ_UINT CStateC::GetIndex()
{
    return 2;
}

// キー押下
IZ_BOOL CStateC::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_BACK) {
        m_IsBack = IZ_TRUE;
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        CStateManager::GetInstance().ChangeState(State_B);
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        CStateManager::GetInstance().ChangeState(State_A);
    }

    return IZ_TRUE;
}
