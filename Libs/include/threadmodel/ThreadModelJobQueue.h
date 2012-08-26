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
        friend class CJobWorker;

    private:
        // ロック可能キュー
        class CLockableQueue
        {
        public:
            CLockableQueue();
            ~CLockableQueue();

        public:
            // エンキュー
            IZ_BOOL Enqueue(CJobQueue* jobQueue, CJob* job);

            // デキュー
            CJob* Dequeue();

            // 削除
            IZ_BOOL Remove(CJob* job);

            // クリア
            void Clear();

            // 要素が登録されているかどうか
            IZ_BOOL HasItem();

            // 自己責任用

            void Lock();
            void Unlock();

            CStdQueue<CJob>& GetQueue() { return m_JobQueue; }

        private:
            CStdQueue<CJob> m_JobQueue;
            CMutex m_Mutex;
        };

    private:
        /** バッファサイズを計算
         */
        static size_t ComputeBufferSize(IZ_UINT threadNum);

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

        /** ジョブをキューに積む.
         */
        IZ_BOOL Enqueue(CJob* job, IZ_BOOL isSync = IZ_FALSE);

        /** ジョブキャンセル.
         */
        IZ_BOOL Cancel(CJob* job);

        /** 更新処理.
         *
         * メインスレッドで呼ぶこと
         */
        void Update();

        /** 実行待ちキューが終了するまで待つ.
         *
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

        // イベント待機
        IZ_BOOL WaitEvent();

        // イベントを非シグナル状態にする
        void ResetEvent();

    private:
        IMemoryAllocator* m_Allocator;

        // 実行待ちキュー
        CLockableQueue m_ExecJobQueue;

        // 終了待ちキュー
        CLockableQueue m_FinishJobQueue;

        // ワーカースレッドリスト
        CJobWorker** m_JobWorker;
        IZ_UINT m_JobWorkerNum;

        // バッファ
        IZ_UINT8* m_Buf;

        // キューの状態によってスレッドを止めたり動かしたりする用
        CEvent m_Event;

        IZ_BOOL m_IsTerminated;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_QUEUE_H__)
