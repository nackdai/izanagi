#include "system/SysTimer.h"

namespace izanagi
{
namespace sys
{
    void CTimer::SetTimeZero(IZ_TIME& time)
    {
        time.tv_sec = 0;
        time.tv_usec = 0;
    }

    /**
    * コンストラクタ
    */
    CTimer::CTimer()
    {
        m_fTime = 0.0f;
    }

    /**
    * 計測開始
    */
    void CTimer::Begin()
    {
        timeval begin;
        gettimeofday(&begin, NULL);

        m_Begin = begin;
    }

    /**
    * 計測終了
    */
    IZ_FLOAT CTimer::End()
    {
        timeval cur;
        gettimeofday(&cur, NULL);

        m_fTime = ComputeTime(m_Begin, cur);
        return m_fTime;
    }

    // 現在の時間取得
    IZ_TIME CTimer::GetCurTime()
    {
        timeval cur;
        gettimeofday(&cur, NULL);

        return cur;
    }

    // 差分から計算
    IZ_FLOAT CTimer::ComputeTime(IZ_TIME begin, IZ_TIME end)
    {
        long sec = end.tv_sec - begin.tv_sec;
        long usec = end.tv_usec - begin.tv_usec;
        IZ_FLOAT msec = sec * 1000.0f + (IZ_FLOAT)usec / 1000.0f;
        return msec;
    }

    IZ_TIME CTimer::ConvertTime(IZ_FLOAT time)
    {
        IZ_TIME ret;
        ret.tv_sec = (long)(time / 1000.0f);

        time -= ret.tv_sec * 1000.0f;
        ret.tv_usec = (long)(time * 1000.0f);

        return ret;
    }

    IZ_TIME CTimer::Add(IZ_TIME time1, IZ_TIME time2)
    {
        IZ_TIME ret = {
            time1.tv_sec + time2.tv_sec,
            time1.tv_usec + time2.tv_usec,
        };

        return ret;
    }

    IZ_TIME CTimer::Add(IZ_TIME time1, IZ_FLOAT time2)
    {
        IZ_TIME time = ConvertTime(time2);
        return Add(time1, time);
    }

    IZ_TIME CTimer::Sub(IZ_TIME time1, IZ_TIME time2)
    {
        IZ_ASSERT(time1.tv_sec >= time2.tv_sec);
        IZ_FLOAT time = ComputeTime(time1, time2);
        IZ_TIME ret = ConvertTime(time);
        return ret;
    }

    IZ_TIME CTimer::Sub(IZ_TIME time1, IZ_FLOAT time2)
    {
        IZ_TIME time = ConvertTime(time2);
        return Sub(time1, time);
    }

	IZ_BOOL CTimer::GreaterThan(IZ_TIME time1, IZ_TIME time2)
    {
        if (time1.tv_sec >= time2.tv_sec) {
            return IZ_TRUE;
        }
        else if (time1.tv_sec < time2.tv_sec) {
            return IZ_FALSE;
        }
        else if (time1.tv_usec >= time2.tv_usec)
        {
            return IZ_TRUE;
        }

        return IZ_FALSE;
    }
}   // namespace sys
}   // namespace izanagi
