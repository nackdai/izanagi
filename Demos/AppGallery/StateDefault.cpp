#include "StateDefault.h"
#include "StateManager.h"
#include "GestureListener.h"

StateDefault::StateDefault(izanagi::CVectorCamera& camera)
    : StateBase(camera)
{
}

StateDefault::~StateDefault()
{
}

IZ_BOOL StateDefault::Update(
    IZ_FLOAT time,
    izanagi::graph::CGraphicsDevice* device)
{
    GestureDetector::Instance().Update();
    return IZ_TRUE;
}

IZ_BOOL StateDefault::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}
