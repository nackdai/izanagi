#include "threadmodel/ThreadPool.h"
#include "threadmodel/ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
	CThreadPool::CThread::CThread(CThreadPool* pool)
    {
		m_Pool = pool;
        m_State = State_Waiting;

        m_ListItem.Init(this);
    }

    CThreadPool::CThread::~CThread()
    {
    }

    void CThreadPool::CThread::Run()
    {
        while (IZ_TRUE) {
			CTask* task = m_Pool->DequeueTask();

            std::lock_guard<std::mutex> lock(m_Mutex);

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
        std::lock_guard<std::mutex> lock(m_Mutex);

        if (m_State == State_Finished) {
            return;
        }

        m_State = State_WillFinish;
    }

    IZ_BOOL CThreadPool::CThread::IsWaiting()
    {
        std::lock_guard<std::mutex> lock(m_Mutex);

        return (m_State == State_Waiting);
    }

	//////////////////////////////////////////////////////

	CThreadPool::CThreadPool()
	{
		m_Allocator = IZ_NULL;

		m_MaxThreadNum = 0;

		m_State = State_None;
	}

	CThreadPool::~CThreadPool()
	{
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
        if (m_Allocator != IZ_NULL) {
            return;
        }

        m_State = State_Running;

        m_Allocator = allocator;

        m_MaxThreadNum = threadNum;

        for (IZ_UINT i = 0; i < threadNum; i++) {
            CThread* thread = CreateThread();

            m_ThreadList.AddTail(thread->GetListItem());
        }
    }

	IZ_BOOL CThreadPool::EneueueTask(CTask* task)
    {
		VRETURN(task->Reset());

        {
            std::lock_guard<std::mutex> lock(m_TaskListLocker);

            if (m_State == State_WillTerminate
                || m_State == State_Terminated)
            {
                return IZ_FALSE;
            }

            m_TaskList.AddTail(task->GetListItem());
        }

        // Notify a task is queued.
        m_TaskWaiter.Set();

		return IZ_TRUE;
    }

    void CThreadPool::beginEnqueueWithoutWait()
    {
        std::lock_guard<std::mutex> lock(m_TaskListLocker);
    }

    void CThreadPool::endEnqueueWithoutWait()
    {
        // Notify a task is queued.
        m_TaskWaiter.Set();
    }

    IZ_BOOL CThreadPool::enqueueWithoutWait(CTask* task)
    {
        VRETURN(task->ResetWithoutWait());

        if (m_State == State_WillTerminate
            || m_State == State_Terminated)
        {
            return IZ_FALSE;
        }

        m_TaskList.AddTail(task->GetListItem());

        return IZ_TRUE;
    }

    void CThreadPool::WaitEmpty()
    {
        std::lock_guard<std::mutex> lock(m_TaskListLocker);

        if (m_TaskList.HasItem()) {
            m_TaskEmptyWaiter.Wait();

            // If WaitEmpty is called again, m_TaskEmptyWaiter will wait.
            m_TaskEmptyWaiter.Reset();
        }
    }

    CThreadPool::CThread* CThreadPool::CreateThread()
    {
        void* buf = ALLOC(m_Allocator, sizeof(CThread));
        VRETURN_NULL(buf);

        CThread* ret = new(buf) CThread(this);
		ret->Start();

        return ret;
    }

    void CThreadPool::Terminate()
    {
        {
			std::lock_guard<std::mutex> lock(m_TaskListLocker);
            m_State = State_WillTerminate;
        }

        // Wait queued tasks are empty.
        WaitEmpty();

        CStdList<CThread>::Item* item = m_ThreadList.GetTop();

        // Notify thread will terminate.
        while (item != IZ_NULL) {
            CThread* thread = item->GetData();
            thread->Terminate();
            item = item->GetNext();
        }

        item = m_ThreadList.GetTop();

        // Joint threads.
        while (item != IZ_NULL) {
            CThread* thread = item->GetData();

            CStdList<CThread>::Item* next = item->GetNext();

            m_TaskWaiter.Set();

            thread->Join();
            delete thread;

            FREE(m_Allocator, thread);

            item = next;
        }

        IZ_ASSERT(m_ThreadList.GetItemNum() == 0);
    }

    IZ_UINT CThreadPool::GetMaxThreadNum()
    {
        return m_MaxThreadNum;
    }

    CTask* CThreadPool::DequeueTask()
    {
        m_TaskWaiter.Wait();

        std::lock_guard<std::mutex> lock(m_TaskListLocker);

        CStdList<CTask>::Item* item = m_TaskList.GetTop();

        if (item == IZ_NULL) {
			if (m_State != State_WillTerminate) {
				// 終了するので、常にシグナル状態にしておいて、Waitで止まらないようにする
				m_TaskWaiter.Reset();
			}
            m_TaskEmptyWaiter.Set();

            return IZ_NULL;
        }

        item->Leave();

        CTask* ret = item->GetData();

        return ret;
    }
}   // namespace threadmodel
}   // namespace izanagi
