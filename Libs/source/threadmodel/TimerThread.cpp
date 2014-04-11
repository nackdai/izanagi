#include "threadmodel/TimerThread.h"

namespace izanagi
{
namespace threadmodel
{
    CTimerTask::CTimerTask()
    {
        m_DelayMs = 0;
    }

    void CTimerTask::SetDelay(IZ_FLOAT ms)
    {
        m_DelayMs = ms;
    }

    IZ_FLOAT CTimerTask::GetDelay()
    {
        return m_DelayMs;
    }

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
        IZ_FLOAT all = 0;

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
                    item = item->GetNext();

                    IZ_FLOAT time = task->GetDelay();
                    time -= elapsed;

                    all += elapsed;

                    if (time <= 0) {
                        //IZ_PRINTF("elpsed: %f\n", all);

                        task->Run(IZ_NULL);

                        if (task->IsDeleteSelf()) {
                            CTask::DeleteTask(task);
                        }
                    }
                    else {
                        task->SetDelay(time);
                    }
                }

                if (m_TaskList.GetItemNum() == 0) {
                    // There is no task, so thread wait.
                    m_Event.Reset();
                    elapsed = 0;
                    all = 0;
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

    IZ_BOOL CTimerThread::PostDelayedTask(CTimerTask* task, IZ_FLOAT delay, IZ_BOOL willDelete/*= IZ_FALSE*/)
    {
        if (s_State == STATE_TERMINATED) {
            return IZ_FALSE;
        }

        if (s_State == STATE_NONE) {
            VRETURN(s_Thread.Start());
            s_State = STATE_INITIALIZED;
        }

        task->SetIsDeleteSelf(willDelete);
        task->SetDelay(delay);
        return s_Thread.Enqueue(reinterpret_cast<CStdList<CTimerTask>::Item*>(task->GetListItem()));
    }

    void CTimerThread::Terminate()
    {
        s_State = STATE_TERMINATED;
        s_Thread.Termnate();
    }
}   // namespace threadmodel
}   // namespace izanagi
