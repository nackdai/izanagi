#include "StateDefault.h"
#include "StateManager.h"
#include "GestureListener.h"
#include "StateChangeView.h"
#include "PhotoItemManager.h"

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
    if (key == izanagi::sys::E_KEYBOARD_BUTTON_UP) {
        StateManager::Instance().ChangeState(
            State_ChangeView, 
            izanagi::CValue(StateChangeView::ChangeViewModeUp));
    }
    else if (key == izanagi::sys::E_KEYBOARD_BUTTON_DOWN) {
        StateManager::Instance().ChangeState(
            State_ChangeView, 
            izanagi::CValue(StateChangeView::ChangeViewModeDown));
    }
    return IZ_TRUE;
}
