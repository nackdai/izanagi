#include <math.h>
#include "animation/Timeline.h"

namespace izanagi {
namespace animation {

    // コンストラクタ
    CTimeline::CTimeline()
    {
        m_Time = 0.0f;
        m_DelayTime = 0.0f;

        m_Duration = 0.0f;
        m_Delay = 0.0f;

        m_Flags.isLoop = IZ_FALSE;
        m_Flags.isReverse = IZ_TRUE;
        m_Flags.isPause = IZ_TRUE;
        m_Flags.isForward = IZ_TRUE;

        m_TimeOverHandler = IZ_NULL;

        m_ListItem.Init(this);
    }

    CTimeline::CTimeline(
        IZ_FLOAT duration,
        IZ_FLOAT delay)
    {
        m_Time = 0.0f;
        m_DelayTime = 0.0f;

        m_Duration = duration;
        m_Delay = delay;

        m_Flags.isLoop = IZ_FALSE;
        m_Flags.isReverse = IZ_TRUE;
        m_Flags.isPause = IZ_TRUE;
        m_Flags.isForward = IZ_TRUE;

        m_TimeOverHandler = IZ_NULL;

        m_ListItem.Init(this);
    }

    CTimeline::CTimeline(const CTimeline& rhs)
    {
        *this = rhs;
    }

    const CTimeline& CTimeline::operator=(const CTimeline& rhs)
    {
        m_Time = rhs.m_Time;
        m_DelayTime = rhs.m_DelayTime;

        m_Duration = rhs.m_Duration;
        m_Delay = rhs.m_Delay;

        m_Flags.isLoop = rhs.m_Flags.isLoop;
        m_Flags.isReverse = rhs.m_Flags.isReverse;
        m_Flags.isPause = rhs.m_Flags.isPause;
        m_Flags.isForward = rhs.m_Flags.isForward;

        m_TimeOverHandler = rhs.m_TimeOverHandler;

        return *this;
    }

    // 初期化
    void CTimeline::Init(
        IZ_FLOAT duration,
        IZ_FLOAT delay)
    {
        m_Duration = duration;
        m_Delay = delay;
    }

    void CTimeline::Start()
    {
        m_Flags.isPause = IZ_FALSE;
    }

    // ストップ
    void CTimeline::Stop()
    {
        m_Time = (IsForward() ? 0.0f : m_Duration);
        m_Flags.isPause = IZ_TRUE;
    }

    void CTimeline::Pause()
    {
        m_Flags.isPause = IZ_TRUE;
    }

    // リセット
    void CTimeline::Reset()
    {
        m_Time = 0.0f;
        m_DelayTime = 0.0f;
        m_Flags.isForward = IZ_TRUE;
    }

    // 進行
    void CTimeline::Advance(IZ_FLOAT delta)
    {
        if (IsPaused()) {
            // ポーズ中なので何もしない
            return;
        }

        if (m_DelayTime <= m_Delay) {
            // 遅延
            m_DelayTime += delta;

            if (m_DelayTime <= m_Delay) {
                // まだまだ始まらない
                return;
            }

            delta = m_DelayTime - m_Delay;
        }

        IZ_BOOL isOver = IZ_FALSE;
        IZ_FLOAT overTime = 0.0f;
        
        if (IsForward()) {
            m_Time += delta;
            isOver = (m_Time >= m_Duration);
        }
        else {
            // 逆方向
            m_Time -= delta;
            isOver = (m_Time <= 0.0f);
        }

        if (isOver) {
            overTime = (IsForward() ? m_Time - m_Duration : abs(m_Time));

            if (IsLoop() && WillReverseIfLoop()) {
                // 向きを変える
                Rewind();
            }

            Stop();

            if (IsLoop()) {
                // ループするので再開
                Start();

                if (IsForward()) {
                    m_Time += overTime;
                }
                else {
                    m_Time -= overTime;
                }
            }

			if (m_TimeOverHandler != IZ_NULL) {
                m_TimeOverHandler->Handle(*this);
            }
        }
    }

    void CTimeline::Rewind()
    {
        m_Flags.isForward = !m_Flags.isForward;
    }

    void CTimeline::SetTimeOverHandler(CTimeOverHandler* handler)
    {
        m_TimeOverHandler = handler;
    }

    IZ_FLOAT CTimeline::GetTime() const
    {
        return m_Time;
    }

    IZ_FLOAT CTimeline::GetDuration() const
    {
        return m_Duration;
    }

    IZ_FLOAT CTimeline::GetNormalized() const
    {
        IZ_FLOAT ret = (m_Duration != 0.0f ? m_Time / m_Duration : 0.0f);
        return ret;
    }

    void CTimeline::EnableLoop(IZ_BOOL enable)
    {
        m_Flags.isLoop = enable;
    }

    IZ_BOOL CTimeline::IsLoop() const
    {
        return m_Flags.isLoop;
    }

    void CTimeline::EnableReverseIfLoop(IZ_BOOL enable)
    {
        m_Flags.isReverse = enable;
    }

    IZ_BOOL CTimeline::WillReverseIfLoop() const
    {
        return m_Flags.isReverse;
    }

    IZ_BOOL CTimeline::IsPaused() const
    {
        return m_Flags.isPause;
    }

    IZ_BOOL CTimeline::IsForward() const
    {
        return m_Flags.isForward;
    }

    CStdList<CTimeline>::Item* CTimeline::GetListItem()
    {
        return &m_ListItem;
    }

}   // namespace animation
}   // namespace izanagi
