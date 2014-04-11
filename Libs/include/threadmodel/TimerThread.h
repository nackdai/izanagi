#if !defined(__IZANAGI_THREAD_MODEL_TIMER_THREAD_H__)
#define __IZANAGI_THREAD_MODEL_TIMER_THREAD_H__

#include "izStd.h"
#include "izSystem.h"

#include "ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
    class CTimerTask : public CTask
    {
        friend class CTimerThread;

    protected:
        CTimerTask();
        virtual ~CTimerTask() {}

    private:
        void SetDelay(IZ_FLOAT ms);
        IZ_FLOAT GetDelay();

    private:
        IZ_FLOAT m_DelayMs;
    };

    class CTimerThread 
    {
    private:
        CTimerThread();
        ~CTimerThread();

    private:
        class CThread : public sys::CThread
        {
        public:
            CThread();
            virtual ~CThread();

        public:
            virtual void Run();

            void Termnate();
            IZ_BOOL IsTerminate();

            IZ_BOOL Enqueue(CStdList<CTimerTask>::Item* item);

        private:
            sys::CTimer m_Timer;
            sys::CMutex m_Mutex;
            sys::CEvent m_Event;
            CStdList<CTimerTask> m_TaskList;

            typedef CStdList<CTimerTask>::Item ListItem;

            IZ_BOOL m_WillTerminate;
        };

    public:
        static IZ_BOOL PostDelayedTask(CTimerTask* task, IZ_FLOAT delay, IZ_BOOL willDelete = IZ_FALSE);

        static void Terminate();

    private:
        enum STATE {
            STATE_NONE = 0,
            STATE_INITIALIZED,
            STATE_TERMINATED,
        };

        static CThread s_Thread;
        static STATE s_State;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TIMER_THREAD_H__)
