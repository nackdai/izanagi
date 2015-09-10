#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"
#include "threadmodel/ThreadModelJobQueue.h"

namespace izanagi
{
namespace threadmodel
{
    void CJob::DeleteJob(CJob*& job)
    {
        delete job;
        FREE(job->m_Allocator, job);
    }

    CJob::CJob()
    {
        m_Allocator = IZ_NULL;

        m_ListItem.Init(this);
        m_JobQueue = IZ_NULL;

        m_State = State_None;

        m_RunResult = IZ_FALSE;
        m_IsCanceled = IZ_FALSE;

        m_EnableDeleteWhenFinish = IZ_FALSE;
    }

    CJob::~CJob()
    {
        FREE(m_Allocator, this);
    }

    void CJob::Run()
    {
        m_RunResult = IZ_FALSE;

        {
            std::lock_guard<std::mutex> lock(m_Mutex);
            {
                if (m_IsCanceled) {
                    return;
                }
                m_State = State_Running;
            }
        }

        m_RunResult = OnRun();

        {
			std::lock_guard<std::mutex> lock(m_Mutex);
            {
                m_State = State_WillFinish;
            }
        }
    }

    void CJob::Finish()
    {
        OnFinish(m_RunResult);
        m_State = State_Finished;
    }

    // キャンセル.
    void CJob::Cancel()
    {
		std::lock_guard<std::mutex> lock(m_Mutex);
        {
            OnCancel();
            m_IsCanceled = IZ_TRUE;
        }
    }

    // 状態を取得
    CJob::State CJob::GetState()
    {
		std::lock_guard<std::mutex> lock(m_Mutex);
        {
            return m_State;
        }
    }

    IZ_BOOL CJob::IsCanceled()
    {
		std::lock_guard<std::mutex> lock(m_Mutex);
        {
            return m_IsCanceled;
        }
    }

    void CJob::SetState(CJob::State state)
    {
		std::lock_guard<std::mutex> lock(m_Mutex);
        {
            m_State = state;
        }
    }

    // 指定されたジョブキューに登録済みかどうか
    IZ_BOOL CJob::IsRegistered(CJobQueue* jobQueue)
    {
        return (m_JobQueue == jobQueue);
    }

    void CJob::SetEnableDeleteWhenFinish()
    {
        m_EnableDeleteWhenFinish = IZ_TRUE;
    }

    IZ_BOOL CJob::EnableDeleteWhenFinish()
    {
        return m_EnableDeleteWhenFinish;
    }

    void CJob::OnFinish(IZ_BOOL runResult)
    {
        // Nothing...
    }

    void CJob::OnCancel()
    {
        // Nothing...
    }
}   // namespace threadmodel
}   // namespace izanagi
