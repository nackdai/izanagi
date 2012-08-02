#include "StateManager.h"

#include "StateMonotone.h"

CStateManager CStateManager::s_Instance;

#define _STATE(type, app, camera) \
	static CState##type s_##type(app, camera); \
	Register(State_##type, &s_##type)

CStateManager::CStateManager()
{	
}

void CStateManager::Create(
	izanagi::sample::CSampleApp* app,
	izanagi::SCameraParam& camera)
{
	_STATE(Monotone, app, camera);
}
