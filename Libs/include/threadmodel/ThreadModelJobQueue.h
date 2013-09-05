#if !defined(__IZANAGI_THREAD_MODEL_JOB_QUEUE_H__)
#define __IZANAGI_THREAD_MODEL_JOB_QUEUE_H__

#include "izStd.h"
#include "izSystem.h"

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
        static sys::CMutex s_QueueListGuarder;

        // JobQueue登録リスト
        static CStdList<CJobQueue> s_JobQueueList;

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

        /** 開始.
         */
        IZ_BOOL Start();

        /** ジョブをキューに積む.
         */
        IZ_BOOL Enqueue(CJob* job);

        /** スレッド終了.
         *
         * 実行待ちキューが終了するまで待つ.
         * メインスレッドで呼ぶこと
         *
         * @return 終了待ちキューが存在する場合はtrueを返す
         */
        void WaitEmpty();

        /** 登録ジョブのキャンセル.
         */
        void CancelAllJobs();

        /** JobQueueの終了.
         */
        void Terminate();

        /** すべてのJobQueueの更新.
         */
        static void UpdateQueues();

        /** JobQueueシステムの終了.
         */
        static void TerminateJobQueue();

    private:
        // 更新処理.
        void Update();

        // 空いているワーカーを取得.
        CJobWorker* GetWaitingJobWorker();

    private:
        IMemoryAllocator* m_Allocator;

        IZ_UINT m_WorkerNum;
        CJobWorker** m_Workers;

        sys::CMutex m_JobListGuarder;

        // Job登録リスト
        CStdList<CJob> m_JobList;

        CStdList<CJobQueue>::Item m_ListItem;

        IZ_BOOL m_IsTerminated;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_QUEUE_H__)
