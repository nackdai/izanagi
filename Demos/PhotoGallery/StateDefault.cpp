#include "StateDefault.h"

StateDefault::StateDefault(izanagi::CCamera& camera)
    : StateBase(camera)
{
}

StateDefault::~StateDefault()
{
}

IZ_BOOL StateDefault::Update()
{
    return IZ_TRUE;
}

IZ_BOOL StateDefault::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}