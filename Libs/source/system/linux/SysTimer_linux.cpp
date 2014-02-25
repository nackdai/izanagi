#include "system/SysTimer.h"

namespace izanagi
{
namespace sys
{
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
}   // namespace sys
}   // namespace izanagi
