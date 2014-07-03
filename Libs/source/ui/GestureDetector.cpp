#include "ui/GestureDetector.h"
#include "izMath.h"

namespace izanagi
{
namespace ui
{
    CGestureDetector::CTask::CTask()
    {
        m_Detector = IZ_NULL;
    }

    CGestureDetector::CTask::~CTask()
    {
    }

    void CGestureDetector::CTask::OnRun(IZ_FLOAT time)
    {
        m_Detector->HandleMessage(m_Type);
    }

    void CGestureDetector::CTask::SetGestureDetector(CGestureDetector* detector)
    {
        m_Detector = detector;
    }

    void CGestureDetector::CTask::SetGestureType(CGestureDetector::TYPE type)
    {
        m_Type = type;
    }

    CGestureDetector::TYPE CGestureDetector::CTask::GetGestureType()
    {
        return m_Type;
    }

    //////////////////////////////////////////////////

    IZ_UINT CGestureDetector::TAP_TIMEOUT = 500;
    IZ_UINT CGestureDetector::LONG_PRESS_TIMEOUT = 1000;
    IZ_UINT CGestureDetector::TAP_RANGE_SQUARE_RADIUS = 1;
    IZ_FLOAT CGestureDetector::MAX_FLING_VELOCITY = 1.0f;
    IZ_FLOAT CGestureDetector::MIN_FLING_VELOCITY = 0.2f;

    CGestureDetector::CGestureDetector()
    {
        m_Allocator = IZ_NULL;
        m_Listener = IZ_NULL;

        m_InDown = IZ_FALSE;
        m_InMoving = IZ_FALSE;
        m_InLongPress = IZ_FALSE;
    }

    CGestureDetector::~CGestureDetector()
    {
    }

    IZ_BOOL CGestureDetector::Init(
        IMemoryAllocator* allocator,
        sys::IGestureListener* listener)
    {
        if (sys::CTouchInput::IsDependedOnSystem()) {
            sys::CTouchInput::SetGestureListener(listener);
        }
        else {
            if (m_Allocator != IZ_NULL) {
                return IZ_TRUE;
            }

            IZ_ASSERT(allocator != IZ_NULL);
            IZ_ASSERT(listener != IZ_NULL);

            m_Allocator = allocator;
            m_Listener = listener;
        }

        return IZ_TRUE;
    }

    sys::IGestureListener* CGestureDetector::GetGestureListener()
    {
        if (sys::CTouchInput::IsDependedOnSystem()) {
            return sys::CTouchInput::GetGestureListener();
        }
        else {
            IZ_ASSERT(m_Listener != IZ_NULL);
            return m_Listener;
        }
    }

