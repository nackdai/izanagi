#if !defined(__IZANAGI_SYSTEM_GESTURE_DETECTOR_H__)
#define __IZANAGI_SYSTEM_GESTURE_DETECTOR_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSystem.h"
#include "izThreadModel.h"

namespace izanagi
{
namespace sys
{
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
            IZ_INT x, IZ_INT y,
            IZ_INT lastX, IZ_INT lastY) = 0;
        virtual void OnFling() = 0;

        virtual IZ_BOOL OnDown() = 0;
    };

    /**
     */
    class CGestureListenerImplement : public IGestureListener {
    public:
        CGestureListenerImplement() {}
        virtual ~CGestureListenerImplement() {}

    public:
        virtual void OnTapUp() {}
        virtual void OnShowPress() {}
        virtual void OnLongPress() {}
        virtual void OnDrag(
            IZ_INT x, IZ_INT y,
            IZ_INT lastX, IZ_INT lastY) {}
        virtual void OnFling() {}

        virtual IZ_BOOL OnDown()  { return IZ_FALSE; };
    };

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

        CTouchEvent(
            E_SYS_TOUCH_EVENT _type,
            IZ_INT _x,
            IZ_INT _y)
        {
            type = _type;
            x = _x;
            y = _y;
        }
        ~CTouchEvent() {}
    };

    /**
     */
    class CGestureDetector {
    public:
        static IZ_UINT TAP_TIMEOUT;
        static IZ_UINT LONG_PRESS_TIMEOUT;
        static IZ_UINT TAP_RANGE_SQUARE_RADIUS;

    public:
        CGestureDetector();
        CGestureDetector(IMemoryAllocator* allocator);

        virtual ~CGestureDetector();

        NO_COPIABLE(CGestureDetector);

    public:
        /** Initialize.
         */
        IZ_BOOL Init(
            IMemoryAllocator* allocator,
            IGestureListener* listener);

        /** Retrieve GetstureListener.
         */
        IGestureListener* GetGestureListener();

        /** Post touch event to detect gesture.
         */
        IZ_BOOL PostTouchEvent(const CTouchEvent& ev);

        /** Update.
         */
        void Update();

    private:
        enum TYPE {
            TYPE_SHOW_PRESS = 0,
            TYPE_LONG_PRESS,
            TYPE_TAP,
        };

        class CTask : public threadmodel::CTimerTask {
            friend class threadmodel::CTask;

        public:
            CTask();
            virtual ~CTask();

        public:
            virtual void OnRun(IZ_FLOAT time);

            inline void SetGestureDetector(CGestureDetector* detector);

            inline void SetGestureType(CGestureDetector::TYPE type);
            inline CGestureDetector::TYPE GetGestureType();

        private:
            CGestureDetector* m_Detector;
            CGestureDetector::TYPE m_Type;
        };

        static IZ_BOOL IsCancel(
            threadmodel::CTimerTask* task,
            void* data);

        void HandleMessage(TYPE type);

        void Cancel(TYPE type);

    protected:
        IMemoryAllocator* m_Allocator;

        IGestureListener* m_Listener;
        threadmodel::CMessageQueue m_MsgQueue;

        threadmodel::CTimerTaskExecuter m_TaskExecuter;
        sys::CMutex m_Mutex;

        IZ_BOOL m_InDown;
        IZ_BOOL m_InMoving;
        IZ_BOOL m_InLongPress;

        CIntPoint m_LastPoint;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_GESTURE_DETECTOR_H__)
