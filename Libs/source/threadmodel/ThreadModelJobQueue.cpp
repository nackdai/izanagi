#include "threadmodel/ThreadModelJobQueue.h"
#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"

namespace izanagi
{
namespace threadmodel
{
    // バッファサイズを計算
    size_t CJobQueue::ComputeBufferSize(IZ_UINT threadNum)
    {
        IZ_ASSERT(threadNum > 0);

        size_t ret = threadNum * sizeof(CJobWorker*);
        ret += threadNum * sizeof(CJobWorker);

        return ret;
    }

    // エンキュー時の処理
    void CJobQueue::EnqueueAction(CJob* job, void* jobQueue)
    {
        job->Prepare(reinterpret_cast<CJobQueue*>(jobQueue));
    }

    CJobQueue::CJobQueue()
    {
        m_Allocator = IZ_NULL;

        m_JobWorker = IZ_NULL;
        m_JobWorkerNum = 0;

        m_Buf = IZ_NULL;

        m_IsTerminated = IZ_FALSE;
        m_IsStarted = IZ_FALSE;
        m_IsWaiting = IZ_FALSE;
    }

    CJobQueue::~CJobQueue()
    {
        // 一応
        Join();

        if (m_JobWorkerNum > 0)
        {
            for (IZ_UINT i = 0; i < m_JobWorkerNum; i++)
            {
                delete m_JobWorker[i];
            }
        }

        FREE(m_Allocator, m_Buf);
    }

    // 初期化
    IZ_BOOL CJobQueue::Init(
        IMemoryAllocator* allocator,
        IZ_UINT threadNum)
    {
        IZ_ASSERT(threadNum > 0);

        IZ_BOOL ret = (m_Buf != IZ_NULL);

        if (!ret)
        {
            // バッファサイズを計算
            size_t bufSize = ComputeBufferSize(threadNum);

            m_Buf = reinterpret_cast<IZ_UINT8*>(ALLOC_ZERO(allocator, bufSize));
            VRETURN(m_Buf != IZ_NULL);

            m_WaitEvent.Open();

            IZ_UINT8* buf = m_Buf;

            // スレッドリスト作成
            m_JobWorker = reinterpret_cast<CJobWorker**>(buf);
            buf += threadNum * sizeof(CJobWorker*);

            for (IZ_UINT i = 0; i < threadNum; i++)
            {
                m_JobWorker[i] = new (buf) CJobWorker(this);
                buf += sizeof(CJobWorker);
            }

            IZ_ASSERT(CStdUtil::GetPtrDistance(buf, m_Buf) == bufSize);

            m_JobWorkerNum = threadNum;
            m_WorkingThreadNum = 0;

            m_IsTerminated = IZ_FALSE;
            m_IsStarted = IZ_FALSE;
            m_IsWaiting = IZ_FALSE;
        }

        return ret;
    }

    // 開始
    void CJobQueue::Start()
    {
        if (m_IsTerminated || m_IsWaiting)
        {
            return;
        }

        if (!m_IsStarted)
        {
            m_IsStarted = IZ_TRUE;

            for (IZ_UINT i = 0; i < m_JobWorkerNum; i++)
            {
                m_JobWorker[i]->Start();
            }
        }
    }

    // ジョブをキューに積む.
    IZ_BOOL CJobQueue::Enqueue(CJob* job, IZ_BOOL isSync/*= IZ_FALSE*/)
    {
        IZ_ASSERT(job != IZ_NULL);

        if (isSync)
        {
            // 同期待ちするので、即実行
            job->OnExecute();
            job->OnFinished();
            return IZ_TRUE;
        }

        if (m_IsTerminated)
        {
            // もう止まっているの何もしない
            return IZ_FALSE;
        }

        IZ_BOOL ret = m_ExecJobQueue.Enqueue(
            job->GetQueueItem(),
            this,
            EnqueueAction);

        // キューに積まれたのでワーカーを動かす
        for (IZ_UINT i = 0; i < m_JobWorkerNum; i++)
        {
            m_JobWorker[i]->Resume();
        }

        return ret;
    }

