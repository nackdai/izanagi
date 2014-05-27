#include "system/TouchInput.h"
#include "system/SysTimer.h"

namespace izanagi
{
namespace sys
{
    IGestureListener* CTouchInput::s_Listener = IZ_NULL;

    IZ_BOOL CTouchInput::IsDependedOnSystem()
    {
        return IZ_FALSE;
    }

    void CTouchInput::SetGestureListener(IGestureListener* listener)
    {
        IZ_ASSERT(IZ_FALSE);
    }

    IGestureListener* CTouchInput::GetGestureListener()
    {
        IZ_ASSERT(IZ_FALSE);
        return IZ_NULL;
    }

    CTouchEvent::CTouchEvent(
        E_SYS_TOUCH_EVENT _type,
        IZ_INT _x,
        IZ_INT _y)
    {
        type = _type;
        x = _x;
        y = _y;

        CTimer::SetTimeZero(eventTime);
    }
}   // namespace sys
}   // namespace izanagi
