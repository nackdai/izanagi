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
        m_WillJoin = IZ_FALSE;
    }

    void CJobWorker::Run()
    {
        IZ_ASSERT(m_JobQueue != IZ_NULL);

        while (m_JobQueue->WaitEvent())
        {
            CJob* job = m_JobQueue->Dequeue();

            if (job != IZ_NULL)
            {
                job->OnExecute();

                // 終了待ちキューに積む
                m_JobQueue->EnqueueAsFinishJob(job);
            }
            else
            {
                if (m_WillJoin)
                {
                    break;
                }
                else
                {
                    // キューが空になったことを通知する
                    m_JobQueue->NotifyEmptyJobToMainThred();

                    // キューが空なので何もすることがなさそうだから寝る
                    m_JobQueue->ResetEvent();
                }
            }

            CThread::YieldThread();
        }
    }
}   // namespace threadmodel
}   // namespace izanagi
