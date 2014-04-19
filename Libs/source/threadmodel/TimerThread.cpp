#include "threadmodel/TimerThread.h"

namespace izanagi
{
namespace threadmodel
{
    CTimerTask::CTimerTask()
    {
        m_Type = TYPE_DELAY;
        m_Time = 0;
        m_Prev = 0;
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

    CTimerThread::CThread CTimerThread::s_Thread;
    CTimerThread::STATE CTimerThread::s_State = CTimerThread::STATE_NONE;

    CTimerThread::CThread::CThread()
    {
        m_WillTerminate = IZ_FALSE;
        m_Mutex.Open();
        m_Event.Open();
    }

    CTimerThread::CThread::~CThread()
    {
    }

    void CTimerThread::CThread::Run()
    {
        m_Timer.Begin();

        while (IZ_TRUE) {
            if (m_WillTerminate) {
                break;
            }

            m_Event.Wait();

            m_Mutex.Lock();
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
                            
                            IZ_FLOAT interval = task->GetInterval();

                            task->SetTime(sys::CTimer::Add(cur, interval));
                            task->SetPrev(cur);

                            task->SetElapsed(elapsed - interval);
                        }
                    }

                    item = next;
                }

                if (m_TaskList.GetItemNum() == 0) {
                    // There is no task, so thread wait.
                    m_Event.Reset();
                }
            }
            m_Mutex.Unlock();
        }

        m_Mutex.Close();
        m_Event.Close();
    }

    void CTimerThread::CThread::Termnate()
    {
        m_WillTerminate = IZ_TRUE;

        // Thread may be waited by event.
        m_Event.Set();

        Join();

        // ŒãŽn––
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

    IZ_BOOL CTimerThread::CThread::Enqueue(CStdList<CTimerTask>::Item* item)
    {
        if (m_WillTerminate) {
            return IZ_FALSE;
        }

        m_Mutex.Lock();
        {
            m_TaskList.AddTail(item);
        }
        m_Mutex.Unlock();

        m_Event.Set();

        return IZ_TRUE;
    }

    IZ_BOOL CTimerThread::PostDelayedTask(
        CTimerTask* task, 
        IZ_FLOAT delay, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        return PostTask(task, CTimerTask::TYPE_DELAY, delay, willDelete);
    }

    IZ_BOOL CTimerThread::PostIntervalTask(
        CTimerTask* task, 
        IZ_FLOAT interval, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        task->SetInterval(interval);
        return PostTask(task, CTimerTask::TYPE_INTERVAL, interval, willDelete);
    }

    IZ_BOOL CTimerThread::PostTask(
        CTimerTask* task, 
        CTimerTask::TYPE type,
        IZ_FLOAT time, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        if (s_State == STATE_TERMINATED) {
            return IZ_FALSE;
        }

        if (s_State == STATE_NONE) {
            VRETURN(s_Thread.Start());
            s_State = STATE_INITIALIZED;
        }

        task->SetIsDeleteSelf(willDelete);
        task->SetType(type);

        IZ_TIME cur = sys::CTimer::GetCurTime();
        task->SetTime(sys::CTimer::Add(cur, time));
        task->SetPrev(cur);

        return s_Thread.Enqueue(reinterpret_cast<CStdList<CTimerTask>::Item*>(task->GetListItem()));
    }

    void CTimerThread::Terminate()
    {
        s_State = STATE_TERMINATED;
        s_Thread.Termnate();
    }
}   // namespace threadmodel
}   // namespace izanagi
