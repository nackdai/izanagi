#if !defined(__IZANAGI_SYSTEM_TOUCH_INPUT_H__)
#define __IZANAGI_SYSTEM_TOUCH_INPUT_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
namespace sys
{
    /**
     */
    enum E_SYS_TOUCH_EVENT {
        E_SYS_TOUCH_EVENT_DOWN = 0,
        E_SYS_TOUCH_EVENT_UP,
        E_SYS_TOUCH_EVENT_MOVE,
        E_SYS_TOUCH_EVENT_CANCEL,

        E_SYS_TOUCH_EVENT_NUM,
        E_SYS_TOUCH_EVENT_FORCE_INT32 = 0x7fffffff,
    };

    /**
     */
    class CTouchEvent {
    public:
        E_SYS_TOUCH_EVENT type;
        IZ_INT x;
        IZ_INT y;

        IZ_TIME eventTime;

        CTouchEvent(
            E_SYS_TOUCH_EVENT _type,
            IZ_INT _x,
            IZ_INT _y);

        CTouchEvent() {}
        ~CTouchEvent() {}
    };

    /**
     */
    class IGestureListener {
    protected:
        IGestureListener() {}
        virtual ~IGestureListener() {}

        NO_COPIABLE(IGestureListener);

    public:
        virtual void OnTapUp() = 0;
        virtual void OnShowPress() = 0;
        virtual void OnLongPress() = 0;
        
        virtual void OnDrag(
            const CTouchEvent& ev,
            IZ_INT moveX, IZ_INT moveY) = 0;

        virtual void OnFling(
            const CTouchEvent& ev,
            IZ_FLOAT velocityX, IZ_FLOAT velocityY) = 0;

        virtual IZ_BOOL OnDown() = 0;
    };

    /**
     */
    class CTouchInput {
    private:
        CTouchInput() {}
        ~CTouchInput() {}

        NO_COPIABLE(CTouchInput);

    public:
        static IZ_BOOL IsDependedOnSystem();

        static void SetGestureListener(IGestureListener* listener);

        static IGestureListener* GetGestureListener();

    private:
        static IGestureListener* s_Listener;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_TOUCH_INPUT_H__)
