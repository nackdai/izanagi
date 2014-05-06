#if !defined(__IZANAGI_UI_GESTURE_DETECTOR_H__)
#define __IZANAGI_UI_GESTURE_DETECTOR_H__

#include "izDefs.h"
#include "izStd.h"
#include "izSystem.h"
#include "izThreadModel.h"

#include "system/TouchInput.h"

#include "VelocityTracker.h"

namespace izanagi
{
namespace ui
{
    /**
     */
    class CGestureListenerImplement : public sys::IGestureListener {
    public:
        CGestureListenerImplement() {}
        virtual ~CGestureListenerImplement() {}

    public:
        virtual void OnTapUp() {}
        virtual void OnShowPress() {}
        virtual void OnLongPress() {}
        
        virtual void OnDrag(
            const sys::CTouchEvent& ev,
            IZ_INT moveX, IZ_INT moveY) {}
        
        virtual void OnFling(
            const sys::CTouchEvent& ev,
            IZ_FLOAT velocityX, IZ_FLOAT velocityY) {}

        virtual IZ_BOOL OnDown()  { return IZ_FALSE; };
    };

    /**
     */
    class CGestureDetector {
    public:
        static IZ_UINT TAP_TIMEOUT;
        static IZ_UINT LONG_PRESS_TIMEOUT;
        static IZ_UINT TAP_RANGE_SQUARE_RADIUS;
        static IZ_FLOAT MAX_FLING_VELOCITY;
        static IZ_FLOAT MIN_FLING_VELOCITY;

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
            sys::IGestureListener* listener);

        /** Retrieve GetstureListener.
         */
        sys::IGestureListener* GetGestureListener();

        /** Post touch event to detect gesture.
         */
        IZ_BOOL PostTouchEvent(const sys::CTouchEvent& ev);

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

        void GetVelocity(IZ_FLOAT* velX, IZ_FLOAT* velY);

    protected:
        IMemoryAllocator* m_Allocator;

        sys::IGestureListener* m_Listener;

        threadmodel::CTimerTaskExecuter m_TaskExecuter;
        sys::CMutex m_Mutex;

        sys::CTouchEvent m_DownEvent;

        IZ_BOOL m_InDown;
        IZ_BOOL m_InMoving;
        IZ_BOOL m_InLongPress;

        CIntPoint m_LastPoint;

        CVelocityTracker m_VelocityTracker;
    };
}   // namespace ui
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_UI_GESTURE_DETECTOR_H__)
