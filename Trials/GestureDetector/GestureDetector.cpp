#include "GestureDetector.h"
#include "izMath.h"

namespace izanagi
{
namespace sys
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
        IGestureListener* listener)
    {
        if (m_Allocator != IZ_NULL) {
            return IZ_TRUE;
        }

        IZ_ASSERT(allocator != IZ_NULL);
        IZ_ASSERT(listener != IZ_NULL);

        m_Allocator = allocator;
        m_Listener = listener;

        return IZ_TRUE;
    }

    IGestureListener* CGestureDetector::GetGestureListener()
    {
        IZ_ASSERT(m_Listener != IZ_NULL);
        return m_Listener;
    }

    IZ_BOOL CGestureDetector::PostTouchEvent(const CTouchEvent& ev)
    {
        IZ_ASSERT(m_Listener != IZ_NULL);

        switch (ev.type) {
        case E_SYS_TOUCH_EVENT_UP:
            {
                if (m_InLongPress) {
                    Cancel(TYPE_TAP);
                    m_InLongPress = IZ_FALSE;
                }
                else if (m_InMoving) {
                }
                else {
                    m_Listener->OnTapUp();
                }

                m_InMoving = IZ_FALSE;
                m_InDown = IZ_FALSE;

                Cancel(TYPE_SHOW_PRESS);
                Cancel(TYPE_LONG_PRESS);
            }
            break;
        case E_SYS_TOUCH_EVENT_DOWN:
            {
                Cancel(TYPE_LONG_PRESS);

                m_LastPoint.x = ev.x;
                m_LastPoint.y = ev.y;

                CTask* taskLongPress = threadmodel::CTask::CreateTask<CTask>(m_Allocator);
                taskLongPress->SetGestureType(TYPE_LONG_PRESS);
                taskLongPress->SetGestureDetector(this);
                m_TaskExecuter.PostTask(
                    taskLongPress,
                    threadmodel::CTimerTask::TYPE_DELAY,
                    TAP_TIMEOUT + LONG_PRESS_TIMEOUT,
                    IZ_TRUE);

                CTask* taskShowPress = threadmodel::CTask::CreateTask<CTask>(m_Allocator);
                taskShowPress->SetGestureType(TYPE_SHOW_PRESS);
                taskShowPress->SetGestureDetector(this);
                m_TaskExecuter.PostTask(
                    taskShowPress,
                    threadmodel::CTimerTask::TYPE_DELAY,
                    TAP_TIMEOUT,
                    IZ_TRUE);

                m_InDown = IZ_TRUE;

                m_Listener->OnDown();
            }
            break;
        case E_SYS_TOUCH_EVENT_MOVE:
            {
                if (m_InDown) {
                    IZ_INT moveX = m_LastPoint.x - ev.x;
                    IZ_INT moveY = m_LastPoint.y - ev.y;
                    IZ_UINT distance = moveX * moveX + moveY * moveY;

                    if (distance > TAP_RANGE_SQUARE_RADIUS) {
                        m_Listener->OnDrag(
                            ev.x, ev.y,
                            m_LastPoint.x, m_LastPoint.y);

                        m_LastPoint.x = ev.x;
                        m_LastPoint.y = ev.y;

                        m_InDown = IZ_FALSE;
                        m_InMoving = IZ_TRUE;

                        Cancel(TYPE_TAP);
                        Cancel(TYPE_SHOW_PRESS);
                        Cancel(TYPE_LONG_PRESS);
                    }
                }
                else if (m_InMoving) {
                    IZ_INT moveX = m_LastPoint.x - ev.x;
                    IZ_INT moveY = m_LastPoint.y - ev.y;

                    if (::abs(moveX) >= 1 || ::abs(moveY) >= 1) {
                        m_Listener->OnDrag(
                            ev.x, ev.y,
                            m_LastPoint.x, m_LastPoint.y);

                        m_LastPoint.x = ev.x;
                        m_LastPoint.y = ev.y;
                    }
                }
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
}   // namespace sys
}   // namespace izanagi
