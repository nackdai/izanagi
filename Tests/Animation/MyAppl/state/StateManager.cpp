﻿#include <stdafx.h>
#include "StateManager.h"
#include "StateAnmList.h"
#include "StateAnmInterp.h"

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
		//_STATE(ANM_LIST, CStateAnmList)
		_STATE(ANM_LIST, CStateAnmInterp)
	}
	
	m_nNextState = STATE_NUM;
	m_nPrevState = STATE_NUM;
	m_nCurrentState = STATE_NUM;
}