#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"
#include "threadmodel/ThreadModelJobQueue.h"
#include "threadmodel/ThreadPool.h"

namespace izanagi
{
namespace threadmodel
{
    CJobWorker::CJobWorker()
    {
        m_Sema.Init(0);
        m_Mutex.Open();

        m_Job = IZ_NULL;

        m_State = State_Waiting;

        m_Runner.worker = this;
    }

    CJobWorker::~CJobWorker()
    {
        // ”O‚Ì‚½‚ß
        Join();

        m_Sema.Close();
        m_Mutex.Close();
    }

    void CJobWorker::Register(CJob* job)
    {
        {
            sys::CGuarder guard(m_Mutex);

            IZ_ASSERT(m_Job == IZ_NULL);

            if (m_State == State_Waiting)
            {
                m_Job = job;
                m_State = State_Registered;
            }
        }

        m_Sema.Release();
    }

    void CJobWorker::Start()
    {
        // TODO
        m_Thread = CThreadPool::GetThreadForcibliyIfNoWaitedThread(&m_Runner);
        m_Thread->Start();
    }

    void CJobWorker::CRunnable::Run(void* data)
    {
        while (IZ_TRUE)
        {
            if (worker->m_Sema.Wait()) {
                sys::CGuarder guard(worker->m_Mutex);
                {
                    if (worker->m_State == State_WillJoin) {
                        worker->m_State = State_Joined;
                        break;
                    }

                    IZ_ASSERT(worker->m_Job != IZ_NULL);

                    worker->m_State = State_Running;

                    worker->m_Job->Run();
                    worker->m_Job = IZ_NULL;

                    worker->m_State = State_Waiting;
                }
            }
        }
    }

    void CJobWorker::Join()
    {
        {
            sys::CGuarder guard(m_Mutex);

            if (m_State == State_Joined) {
                return;
            }

            m_State = State_WillJoin;
        }

        // Ž~‚Ü‚Á‚Ä‚¢‚é‚©‚à‚µ‚ê‚È‚¢‚Ì‚Å‹N‚±‚·
        m_Sema.Release();

        m_Thread->Join();
    }

    CJobWorker::State CJobWorker::GetState()
    {
        sys::CGuarder guard(m_Mutex);
        {
            return m_State;
        }
    }

    IZ_BOOL CJobWorker::IsWaiting()
    {
        sys::CGuarder guard(m_Mutex);
        {
            return m_State == State_Waiting;
        }
    }
}   // namespace threadmodel
}   // namespace izanagi
