#include "threadmodel/TimerThread.h"

namespace izanagi
{
namespace threadmodel
{
    CTimerTask::CTimerTask()
    {
        m_Type = TYPE_DELAY;
        m_Ms = 0;
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

    void CTimerTask::SetTime(IZ_FLOAT ms)
    {
        m_Ms = ms;
    }

    IZ_FLOAT CTimerTask::GetTime()
    {
        return m_Ms;
    }

    void CTimerTask::SetElapsed(IZ_FLOAT ms)
    {
        m_Elapsed = ms;
    }

    IZ_FLOAT CTimerTask::GetElapsed()
    {
        return m_Elapsed;
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
        IZ_FLOAT elapsed = 0;

        while (IZ_TRUE) {
            if (m_WillTerminate) {
                break;
            }

            m_Event.Wait();

            m_Timer.Begin();

            m_Mutex.Lock();
            {
                ListItem* item = m_TaskList.GetTop();
                while (item != IZ_NULL) {
                    CTimerTask* task = item->GetData();
                    ListItem* next = item->GetNext();

                    CTimerTask::TYPE type = task->GetType();
                    IZ_FLOAT time = task->GetTime();

                    if (type == CTimerTask::TYPE_DELAY) {
                        // Delay
                        time -= elapsed;

                        if (task->WillCancel()
                            || time <= 0)
                        {
                            item->Leave();
                            task->Run(IZ_NULL);

                            if (task->IsDeleteSelf()) {
                                CTask::DeleteTask(task);
                            }
                        }
                        else {
                            task->SetTime(time);
                        }
                    }
                    else {
                        // Interval
                        IZ_FLOAT taskElapsed = task->GetElapsed();
                        taskElapsed += elapsed;
                        
                        if (task->WillCancel()
                            || time <= taskElapsed)
                        {
                            taskElapsed -= time;
                            task->Run(IZ_NULL);
                        }

                        task->SetElapsed(taskElapsed);
                    }

                    item = next;
                }

                if (m_TaskList.GetItemNum() == 0) {
                    // There is no task, so thread wait.
                    m_Event.Reset();
                    elapsed = 0;
                }
            }
            m_Mutex.Unlock();

            elapsed = m_Timer.End();
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
        task->SetTime(time);
        return s_Thread.Enqueue(reinterpret_cast<CStdList<CTimerTask>::Item*>(task->GetListItem()));
    }

    void CTimerThread::Terminate()
    {
        s_State = STATE_TERMINATED;
        s_Thread.Termnate();
    }
}   // namespace threadmodel
}   // namespace izanagi
