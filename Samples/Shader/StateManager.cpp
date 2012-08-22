#include "StateManager.h"

#include "StateMirrorMap.h"
#include "StatePhongShader.h"
#include "StatePointLight.h"
#include "StateSpotLight.h"
#include "StateNormalMap.h"

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
	_STATE(MirrorMap, app, camera);
	_STATE(PhongShader, app, camera);
    _STATE(PointLight, app, camera);
    _STATE(SpotLight, app, camera);
    _STATE(NormalMap, app, camera);
}
