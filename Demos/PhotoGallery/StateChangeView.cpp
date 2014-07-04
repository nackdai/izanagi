#include "StateChangeView.h"

StateChangeView::StateChangeView(izanagi::CCamera& camera)
    : StateBase(camera)
{
}

StateChangeView::~StateChangeView()
{
}

IZ_BOOL StateChangeView::Update()
{
    return IZ_TRUE;
}

IZ_BOOL StateChangeView::OnKeyDown(izanagi::sys::E_KEYBOARD_BUTTON key)
{
    return IZ_TRUE;
}
