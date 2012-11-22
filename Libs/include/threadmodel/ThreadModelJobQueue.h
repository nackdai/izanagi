#if !defined(__IZANAGI_THREAD_MODEL_JOB_QUEUE_H__)
#define __IZANAGI_THREAD_MODEL_JOB_QUEUE_H__

#include "izStd.h"
#include "izSystem.h"

#include "threadmodel/ThreadSafeValue.h"
#include "threadmodel/ThreadSafeQueue.h"

namespace izanagi
{
namespace threadmodel
{
    class CJob;
    class CJobWorker;

    /** ジョブキュー
     */
    class CJobQueue
    {
        friend class CJobWorker;
        friend class CJob;

    private:
        // バッファサイズを計算
        static size_t ComputeBufferSize(IZ_UINT threadNum);

        // エンキュー時の処理
        static void EnqueueAction(CJob* job, void* jobQueue);

    public:
        CJobQueue();
        ~CJobQueue();

        NO_COPIABLE(CJobQueue);

    public:
        /** 初期化.
         */
        IZ_BOOL Init(
            IMemoryAllocator* allocator,
            IZ_UINT threadNum);

        /** 開始
         */
        void Start();

        /** ジョブをキューに積む.
         */
        IZ_BOOL Enqueue(CJob* job, IZ_BOOL isSync = IZ_FALSE);

        /** 更新処理.
         *
         * メインスレッドで呼ぶこと
         */
        void Update();

        /** 実行待ちキューが終了するまで待つ.
         *
         * メインスレッドで呼ぶこと
         */
        void WaitForFinishJobQueue();

        /** スレッド終了.
         *
         * 実行待ちキューが終了するまで待つ.
         * メインスレッドで呼ぶこと
         *
         * @return 終了待ちキューが存在する場合はtrueを返す
         */
        IZ_BOOL Join();

        /** 実行待ちキューに登録されているジョブが存在するかどうか.
         */
        IZ_BOOL HasJob();

        /** 終了待ちキューに登録されているジョブが存在するかどうか.
         */
        IZ_BOOL HasFinishJob();

    private:
        // デキュー
        CJob* Dequeue();

        // 終了ジョブとしてエンキュー
        IZ_BOOL EnqueueAsFinishJob(CJob* job);

        // 終了ジョブキューからデキュー
        CJob* DequeueFromFinishJobQueue();

        // ワーカースレッドが停止したことを通知
        void NotifyWorkerThreadSuspend();

        // ジョブをキャンセル
        IZ_BOOL Cancel(CJob* job);

    private:
        IMemoryAllocator* m_Allocator;

        // 実行待ちキュー
        CThreadSafeQueue<CJob> m_ExecJobQueue;

        // 終了待ちキュー
        CThreadSafeQueue<CJob> m_FinishJobQueue;

        // ワーカースレッドリスト
        CJobWorker** m_JobWorker;
        IZ_UINT m_JobWorkerNum;

        // バッファ
        IZ_UINT8* m_Buf;

        CEvent m_WaitEvent;
        IZ_INT m_WorkingThreadNum;

        IZ_BOOL m_IsTerminated;
        IZ_BOOL m_IsStarted;
        CThreadSafeFlag m_IsWaiting;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_QUEUE_H__)
