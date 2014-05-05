#include "threadmodel/TimerThread.h"

namespace izanagi
{
namespace threadmodel
{
    CTimerTask::CTimerTask()
    {
        m_Type = TYPE_DELAY;
        sys::CTimer::SetTimeZero(m_Time);
        sys::CTimer::SetTimeZero(m_Prev);
        m_Interval = 0;
        m_Elapsed = 0;
    }

    void CTimerTask::SetType(CTimerTask::TYPE type)
    {
        m_Type = type;
    }

    CTimerTask::TYPE CTimerTask::GetType()
    {
        return m_Type;
    }

    void CTimerTask::SetTime(IZ_TIME ms)
    {
        m_Time = ms;
    }

    IZ_TIME CTimerTask::GetTime()
    {
        return m_Time;
    }

    void CTimerTask::SetPrev(IZ_TIME ms)
    {
        m_Prev = ms;
    }

    IZ_TIME CTimerTask::GetPrev()
    {
        return m_Prev;
    }

    void CTimerTask::SetInterval(IZ_FLOAT ms)
    {
        m_Interval = ms;
    }

    IZ_FLOAT CTimerTask::GetInterval()
    {
        return m_Interval;
    }

    void CTimerTask::SetElapsed(IZ_FLOAT ms)
    {
        m_Elapsed = ms;
    }

    IZ_FLOAT CTimerTask::GetElapsed()
    {
        return m_Elapsed;
    }

    void CTimerTask::SetTimeForRun(IZ_FLOAT time)
    {
        m_TempTime = time;
    }

    void CTimerTask::OnRun()
    {
        OnRun(m_TempTime);
    }

    ///////////////////////////////////////////////////

    CTimerTaskExecuter::CTimerTaskExecuter()
    {
    }

    CTimerTaskExecuter::~CTimerTaskExecuter()
    {
        Clear();
    }

    IZ_BOOL CTimerTaskExecuter::PostTask(
        CTimerTask* task, 
        CTimerTask::TYPE type,
        IZ_TIME current,
        IZ_FLOAT time, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        task->SetType(type);

        if (type == CTimerTask::TYPE_INTERVAL) {
            task->SetInterval(time);
        }

        task->SetIsDeleteSelf(willDelete);

        task->SetTime(sys::CTimer::Add(current, time));
        task->SetPrev(current);

        return m_TaskList.AddTail(reinterpret_cast<CStdList<CTimerTask>::Item*>(task->GetListItem()));
    }

    IZ_BOOL CTimerTaskExecuter::PostTask(
        CTimerTask* task, 
        CTimerTask::TYPE type,
        IZ_FLOAT time, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        IZ_TIME cur = sys::CTimer::GetCurTime();

        return PostTask(
            task,
            type,
            cur,
            time,
            willDelete);
    }

    void CTimerTaskExecuter::Update()
    {
        ListItem* item = m_TaskList.GetTop();
        while (item != IZ_NULL) {
            CTimerTask* task = item->GetData();
            ListItem* next = item->GetNext();

            CTimerTask::TYPE type = task->GetType();
            IZ_TIME time = task->GetTime();

            IZ_TIME cur = sys::CTimer::GetCurTime();
            IZ_TIME base = cur;

            IZ_FLOAT taskElapsed = task->GetElapsed();
            if (taskElapsed > 0) {
                base = sys::CTimer::Sub(cur, taskElapsed);
            }

            if (task->WillCancel()
                || sys::CTimer::Compare(base, time))
            {
                IZ_FLOAT elapsed = sys::CTimer::ComputeTime(
                    task->GetPrev(),
                    cur);

                if (type == CTimerTask::TYPE_DELAY) {
                    item->Leave();
                    task->SetTimeForRun(elapsed);
                    task->Run(IZ_NULL);

                    if (task->IsDeleteSelf()) {
                        CTask::DeleteTask(task);
                        task = IZ_NULL;
                    }
                }
                else {
                    task->SetTimeForRun(elapsed);
                    task->Run(IZ_NULL);

                    if (task->IsCanceled()) {
                        item->Leave();

                        if (task->IsDeleteSelf()) {
                            CTask::DeleteTask(task);
                            task = IZ_NULL;
                        }
                    }
                    else {
                        task->ResetState();

                        IZ_FLOAT interval = task->GetInterval();

                        task->SetTime(sys::CTimer::Add(cur, interval));
                        task->SetPrev(cur);

                        task->SetElapsed(elapsed - interval);
                    }
                }
            }

            item = next;
        }
    }

