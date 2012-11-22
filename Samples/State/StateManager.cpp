#include "StateManager.h"

#include "StateA.h"
#include "StateB.h"
#include "StateC.h"

CStateManager CStateManager::s_Instance;

#define _STATE(idx, clazz) \
	static clazz s_##clazz; \
	Register(idx, &s_##clazz)

CStateManager::CStateManager()
{
	_STATE(State_A, CStateA);
	_STATE(State_B, CStateB);
	_STATE(State_C, CStateC);
}
