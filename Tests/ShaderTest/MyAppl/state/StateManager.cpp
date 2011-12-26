#include "StateManager.h"
#include "StateMenu.h"
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
		_STATE(MENU, CStateMenu)
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