    // 更新処理.
    void CJobQueue::Update()
    {
        // 終了待ちジョブを片付ける
        m_FinishJobQueue.Lock();
        {
            CStdQueue<CJob>::Item* item = m_FinishJobQueue.GetQueue().Dequeue();

            while (item != IZ_NULL)
            {
                CJob* job = item->GetData();

                if (job->WillCancel())
                {
                    job->NotifyCancel();
                }
                else
                {
                    job->OnFinished();
                }

                job->Detach();

                item = m_FinishJobQueue.GetQueue().Dequeue();
            }
        }
        m_FinishJobQueue.Unlock();
    }

    // 実行待ちキューが終了するまで待つ.
    void CJobQueue::WaitForFinishJobQueue()
    {
        if (m_IsTerminated)
        {
            return;
        }

        // 一度止める
        for (IZ_UINT i = 0; i < m_JobWorkerNum; i++)
        {
            m_JobWorker[i]->Suspend();
        }

        m_IsWaiting = IZ_TRUE;
        m_WorkingThreadNum = m_JobWorkerNum;

        m_WaitEvent.Reset();

        // 動かす
        for (IZ_UINT i = 0; i < m_JobWorkerNum; i++)
        {
            m_JobWorker[i]->Resume();
        }

        while (m_WorkingThreadNum > 0)
        {
            m_WaitEvent.Set();
        }

        m_IsWaiting = IZ_FALSE;
    }

    // スレッド終了.
    IZ_BOOL CJobQueue::Join()
    {
        if (m_IsTerminated)
        {
            return IZ_TRUE;
        }

        for (IZ_UINT i = 0; i < m_JobWorkerNum; i++)
        {
            m_JobWorker[i]->WillJoin();

            // 止まっているかもしれないので動かす
            m_JobWorker[i]->Resume();
        }

        for (IZ_UINT i = 0; i < m_JobWorkerNum; i++)
        {
            m_JobWorker[i]->Join();
        }

        m_IsTerminated = IZ_TRUE;

        // NOTE
        // この時点でスレッドは回っていないので
        // ロックする必要はない
        IZ_BOOL ret = m_FinishJobQueue.GetQueue().HasItem();

        return ret;
    }

    // 実行待ちキューに登録されているジョブが存在するかどうか.
    IZ_BOOL CJobQueue::HasJob()
    {
        IZ_BOOL ret = m_ExecJobQueue.HasItem();
        return ret;
    }

    // 終了待ちキューに登録されているジョブが存在するかどうか.
    IZ_BOOL CJobQueue::HasFinishJob()
    {
        IZ_BOOL ret = m_FinishJobQueue.HasItem();
        return ret;
    }

    // デキュー
    CJob* CJobQueue::Dequeue()
    {
        CJob* ret = m_ExecJobQueue.Dequeue();
        return ret;
    }

    // 終了ジョブとしてエンキュー
    IZ_BOOL CJobQueue::EnqueueAsFinishJob(CJob* job)
    {
        return m_FinishJobQueue.Enqueue(job->GetQueueItem(), this, IZ_NULL);
    }

    // 終了ジョブキューからデキュー
    CJob* CJobQueue::DequeueFromFinishJobQueue()
    {
        return m_FinishJobQueue.Dequeue();
    }

    // ワーカースレッドが停止したことを通知
    void CJobQueue::NotifyWorkerThreadSuspend()
    {
        if (m_IsWaiting)
        {
            IZ_INTERLOCKED_DECREMENT(&m_WorkingThreadNum);
            IZ_ASSERT(m_WorkingThreadNum >= 0);

            if (m_WorkingThreadNum == 0)
            {
                m_WaitEvent.Set();
            }
        }
    }

    // ジョブをキャンセル
    IZ_BOOL CJobQueue::Cancel(CJob* job)
    {
        IZ_BOOL ret = m_ExecJobQueue.Remove(job->GetQueueItem());
        if (!ret)
        {
            ret = m_FinishJobQueue.Remove(job->GetQueueItem());
        }
        return ret;
    }

}   // namespace threadmodel
}   // namespace izanagi
