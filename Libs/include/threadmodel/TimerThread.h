#if !defined(__IZANAGI_THREAD_MODEL_TIMER_THREAD_H__)
#define __IZANAGI_THREAD_MODEL_TIMER_THREAD_H__

#include "izStd.h"
#include "izSystem.h"

#include "ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CTimerTask : public CTask
    {
        friend class CTimerThread;
        friend class CTimerTaskExecuter;

    protected:
        CTimerTask();
        virtual ~CTimerTask() {}

    public:
        enum TYPE {
            TYPE_DELAY = 0,
            TYPE_INTERVAL,
        };

    private:
        inline void SetType(TYPE type);
        inline TYPE GetType();

        inline void SetTime(IZ_TIME time);
        inline IZ_TIME GetTime();

        inline void SetPrev(IZ_TIME time);
        inline IZ_TIME GetPrev();

        inline void SetInterval(IZ_FLOAT ms);
        inline IZ_FLOAT GetInterval();

        inline void SetElapsed(IZ_FLOAT ms);
        inline IZ_FLOAT GetElapsed();

        inline void SetTimeForRun(IZ_FLOAT time);

        virtual void OnRun();

    protected:
        virtual void OnRun(IZ_FLOAT time) = 0;

    private:
        TYPE m_Type;
        IZ_TIME m_Time;
        IZ_TIME m_Prev;
        IZ_FLOAT m_Interval;
        IZ_FLOAT m_Elapsed;

        IZ_FLOAT m_TempTime;
    };

    /**
     */
    class CTimerTaskExecuter {
    public:
        CTimerTaskExecuter();
        ~CTimerTaskExecuter();

        NO_COPIABLE(CTimerTaskExecuter);

    public:
        IZ_BOOL PostTask(
            CTimerTask* task, 
            CTimerTask::TYPE type,
            IZ_FLOAT time, 
            IZ_BOOL willDelete = IZ_FALSE);

        void Update();

        void Clear();

        IZ_UINT GetRegisteredTaskNum();

        void Cancel(
            IZ_BOOL (*funcIsCancel)(CTimerTask* task, void* userData),
            void* userData);

    private:
        CStdList<CTimerTask> m_TaskList;

        typedef CStdList<CTimerTask>::Item ListItem;
    };

    /**
     */
    class CTimerThread : public sys::CThread
    {
    public:
        CTimerThread();
        ~CTimerThread();

    private:
        virtual void Run();

    public:
        IZ_BOOL PostDelayedTask(
            CTimerTask* task, 
            IZ_FLOAT delay, 
            IZ_BOOL willDelete = IZ_FALSE);

        IZ_BOOL PostIntervalTask(
            CTimerTask* task, 
            IZ_FLOAT interval, 
            IZ_BOOL willDelete = IZ_FALSE);

        virtual void Join();

    private:
        IZ_BOOL PostTaskInternal(
            CTimerTask* task, 
            CTimerTask::TYPE type,
            IZ_FLOAT time, 
            IZ_BOOL willDelete = IZ_FALSE);

    private:
        enum STATE {
            STATE_NONE = 0,
            STATE_INITIALIZED,
            STATE_TERMINATED,
        };

        STATE m_State;

        sys::CTimer m_Timer;
        sys::CMutex m_Mutex;
        sys::CEvent m_Event;
        
        CTimerTaskExecuter m_TaskExecuter;

        IZ_BOOL m_WillTerminate;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TIMER_THREAD_H__)
