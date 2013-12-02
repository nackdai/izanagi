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
        m_Event.Open();
        m_Mutex.Open();

        m_State = State_Waiting;

        m_ListItem.Init(this);
    }

    void CThreadPool::CThread::SetRunnable(sys::IRunnable* runnable)
    {
        m_Mutex.Lock();
        {
            m_State = State_Registered;
            m_Runnable = runnable;
        }
        m_Mutex.Unlock();

        m_Event.Reset();
        m_Sema.Release();
    }

    sys::IRunnable* CThreadPool::CThread::GetRunnable()
    {
        return m_Runnable;
    }

    void CThreadPool::CThread::Run()
    {
        while (m_Sema.Wait()) {
            {
                sys::CGuarder guard(m_Mutex);

                if (m_State == State_WillFinish) {
                    m_State = State_Finished;
                    m_Event.Set();
                    break;
                }

                m_State = State_Running;

                if (m_Runnable != IZ_NULL) {
                    m_Runnable->Run(GetUserData());
                }

                m_State = State_Waiting;
            }

            m_Event.Set();
        }
    }

    void CThreadPool::CThread::Join()
    {
        // NOTE
        // Ç±Ç±Ç≈ÇÕñ{ìñÇ…JoinÇÕÇπÇ∏Ç…ÅARunÇ™ÇPé¸Ç∑ÇÈÇÃÇë“Ç¬
        m_Event.Wait();
    }

    void CThreadPool::CThread::Terminate()
    {
        {
            sys::CGuarder guard(m_Mutex);

            if (m_State == State_Finished) {
                return;
            }

            m_State = State_WillFinish;
        }

        // é~Ç‹Ç¡ÇƒÇ¢ÇÈÇ©Ç‡ÇµÇÍÇ»Ç¢ÇÃÇ≈ãNÇ±Ç∑
        m_Sema.Release();

        sys::CThread::Join();

        m_Sema.Close();
        m_Event.Close();
        m_Mutex.Close();
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
