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

                if (m_TaskList.GetItemNum() == 0) {
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
        m_WillTerminate = IZ_TRUE;

        // Thread may be waited by event.
        m_Event.Set();

        sys::CThread::Join();

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

        m_State = STATE_TERMINATED;
    }

    IZ_BOOL CTimerThread::PostDelayedTask(
        CTimerTask* task, 
        IZ_FLOAT delay, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        task->SetType(CTimerTask::TYPE_DELAY);
        return PostTaskInternal(task, delay, willDelete);
    }

    IZ_BOOL CTimerThread::PostIntervalTask(
        CTimerTask* task, 
        IZ_FLOAT interval, 
        IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        task->SetInterval(interval);
        task->SetType(CTimerTask::TYPE_INTERVAL);
        return PostTaskInternal(task, interval, willDelete);
    }

    IZ_BOOL CTimerThread::PostTaskInternal(
        CTimerTask* task, 
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

        task->SetIsDeleteSelf(willDelete);

        IZ_TIME cur = sys::CTimer::GetCurTime();
        task->SetTime(sys::CTimer::Add(cur, time));
        task->SetPrev(cur);

        m_Mutex.Lock();
        {
            m_TaskList.AddTail(reinterpret_cast<CStdList<CTimerTask>::Item*>(task->GetListItem()));
        }
        m_Mutex.Unlock();

        m_Event.Set();

        return IZ_TRUE;
    }
}   // namespace threadmodel
}   // namespace izanagi
