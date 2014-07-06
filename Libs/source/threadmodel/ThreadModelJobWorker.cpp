#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"
#include "threadmodel/ThreadModelJobQueue.h"

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
    }

    CJobWorker::CJobWorker(const sys::ThreadName& name)
        : sys::CThread(name)
    {
        m_Sema.Init(0);
        m_Mutex.Open();

        m_Job = IZ_NULL;

        m_State = State_Waiting;
        m_StateMutex.Open();
    }

    CJobWorker::~CJobWorker()
    {
        // ”O‚Ì‚½‚ß
        Join();

        m_Sema.Close();
        m_Mutex.Close();
        m_StateMutex.Close();
    }

    void CJobWorker::Register(CJob* job)
    {
        {
            sys::CGuarder guard(m_Mutex);

            IZ_ASSERT(m_Job == IZ_NULL);

            if (m_State == State_Waiting)
            {
                m_Job = job;
                SetState(State_Registered);
            }
        }

        m_Sema.Release();
    }

    void CJobWorker::Run()
    {
        while (IZ_TRUE)
        {
            if (m_Sema.Wait()) {
                sys::CGuarder guard(m_Mutex);
                {
                    if (m_State == State_WillJoin) {
                        m_State = State_Joined;
                        break;
                    }

                    IZ_ASSERT(m_Job != IZ_NULL);

                    SetState(State_Running);

                    m_Job->Run();
                    m_Job = IZ_NULL;

                    SetState(State_Waiting);
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

            SetState(State_WillJoin);
        }

        // Ž~‚Ü‚Á‚Ä‚¢‚é‚©‚à‚µ‚ê‚È‚¢‚Ì‚Å‹N‚±‚·
        m_Sema.Release();

        sys::CThread::Join();
    }

    CJobWorker::State CJobWorker::GetState()
    {
        sys::CGuarder guard(m_StateMutex);
        return m_State;
    }

    IZ_BOOL CJobWorker::IsWaiting()
    {
        sys::CGuarder guard(m_StateMutex);
        return m_State == State_Waiting;
    }

    void CJobWorker::SetState(State state)
    {
        sys::CGuarder guard(m_StateMutex);
        m_State = state;
    }
}   // namespace threadmodel
}   // namespace izanagi
