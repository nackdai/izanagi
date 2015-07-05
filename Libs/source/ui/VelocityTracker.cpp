#include "ui/VelocityTracker.h"
#include "izMath.h"
#include "izSystem.h"

namespace izanagi
{
namespace ui
{
    const IZ_FLOAT CVelocityTracker::RECOG_THRESHOLD_MS = 16;

    CVelocityTracker::CVelocityTracker()
    {
        m_State = STATE_NONE;
    }

    CVelocityTracker::~CVelocityTracker()
    {
    }

    void CVelocityTracker::AddEvent(const CMotionEvent& event)
    {
        if (m_State == STATE_NONE) {
            Init(event);
        }
        else {
            Update(event);
        }
    }

    void CVelocityTracker::Clear()
    {
        m_State = STATE_NONE;
    }

    IZ_FLOAT CVelocityTracker::GetVelocityX()
    {
        return m_Velocity.x;
    }

    IZ_FLOAT CVelocityTracker::GetVelocityY()
    {
        return m_Velocity.y;
    }

    void CVelocityTracker::Init(const CMotionEvent& event)
    {
        m_PrevTime = event.time;

        m_PrevPos.x = event.x;
        m_PrevPos.y = event.y;

        m_State = STATE_INITIALIZED;
    }

    void CVelocityTracker::Update(const CMotionEvent& event)
    {
		if (sys::CTimer::GreaterThan(
                sys::CTimer::Add(m_PrevTime, RECOG_THRESHOLD_MS),
                event.time))
        {
            // If next event is faster than I think, then ignore it.
            return;
        }

        IZ_FLOAT dt = sys::CTimer::ComputeTime(m_PrevTime, event.time);
        m_PrevTime = event.time;

        // Compute velocity.
        IZ_FLOAT velX = (event.x - m_PrevPos.x) / dt;
        IZ_FLOAT velY = (event.y - m_PrevPos.y) / dt;

        if (m_State == STATE_INITIALIZED) {
            // First velocity
            m_Velocity.x = velX;
            m_Velocity.y = velY;
            m_State = STATE_UPDATED_VELOCITY;
        }
        else {
            // Compute accel.
            IZ_FLOAT accelX = (velX - m_Velocity.x) / dt;
            IZ_FLOAT accelY = (velY - m_Velocity.y) / dt;

            if (m_State == STATE_UPDATED_VELOCITY) {
                // First accel.
                m_Accel.x = accelX;
                m_Accel.y = accelY;
                m_State = STATE_UPDATED_ACCEL;
            }
            else {
                // Update accel.
                m_Accel.x += (accelX - m_Accel.x);
                m_Accel.y += (accelY - m_Accel.y);
                m_State = STATE_UPDATING;
            }

            // Update velocity.
            m_Velocity.x += m_Accel.x * dt;
            m_Velocity.y += m_Accel.y * dt;
        }

        m_PrevPos.x = event.x;
        m_PrevPos.y = event.y;
    }
}   // namespace ui
}   // namespace izanagi
