#include "StateBase.h"
#include "PhotoItemManager.h"
#include "GestureListener.h"

StateBase::StateBase(izanagi::CVectorCamera& camera)
    : m_Camera(camera)
{
}

IZ_BOOL StateBase::Render(izanagi::graph::CGraphicsDevice* device)
{
    PhotoItemManager::Instance().Render(
        device,
        m_Camera);

    return IZ_TRUE;
}

IZ_BOOL StateBase::OnMouseLBtnDown(const izanagi::CIntPoint& point)
{
    GestureDetector::Instance().PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_DOWN,
            point.x, point.y));
    return IZ_TRUE;
}

IZ_BOOL StateBase::OnMouseLBtnUp(const izanagi::CIntPoint& point)
{
    GestureDetector::Instance().PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_UP,
            point.x, point.y));
    return IZ_TRUE;
}

void StateBase::OnMouseMove(const izanagi::CIntPoint& point)
{
    GestureDetector::Instance().PostTouchEvent(
        izanagi::sys::CTouchEvent(
            izanagi::sys::E_SYS_TOUCH_EVENT_MOVE,
            point.x, point.y));
}
