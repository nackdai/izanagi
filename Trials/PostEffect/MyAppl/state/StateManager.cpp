#include "StateManager.h"

#include "StateMenu.h"
#include "StateMonotone.h"
#include "StateBloomStar.h"
#include "StateBloomStarEx.h"
#include "StateHDR.h"

CStateManager CStateManager::m_cInstance;

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
		_STATE(MONOTONE, CStateMonotone)
		_STATE(BLOOMSTAR, CStateBloomStar)
		_STATE(BLOOMSTAR_EX, CStateBloomStarEx)
		_STATE(HDR, CStateHDR)
	}
	
	m_nNextState = STATE_NUM;
	m_nPrevState = STATE_NUM;
	m_nCurrentState = STATE_NUM;
}