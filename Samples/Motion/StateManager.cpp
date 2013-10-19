#include "StateManager.h"

#include "StateCollada.h"
#include "StateXFile.h"

CStateManager CStateManager::s_Instance;

#define _STATE(type, app) \
    static CState##type s_##type(app); \
    Register(State_##type, &s_##type)

CStateManager::CStateManager()
{    
}

void CStateManager::Create(izanagi::sample::CSampleApp* app)
{
    _STATE(Collada, app);
    _STATE(XFile, app);
}
