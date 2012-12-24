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
        m_Flags.is_reverse = IZ_TRUE;
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
        m_Flags.isPause = IZ_TRUE;

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
        m_Flags.isPause = rhs.m_Flags.isPause;
        m_Flags.isForward = rhs.m_Flags.isForward;

        return *this;
    }

    /**
    * 初期化
    */
    void CTimeline::Init(
        IZ_FLOAT duration,
        IZ_FLOAT delay)
    {
        m_Duration = duration;
        m_Delay = delay;
    }

    /**
    * リセット
    */
    void CTimeline::Reset()
    {
        m_Time = 0.0f;
        m_DelayTime = 0.0f;
    }

    /**
    * ストップ
    */
    void CTimeline::Stop()
    {
        m_Time = (IsForwardDir() ? 0.0f : m_Duration);
        m_DelayTime = 0.0f;

        m_Flags.isPause = IZ_TRUE;
    }

    /**
    * 進行
    */
    void CTimeline::Advance(IZ_FLOAT delta)
    {
        if (IsPause()) {
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

        IZ_BOOL bIsOver = IZ_FALSE;
        
        if (IsForwardDir()) {
            m_Time += delta;
            bIsOver = (m_Time > m_Duration);
        }
        else {
            // 逆方向
            m_Time -= delta;
            bIsOver = (m_Time < 0.0f);
        }

        if (bIsOver) {
            if (m_TimeOverHandler != IZ_NULL) {
                m_TimeOverHandler->Handle(*this);
            }

            if (IsLoop()) {
                if (IsReverse()) {
                    // 向きを変える
                    Rewind();
                }
            }

            // リセット
            Stop();

            if (IsLoop()) {
                // ループするので再開
                Start();
            }
        }
    }

    /**
    * [0.0f - 1.0f]で取得
    */
    IZ_FLOAT CTimeline::GetFraction() const
    {
        IZ_FLOAT ret = (m_Duration != 0.0f ? m_Time / m_Duration : 0.0f);
        return ret;
    }

}   // namespace animation
}   // namespace izanagi
