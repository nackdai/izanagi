#include "StateManager.h"

#include "StateMirrorMap.h"

CStateManager CStateManager::s_Instance;

#define _STATE(type, param) \
	static CState##type s_##type(param); \
	Register(State_##type, &s_##type)

CStateManager::CStateManager()
{	
}

void CStateManager::Create(izanagi::SCameraParam& camera)
{
	_STATE(MirrorMap, camera);
}
