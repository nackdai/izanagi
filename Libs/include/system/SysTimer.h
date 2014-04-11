#if !defined(__IZANAGI_SYSTEM_SYS_TIMER_H__)
#define __IZANAGI_SYSTEM_SYS_TIMER_H__

#include "izDefs.h"

namespace izanagi
{
namespace sys
{
    /**
    * Timer
    */
    class CTimer {
    public:
        static void SetTimeZero(IZ_TIME& time);

    public:
        CTimer();
        ~CTimer() {}

        NO_COPIABLE(CTimer);

    public:
        // 計測開始
        void Begin();

        // 計測終了
        IZ_FLOAT End();

        // 計測時間取得
        IZ_FLOAT GetTime() { return m_fTime; }

        // 現在の時間取得
        static IZ_TIME GetCurTime();

        // 差分から計算
        static IZ_FLOAT ComputeTime(IZ_TIME begin, IZ_TIME end);

    protected:
        IZ_TIME m_Begin;

        IZ_FLOAT m_fTime;
    };
}   // namespace sys
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_SYSTEM_SYS_TIMER_H__)