    void CTimerTaskExecuter::Clear()
    {
        // å„énññ
        ListItem* item = m_TaskList.GetTop();
        while (item != IZ_NULL) {
            CTimerTask* task = item->GetData();
            item = item->GetNext();

            task->Cancel();
            task->Run(IZ_NULL);

            if (task->IsDeleteSelf()) {
                CTask::DeleteTask(task);
            }
        }

        m_TaskList.Clear();
    }

    IZ_UINT CTimerTaskExecuter::GetRegisteredTaskNum()
    {
        return m_TaskList.GetItemNum();
    }

    void CTimerTaskExecuter::Cancel(
        IZ_BOOL (*funcIsCancel)(CTimerTask* task, void* userData),
        void* userData)
    {
        IZ_ASSERT(funcIsCancel);

        ListItem* item = m_TaskList.GetTop();
        while (item != IZ_NULL) {
            ListItem* next = item->GetNext();
            CTimerTask* task = item->GetData();

            if ((*funcIsCancel)(task, userData)) {
                item->Leave();

                task->Cancel();
                task->Run(IZ_NULL);

                if (task->IsDeleteSelf()) {
                    CTask::DeleteTask(task);
                }
            }

            item = next;
        }
    }

    ///////////////////////////////////////////////////

    CTimerThread::CTimerThread()
    {
        m_WillTerminate = IZ_FALSE;
        m_Mutex.Open();
        m_Event.Open();

        m_State = CTimerThread::STATE_NONE;
    }

    CTimerThread::~CTimerThread()
    {
    }

    void CTimerThread::Run()
    {
        m_Timer.Begin();

        while (IZ_TRUE) {
            m_Event.Wait();

            {
                sys::CGuarder guard(m_Mutex);

                if (m_WillTerminate) {
                    break;
                }

                m_TaskExecuter.Update();

                if (m_TaskExecuter.GetRegisteredTaskNum() == 0) {
                    // There is no task, so thread wait.
                    m_Event.Reset();
                }
            }
        }

        m_Mutex.Close();
        m_Event.Close();
    }

    void CTimerThread::Join()
    {
        if (m_State == STATE_TERMINATED) {
            return;
        }

        m_WillTerminate = IZ_TRUE;

        // Thread may be waited by event.
        m_Event.Set();

        sys::CThread::Join();

        // å„énññ
        m_TaskExecuter.Clear();

        m_State = STATE_TERMINATED;
    }

    IZ_BOOL CTimerThread::PostDelayedTask(
        CTimerTask* task, 
        IZ_FLOAT delay, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        return PostTaskInternal(
            task,
            CTimerTask::TYPE_DELAY,
            delay, 
            willDelete);
    }

    IZ_BOOL CTimerThread::PostIntervalTask(
        CTimerTask* task, 
        IZ_FLOAT interval, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        return PostTaskInternal(
            task, 
            CTimerTask::TYPE_INTERVAL,
            interval,
            willDelete);
    }

    IZ_BOOL CTimerThread::PostTaskInternal(
        CTimerTask* task, 
        CTimerTask::TYPE type,
        IZ_FLOAT time, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        if (m_WillTerminate) {
            return IZ_FALSE;
        }

        if (m_State == STATE_TERMINATED) {
            return IZ_FALSE;
        }

        if (m_State == STATE_NONE) {
            VRETURN(Start());
            m_State = STATE_INITIALIZED;
        }

        m_Mutex.Lock();
        {
            m_TaskExecuter.PostTask(task, type, time, willDelete);
        }
        m_Mutex.Unlock();

        m_Event.Set();

        return IZ_TRUE;
    }
}   // namespace threadmodel
}   // namespace izanagi
