#include "StateManager.h"

#include "StateConstColor.h"

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
		_STATE(CONST_COLOR, CStateConstColor)
	}
	
	m_nNextState = STATE_NUM;
	m_nPrevState = STATE_NUM;
	m_nCurrentState = STATE_NUM;
}