#include "MySystem.h"
#include "StateManager.h"
#include "StateBasic.h"
#include "StatePhong.h"
#include "StateHemiLight.h"
#include "StateNormalMap.h"
#include "StateParallax.h"
#include "StateRimLight.h"

CStateManager CStateManager::s_cInstance;

#define _STATE(StateName, ClassName) \
	static ClassName _##ClassName; \
	m_pStateTable[STATE_##StateName] = &_##ClassName;

// コンストラクタ
CStateManager::CStateManager()
{
	memset(m_pStateTable, 0, sizeof(m_pStateTable));
	
	// タスクテーブル
	{
		_STATE(BASIC, CStateBasic)
		_STATE(PHONG, CStatePhong)
		_STATE(HEMISPHERE, CStateHemiLight)
		_STATE(NORMAL_MAP, CStateNormalMap)
		_STATE(PARALLAX, CStateParallax)
		_STATE(RIM_LIGHT, CStateRimLight)
	}
	
	m_nNextState = STATE_NUM;
	m_nPrevState = STATE_NUM;
	m_nCurrentState = STATE_NUM;
}

IZ_BOOL CStateManager::OnKeyDown(IZ_UINT nChar)
{
	IZ_BOOL ret = IZ_FALSE;

	izanagi::CKeyboard* pKeyboard = CMySystem::GetInstance().GetKeyboard();

	IZ_INT nState = m_nCurrentState;

	if (ret = pKeyboard->IsPushOneShotKey(izanagi::E_KEYBOARD_BUTTON_UP)) {
		nState--;
		if (nState < 0) {
			nState = STATE_NUM - 1;
		}
	}
	else if (ret = pKeyboard->IsPushOneShotKey(izanagi::E_KEYBOARD_BUTTON_DOWN)) {
		nState++;
		if (nState >= STATE_NUM) {
			nState = 0;
		}
	}

	if (ret) {
		ChangeState((STATE)nState);
	}

	return ret;
}
