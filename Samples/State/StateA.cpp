#include "StateA.h"
#include "StateManager.h"

// 更新.
IZ_BOOL CStateA::Update()
{
	if (m_IsBack) {
		m_IsBack = IZ_FALSE;

		CStateManager::GetInstance().ChangePrevState();
	}

	return IZ_TRUE;
}

IZ_UINT CStateA::GetIndex()
{
	return 0;
}

// キー押下
IZ_BOOL CStateA::OnKeyDown(IZ_UINT nChar)
{
	if (nChar == VK_BACK) {
		m_IsBack = IZ_TRUE;
	}
	else if (nChar == VK_UP) {
		CStateManager::GetInstance().ChangeState(State_C);
	}
	else if (nChar == VK_DOWN) {
		CStateManager::GetInstance().ChangeState(State_B);
	}

	return IZ_TRUE;
}