    IZ_BOOL CGestureDetector::PostTouchEvent(const sys::CTouchEvent& ev)
    {
        if (sys::CTouchInput::IsDependedOnSystem()) {
            return IZ_TRUE;
        }

        IZ_ASSERT(m_Listener != IZ_NULL);

        IZ_TIME cur = sys::CTimer::GetCurTime();

        switch (ev.type) {
        case sys::E_SYS_TOUCH_EVENT_UP:
            {
                if (m_InLongPress) {
                    Cancel(TYPE_TAP);
                    m_InLongPress = IZ_FALSE;
                }
                else if (m_InMoving) {
                    IZ_FLOAT velX, velY;
                    GetVelocity(&velX, &velY);

                    if (math::CMath::Absf(velX) >= MIN_FLING_VELOCITY
                        || math::CMath::Absf(velY) >= MIN_FLING_VELOCITY)
                    {
                        m_Listener->OnFling(m_DownEvent, velX, velY);
                        m_VelocityTracker.Clear();
                    }
                }
                else {
                    m_Listener->OnTapUp(ev);
                }

                m_InMoving = IZ_FALSE;
                m_InDown = IZ_FALSE;

                Cancel(TYPE_SHOW_PRESS);
                Cancel(TYPE_LONG_PRESS);
            }
            break;
        case sys::E_SYS_TOUCH_EVENT_DOWN:
            {
                Cancel(TYPE_LONG_PRESS);

                // Keep touch point.
                m_LastPoint.x = ev.x;
                m_LastPoint.y = ev.y;

                // Post task for long press.
                CTask* taskLongPress = threadmodel::CTask::CreateTask<CTask>(m_Allocator);
                taskLongPress->SetGestureType(TYPE_LONG_PRESS);
                taskLongPress->SetGestureDetector(this);
                m_TaskExecuter.PostTask(
                    taskLongPress,
                    threadmodel::CTimerTask::TYPE_DELAY,
                    cur,
                    TAP_TIMEOUT + LONG_PRESS_TIMEOUT,
                    IZ_TRUE);

                // Post task for showing press.
                CTask* taskShowPress = threadmodel::CTask::CreateTask<CTask>(m_Allocator);
                taskShowPress->SetGestureType(TYPE_SHOW_PRESS);
                taskShowPress->SetGestureDetector(this);
                m_TaskExecuter.PostTask(
                    taskShowPress,
                    threadmodel::CTimerTask::TYPE_DELAY,
                    cur,
                    TAP_TIMEOUT,
                    IZ_TRUE);

                m_InDown = IZ_TRUE;

                // Keep touch event.
                m_DownEvent = ev;
                m_DownEvent.eventTime = cur;

                // Add event to track velocity.
                m_VelocityTracker.Clear();
                m_VelocityTracker.AddEvent(
                    CMotionEvent(cur, ev.x, ev.y));

                // Dispatch down event.
                m_Listener->OnDown();
            }
            break;
        case sys::E_SYS_TOUCH_EVENT_MOVE:
            {
                if (m_InDown) {
                    // Compute how distance moved.
                    IZ_INT moveX = m_LastPoint.x - ev.x;
                    IZ_INT moveY = m_LastPoint.y - ev.y;
                    IZ_UINT distance = moveX * moveX + moveY * moveY;

                    if (distance > TAP_RANGE_SQUARE_RADIUS) {
                        // Dispatch first drag event.
                        m_Listener->OnDrag(
                            m_DownEvent,
                            moveX, moveY);

                        m_LastPoint.x = ev.x;
                        m_LastPoint.y = ev.y;

                        m_InDown = IZ_FALSE;
                        m_InMoving = IZ_TRUE;

                        // Cancel all press events.
                        Cancel(TYPE_TAP);
                        Cancel(TYPE_SHOW_PRESS);
                        Cancel(TYPE_LONG_PRESS);
                    }
                }
                else if (m_InMoving) {
                    // Compute how distance moved.
                    IZ_INT moveX = m_LastPoint.x - ev.x;
                    IZ_INT moveY = m_LastPoint.y - ev.y;

                    if (math::CMath::Absf(moveX) >= 1 || math::CMath::Absf(moveY) >= 1) {
                        // Dispatch drag event.
                        m_Listener->OnDrag(
                            m_DownEvent,
                            moveX, moveY);

                        m_LastPoint.x = ev.x;
                        m_LastPoint.y = ev.y;
                    }
                }

                m_VelocityTracker.AddEvent(
                    CMotionEvent(cur, ev.x, ev.y));
            }
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            return IZ_FALSE;
        }

        return IZ_TRUE;
    }

    void CGestureDetector::Update()
    {
        if (sys::CTouchInput::IsDependedOnSystem()) {
            return;
        }

        m_TaskExecuter.Update();
    }

    IZ_BOOL CGestureDetector::IsCancel(
        threadmodel::CTimerTask* task,
        void* data)
    {
        IZ_ASSERT(task != IZ_NULL);
        IZ_ASSERT(data != IZ_NULL);

        TYPE type = *(TYPE*)data;

        CTask* gestureTask = reinterpret_cast<CTask*>(task);

        return (gestureTask->GetGestureType() == type);
    }

    void CGestureDetector::HandleMessage(TYPE type)
    {
        IZ_ASSERT(m_Listener != IZ_NULL);

        switch (type) {
        case TYPE_SHOW_PRESS:
            m_Listener->OnShowPress();
            break;
        case TYPE_LONG_PRESS:
            {
                m_InDown = IZ_FALSE;
                m_InMoving = IZ_FALSE;
                m_InLongPress = IZ_TRUE;
                m_Listener->OnLongPress();
            }
            break;
        default:
            IZ_ASSERT(IZ_FALSE);
            break;
        }
    }

    void CGestureDetector::Cancel(TYPE type)
    {
        m_TaskExecuter.Cancel(
            CGestureDetector::IsCancel,
            &type);
    }

    void CGestureDetector::GetVelocity(IZ_FLOAT* velX, IZ_FLOAT* velY)
    {
        if (velX) {
            *velX = CLAMP(
                m_VelocityTracker.GetVelocityX(),
                -MAX_FLING_VELOCITY,
                MAX_FLING_VELOCITY);
        }

        if (velY) {
            *velY = CLAMP(
                m_VelocityTracker.GetVelocityY(),
                -MAX_FLING_VELOCITY,
                MAX_FLING_VELOCITY);
        }
    }
}   // namespace ui
}   // namespace izanagi
