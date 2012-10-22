#if !defined(__IZANAGI_THREAD_MODEL_JOB_H__)
#define __IZANAGI_THREAD_MODEL_JOB_H__

#include "izStd.h"

namespace izanagi
{
namespace threadmodel
{
    class CJobQueue;

    /** ジョブのベース
     */
    class CJob
    {
        friend class CJobQueue;
        friend class CJobWorker;

    protected:
        CJob();

        virtual ~CJob()
        {
        }

    public:
        /**
         */
        virtual void OnExecute() = 0;

        /**
         */
        virtual void OnFinished() = 0;

        /** キャンセル.
         */
        void Cancel();

        /** キャンセルリクエストされているかどうか
         */
        IZ_BOOL WillCancel()
        {
            return m_WillCancel;
        }

        /** キャンセルされたかどうか
         */
        IZ_BOOL IsCanceled()
        {
            return m_IsCanceled;
        }

        /** すでに終了しているかどうか
         */
        IZ_BOOL IsFinished()
        {
            return m_IsFinished;
        }

    private:
        CStdQueue<CJob>::Item* GetQueueItem() { return &m_QueueItem; }

        // 指定されたジョブキューに登録済みかどうか
        IZ_BOOL IsRegistered(CJobQueue* jobQueue);

        // エンキューのための準備
        void Prepare(CJobQueue* jobQueue);

        // 処理が終了してジョブキューから外れるときに呼ばれる
        void Detach();

        void NotifyCancel();

    private:
        CStdQueue<CJob>::Item m_QueueItem;

        // 登録されているジョブキュー
        CJobQueue* m_JobQueue;

        IZ_BOOL m_IsFinished;
        IZ_BOOL m_WillCancel;
        IZ_BOOL m_IsCanceled;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_H__)
