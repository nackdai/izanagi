#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"
#include "threadmodel/ThreadModelJobQueue.h"

namespace izanagi
{
namespace threadmodel
{
    CJobWorker::CJobWorker()
    {
        m_Job = IZ_NULL;

        m_State = State_Waiting;
    }

    CJobWorker::CJobWorker(const sys::ThreadName& name)
        : sys::CThread(name)
    {
        m_Job = IZ_NULL;

        m_State = State_Waiting;
    }

    CJobWorker::~CJobWorker()
    {
        // ”O‚Ì‚½‚ß
        Join();
    }

    void CJobWorker::Register(CJob* job)
    {
        {
            std::lock_guard<std::mutex> lock(m_Mutex);

            IZ_ASSERT(m_Job == IZ_NULL);

            if (m_State == State_Waiting)
            {
                m_Job = job;
                SetState(State_Registered);
            }
        }

        m_Sema.notify();
    }

    void CJobWorker::Run()
    {
        while (IZ_TRUE)
        {
			m_Sema.wait();
			{
				std::lock_guard<std::mutex> lock(m_Mutex);
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
			std::lock_guard<std::mutex> lock(m_Mutex);

            if (m_State == State_Joined) {
                return;
            }

            SetState(State_WillJoin);
        }

        // Ž~‚Ü‚Á‚Ä‚¢‚é‚©‚à‚µ‚ê‚È‚¢‚Ì‚Å‹N‚±‚·
        m_Sema.notify();

        sys::CThread::Join();
    }

    CJobWorker::State CJobWorker::GetState()
    {
		std::lock_guard<std::mutex> lock(m_StateMutex);
        return m_State;
    }

    IZ_BOOL CJobWorker::IsWaiting()
    {
		std::lock_guard<std::mutex> lock(m_StateMutex);
        return m_State == State_Waiting;
    }

    void CJobWorker::SetState(State state)
    {
		std::lock_guard<std::mutex> lock(m_StateMutex);
        m_State = state;
    }
}   // namespace threadmodel
}   // namespace izanagi
