#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"
#include "threadmodel/ThreadModelJobQueue.h"

namespace izanagi
{
namespace threadmodel
{

    CJobWorker::CJobWorker(CJobQueue* jobQueue)
    {
        m_JobQueue = jobQueue;
        m_Event.Open();
        m_WillJoin = IZ_FALSE;
    }

    void CJobWorker::Run()
    {
        IZ_ASSERT(m_JobQueue != IZ_NULL);

        while (m_Event.Wait())
        {
            CJob* job = m_JobQueue->Dequeue();

            if (job != IZ_NULL)
            {
                if (job->WillCancel())
                {
                    job->NotifyCancel();
                    job->Detach();
                }
                else
                {
                    job->OnExecute();

                    // 終了待ちキューに積む
                    m_JobQueue->EnqueueAsFinishJob(job);
                }
            }
            else
            {
                m_JobQueue->NotifyWorkerThreadSuspend();

                if (m_WillJoin)
                {
                    break;
                }
                else
                {
                    m_Event.Reset();
                }
            }

            CThread::YieldThread();
        }
    }

    // このスレッドの実行を開始.
    IZ_BOOL CJobWorker::Start()
    {
        IZ_BOOL ret = CThread::Start();
        if (ret)
        {
            Resume();
        }
        return ret;
    }

    void CJobWorker::Resume()
    {
        m_Event.Set();
    }

    void CJobWorker::Suspend()
    {
        m_Event.Reset();
    }
}   // namespace threadmodel
}   // namespace izanagi
