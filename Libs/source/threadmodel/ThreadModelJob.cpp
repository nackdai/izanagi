#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"
#include "threadmodel/ThreadModelJobQueue.h"

namespace izanagi
{
namespace threadmodel
{

CJob::CJob()
{
    m_QueueItem.Init(this);
    m_JobQueue = IZ_NULL;
}

// キャンセル.
IZ_BOOL CJob::Cancel()
{
    IZ_ASSERT(m_JobQueue != IZ_NULL);

    return m_JobQueue->Cancel(this);
}

// ジョブキューに登録されるときに呼ばれる
void CJob::Registered(CJobQueue* jobQueue)
{
    // NOTE
    // ロックされている状態で呼び出されることが保証される

    if (m_JobQueue != jobQueue)
    {
        if (m_JobQueue != IZ_NULL)
        {
            m_JobQueue->Cancel(this);
        }

        m_JobQueue = jobQueue;
    }
}

// 処理が終了してジョブキューから外れるときに呼ばれる
void CJob::Detached()
{
    // NOTE
    // ロックされている状態で呼び出されることが保証される

    m_JobQueue = IZ_NULL;
}

}   // namespace threadmodel
}   // namespace izanagi
