#include "threadmodel/ThreadModelTaskScheduler.h"
#include "threadmodel/ThreadModelTask.h"
#include "threadmodel/ThreadPool.h"

namespace izanagi
{
namespace threadmodel
{
    void CTaskScheduler::CTaskExecuter::Run(void* userData)
    {
        m_Scheduler->ExecuteTask();
    }

    void CTaskScheduler::CTaskExecuter::SetScheduler(CTaskScheduler* scheduler)
    {
        m_Scheduler = scheduler;
    }

    CTaskScheduler::CTaskScheduler()
    {
        m_Mutex.Open();
        m_Executer.SetScheduler(this);

        m_Sema.Init(0);
    }

    CTaskScheduler::~CTaskScheduler()
    {
        m_Mutex.Close();
    }

    IZ_BOOL CTaskScheduler::Enqueue(CTask& task, IZ_BOOL isWaitThreadEmpty/*= IZ_TRUE*/)
    {
        sys::CThread* thread = CThreadPool::GetThread(&m_Executer);

        if (thread == IZ_NULL) {
            if (isWaitThreadEmpty) {
                thread = CThreadPool::GetThreadUntilThreadIsEmpty(&m_Executer);
            }
            else {
                task.Run(IZ_NULL);
                return IZ_FALSE;
            }
        }

        if (thread != IZ_NULL) {
            m_Mutex.Lock();
            {
                m_Tasks.AddTail(task.GetListItem());
            }
            m_Mutex.Unlock();

            m_Sema.Release();
        }

        return IZ_TRUE;
    }

    void CTaskScheduler::ExecuteTask()
    {
        m_Sema.Wait();

        CStdList<CTask>::Item* item = IZ_NULL;

        m_Mutex.Lock();
        {
            item = m_Tasks.GetTop();

            if (item != IZ_NULL) {
                item->Leave();
            }
        }
        m_Mutex.Unlock();

        if (item != IZ_NULL) {
            CTask* task = item->GetData();
            task->Run(IZ_NULL);
        }
    }
}   // namespace threadmodel
}   // namespace izanagi
