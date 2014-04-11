#include <windows.h>
#include <mmsystem.h>
#include "system/SysTimer.h"

namespace izanagi
{
namespace sys
{
    static IZ_TIME s_Frequency = 0;

    static inline void InitTimer()
    {
        if (s_Frequency == 0) {
            LARGE_INTEGER sFreq;
            FILL_ZERO(&sFreq, sizeof(sFreq));

            IZ_BOOL result = QueryPerformanceFrequency(&sFreq);
            IZ_ASSERT(result);

            s_Frequency = sFreq.QuadPart;
        }
    }

    void CTimer::SetTimeZero(IZ_TIME& time)
    {
        time = 0;
    }

    /**
    * コンストラクタ
    */
    CTimer::CTimer()
    {
        m_fTime = 0.0f;
        InitTimer();
    }

    /**
    * 計測開始
    */
    void CTimer::Begin()
    {
        LARGE_INTEGER sBegin;
        FILL_ZERO(&sBegin, sizeof(sBegin));

        IZ_BOOL result = QueryPerformanceCounter(&sBegin);
        IZ_ASSERT(result);

        m_Begin = sBegin.QuadPart;
    }

    /**
    * 計測終了
    */
    IZ_FLOAT CTimer::End()
    {
        LARGE_INTEGER cur;
        QueryPerformanceCounter(&cur);

        m_fTime = (cur.QuadPart - m_Begin) * 1000.0f / s_Frequency;
        return m_fTime;
    }

    // 現在の時間取得
    IZ_INT64 CTimer::GetCurTime()
    {
        LARGE_INTEGER time;
        FILL_ZERO(&time, sizeof(time));

        IZ_BOOL result = QueryPerformanceCounter(&time);
        IZ_ASSERT(result);

        IZ_INT64 ret = time.QuadPart;
        return ret;
    }

    // 差分から計算
    IZ_FLOAT CTimer::ComputeTime(IZ_TIME begin, IZ_TIME end)
    {
        InitTimer();
        IZ_FLOAT ret = (end - begin) * 1000.0f / s_Frequency;
        return ret;
    }
}   // namespace sys
}   // namespace izanagi
