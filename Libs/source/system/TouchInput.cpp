#include "system/TouchInput.h"

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
}   // namespace sys
}   // namespace izanagi
