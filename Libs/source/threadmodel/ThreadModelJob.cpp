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

        m_IsFinished = IZ_FALSE;
        m_WillCancel = IZ_FALSE;
        m_IsCanceled = IZ_FALSE;
    }

    // キャンセル.
    void CJob::Cancel()
    {
        if (m_JobQueue->Cancel(this))
        {
            NotifyCancel();
            Detach();
        }
        else
        {
            m_WillCancel = IZ_TRUE;
        }
    }

    // 指定されたジョブキューに登録済みかどうか
    IZ_BOOL CJob::IsRegistered(CJobQueue* jobQueue)
    {
        return (m_JobQueue == jobQueue);
    }

    // エンキューのための準備
    void CJob::Prepare(CJobQueue* jobQueue)
    {
        // NOTE
        // ロックされている状態で呼び出されることが保証される

        // フラグをリセット
        m_IsFinished = IZ_FALSE;
        m_WillCancel = IZ_FALSE;
        m_IsCanceled = IZ_FALSE;

        m_JobQueue = jobQueue;
    }

    // 処理が終了してジョブキューから外れるときに呼ばれる
    void CJob::Detach()
    {
        // NOTE
        // ロックされている状態で呼び出されることが保証される

        if (m_QueueItem.HasList())
        {
            m_QueueItem.Leave();
        }

        m_JobQueue = IZ_NULL;
        m_IsFinished = IZ_TRUE;
    }

    void CJob::NotifyCancel()
    {
        m_WillCancel = IZ_FALSE;
        m_IsCanceled = IZ_TRUE;
    }

}   // namespace threadmodel
}   // namespace izanagi
