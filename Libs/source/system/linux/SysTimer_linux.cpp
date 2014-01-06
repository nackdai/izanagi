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

        m_Begin = begin.tv_sec;
    }

    /**
    * 計測終了
    */
    IZ_FLOAT CTimer::End()
    {
        timeval cur;
        gettimeofday(&cur, NULL);

        m_fTime = (cur.tv_sec - m_Begin) * 1000.0f;
        return m_fTime;
    }

    // 現在の時間取得
    IZ_TIME CTimer::GetCurTime()
    {
        timeval cur;
        gettimeofday(&cur, NULL);

        return cur.tv_sec;
    }

    // 差分から計算
    IZ_FLOAT CTimer::ComputeTime(IZ_TIME begin, IZ_TIME end)
    {
        IZ_FLOAT ret = (end - begin) * 1000.0f;
        return ret;
    }
}   // namespace sys
}   // namespace izanagi
