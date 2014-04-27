#if !defined(__IZANAGI_SYSTEM_VELOCITY_TRACKER_H__)
#define __IZANAGI_SYSTEM_VELOCITY_TRACKER_H__

#include "izDefs.h"
#include "izStd.h"

namespace izanagi
{
namespace sys
{
    /**
     */
    class CMotionEvent {
    public:
        IZ_TIME time;
        IZ_INT x;
        IZ_INT y;

        CMotionEvent(IZ_TIME _time, IZ_INT _x, IZ_INT _y)
        {
            time = _time;
            x = _x;
            y = _y;
        }
        ~CMotionEvent() {}
    };

    /**
     */
    class CVelocityTracker {
    public:
        static const IZ_FLOAT RECOG_THRESHOLD_MS;

    public:
        CVelocityTracker();
        ~CVelocityTracker();

    public:
        void AddEvent(const CMotionEvent& event);

        void Clear();

        IZ_FLOAT GetVelocityX();
        IZ_FLOAT GetVelocityY();

    private:
        void Init(const CMotionEvent& event);

        void Update(const CMotionEvent& event);

    private:
        enum STATE {
            STATE_NONE = 0,
            STATE_INITIALIZED,
            STATE_UPDATED_VELOCITY,
            STATE_UPDATED_ACCEL,
            STATE_UPDATING,
        };

        STATE m_State;;

        IZ_TIME m_PrevTime;

        CIntPoint m_PrevPos;
        CFloatPoint m_Velocity;
        CFloatPoint m_Accel;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_VELOCITY_TRACKER_H__)
