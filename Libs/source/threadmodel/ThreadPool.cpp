#include "threadmodel/ThreadPool.h"
#include "threadmodel/ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
    IMemoryAllocator* CThreadPool::s_Allocator = IZ_NULL;

    sys::CMutex CThreadPool::s_TaskListLocker;
    sys::CEvent CThreadPool::s_TaskWaiter;
    CStdList<CTask> CThreadPool::s_TaskList;

    sys::CEvent CThreadPool::s_TaskEmptyWaiter;

    IZ_UINT CThreadPool::s_MaxThreadNum = 0;
    CStdList<CThreadPool::CThread> CThreadPool::s_ThreadList;

    CThreadPool::State CThreadPool::s_State;

    CThreadPool::CThread::CThread()
    {
        m_Mutex.Open();

        m_State = State_Waiting;

        m_ListItem.Init(this);
    }

    CThreadPool::CThread::~CThread()
    {
        m_Mutex.Close();
    }

    void CThreadPool::CThread::Run()
    {
        while (IZ_TRUE) {
            CTask* task = CThreadPool::DequeueTask();

            sys::CGuarder guarder(m_Mutex);

            if (m_State == State_WillFinish) {
                m_State = State_Finished;
                break;
            }

            if (task != IZ_NULL) {
                m_State = State_Running;
                task->Run(IZ_NULL);

                if (task->IsDeleteSelf()) {
                    CTask::DeleteTask(task);
                }
            }

            m_State = State_Waiting;
        }
    }

    void CThreadPool::CThread::Terminate()
    {
        sys::CGuarder guard(m_Mutex);

        if (m_State == State_Finished) {
            return;
        }

        m_State = State_WillFinish;
    }

    IZ_BOOL CThreadPool::CThread::IsWaiting()
    {
        sys::CGuarder guard(m_Mutex);

        return (m_State == State_Waiting);
    }

    void CThreadPool::Init(IMemoryAllocator* allocator)
    {
        Init(
            allocator,
            sys::CEnvironment::GetProcessorNum());
    }

    void CThreadPool::Init(
        IMemoryAllocator* allocator,
        IZ_UINT threadNum)
    {
        if (s_Allocator != IZ_NULL) {
            return;
        }

        s_State = State_Running;

        s_TaskListLocker.Open();
        s_TaskWaiter.Open();
        s_TaskEmptyWaiter.Open();

        s_Allocator = allocator;

        s_MaxThreadNum = threadNum;

        for (IZ_UINT i = 0; i < threadNum; i++) {
            CThread* thread = CreateThread();

            s_ThreadList.AddTail(thread->GetListItem());
        }
    }

    void CThreadPool::EneueueTask(CTask* task)
    {
        {
            sys::CGuarder guarder(s_TaskListLocker);

            if (s_State == State_WillTerminate
                || s_State == State_Terminated)
            {
                return;
            }

            s_TaskList.AddTail(task->GetListItem());
        }

        // Notify a task is queued.
        s_TaskWaiter.Set();
    }

    void CThreadPool::WaitEmpty()
    {
        sys::CGuarder guarder(s_TaskListLocker);

        if (s_TaskList.HasItem()) {
            s_TaskEmptyWaiter.Wait();

            // If WaitEmpty is called again, s_TaskEmptyWaiter will wait.
            s_TaskEmptyWaiter.Reset();
        }
    }

    CThreadPool::CThread* CThreadPool::CreateThread()
    {
        void* buf = ALLOC(s_Allocator, sizeof(CThread));
        VRETURN_NULL(buf);

        CThread* ret = new(buf) CThread();
        ret->Start();

        return ret;
    }

    void CThreadPool::Terminate()
    {
        s_TaskListLocker.Lock();
        {
            s_State = State_WillTerminate;
        }
        s_TaskListLocker.Unlock();

        // Wait queued tasks are empty.
        WaitEmpty();

        CStdList<CThread>::Item* item = s_ThreadList.GetTop();

        // Notify thread will terminate.
        while (item != IZ_NULL) {
            CThread* thread = item->GetData();
            thread->Terminate();
            item = item->GetNext();
        }

        item = s_ThreadList.GetTop();

        // Joint threads.
        while (item != IZ_NULL) {
            CThread* thread = item->GetData();

            CStdList<CThread>::Item* next = item->GetNext();

            s_TaskWaiter.Set();

            thread->Join();
            delete thread;

            FREE(s_Allocator, thread);

            item = next;
        }

        IZ_ASSERT(s_ThreadList.GetItemNum() == 0);

        s_TaskListLocker.Close();
        s_TaskWaiter.Close();
        s_TaskEmptyWaiter.Close();
    }

    IZ_UINT CThreadPool::GetMaxThreadNum()
    {
        return s_MaxThreadNum;
    }

    CTask* CThreadPool::DequeueTask()
    {
        s_TaskWaiter.Wait();

        sys::CGuarder guarder(s_TaskListLocker);

        CStdList<CTask>::Item* item = s_TaskList.GetTop();

        if (item == IZ_NULL) {
            s_TaskWaiter.Reset();
            s_TaskEmptyWaiter.Set();

            return IZ_NULL;
        }

        item->Leave();

        CTask* ret = item->GetData();

        return ret;
    }
}   // namespace threadmodel
}   // namespace izanagi
