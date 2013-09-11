#include "threadmodel/ThreadPool.h"

namespace izanagi
{
namespace threadmodel
{
    IMemoryAllocator* CThreadPool::s_Allocator = IZ_NULL;
    sys::CMutex CThreadPool::s_Mutex;
    CStdList<CThreadPool::CThread> CThreadPool::s_ThreadList;

    CThreadPool::CThread::CThread()
    {
        m_Sema.Init(0);
        m_IsTerminated = IZ_FALSE;
        m_IsWaiting = IZ_TRUE;

        m_RunnableGuarder.Open();

        m_ListItem.Init(this);
    }

    void CThreadPool::CThread::SetRunnable(sys::IRunnable* runnable)
    {
        m_IsWaiting = IZ_FALSE;
        m_Runnable = runnable;
        m_Sema.Release();
    }

    sys::IRunnable* CThreadPool::CThread::GetRunnable()
    {
        return m_Runnable;
    }

    void CThreadPool::CThread::Run()
    {
        for (;;) {
            if (m_IsTerminated) {
                break;
            }

            IZ_BOOL isRunning = IZ_FALSE;

            m_RunnableGuarder.Lock();
            {
                if (m_Runnable != IZ_NULL) {
                    isRunning = IZ_TRUE;
                    m_Runnable->Run(GetUserData());
                }
            }
            m_RunnableGuarder.Unlock();

            if (!isRunning) {
                m_IsWaiting = IZ_TRUE;
                m_Sema.Wait();
            }
        }
    }

    void CThreadPool::CThread::Join()
    {
        // NOTE
        // Ç±Ç±Ç≈ÇÕñ{ìñÇ…JoinÇÕÇπÇ∏Ç…ÅARunÇë“ÇΩÇπÇÈ

        sys::CGuarder guarder(m_RunnableGuarder);

        m_Runnable = IZ_NULL;
        m_IsWaiting = IZ_TRUE;
    }

    void CThreadPool::CThread::Terminate()
    {
        m_IsTerminated = IZ_TRUE;
        m_Sema.Release();

        sys::CThread::Join();

        m_Sema.Close();
        m_RunnableGuarder.Close();
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

        s_Allocator = allocator;

        for (IZ_UINT i = 0; i < threadNum; i++) {
            CThread* thread = CreateThread();

            s_ThreadList.AddTail(thread->GetListItem());
        }

        s_Mutex.Open();
    }

    CThreadPool::CThread* CThreadPool::CreateThread()
    {
        void* buf = ALLOC(s_Allocator, sizeof(CThread));
        VRETURN_NULL(buf);

        CThread* ret = new(buf) CThread();

        return ret;
    }

    void CThreadPool::Terminate()
    {
        ListItem* item = s_ThreadList.GetTop();

        while (item != IZ_NULL) {
            CThread* thread = item->GetData();

            ListItem* next = item->GetNext();

            thread->Terminate();
            delete thread;

            FREE(s_Allocator, thread);

            item = next;
        }

        IZ_ASSERT(s_ThreadList.GetItemNum() == 0);

        s_Mutex.Close();
    }

    CThreadPool::CThread* CThreadPool::GetThread(sys::IRunnable* runnable)
    {
        sys::CGuarder gurader(s_Mutex);

        ListItem* item = s_ThreadList.GetTop();

        while (item != IZ_NULL) {
            CThread* thread = item->GetData();

            if (thread->IsWaiting()) {
                thread->SetRunnable(runnable);
                return thread;
            }

            item = item->GetNext();
        }

        return IZ_NULL;
    }

    CThreadPool::CThread* CThreadPool::GetThreadForcibliyIfNoWaitedThread(sys::IRunnable* runnable)
    {
        sys::CGuarder gurader(s_Mutex);

        ListItem* item = s_ThreadList.GetTop();

        while (item != IZ_NULL) {
            CThread* thread = item->GetData();

            if (thread->IsWaiting()) {
                thread->SetRunnable(runnable);
                return thread;
            }

            item = item->GetNext();
        }

        CThread* thread = CreateThread();
        s_ThreadList.AddTail(thread->GetListItem());

        return thread;
    }
}   // namespace threadmodel
}   // namespace izanagi
