#include "StateC.h"
#include "StateManager.h"

// 更新.
IZ_BOOL CStateC::Update()
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
IZ_BOOL CStateC::OnKeyDown(IZ_UINT nChar)
{
	if (nChar == VK_BACK) {
		m_IsBack = IZ_TRUE;
	}
	else if (nChar == VK_UP) {
		CStateManager::GetInstance().ChangeState(State_B);
	}
	else if (nChar == VK_DOWN) {
		CStateManager::GetInstance().ChangeState(State_A);
	}

	return IZ_TRUE;
}
