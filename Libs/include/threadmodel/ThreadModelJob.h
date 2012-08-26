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
        IZ_BOOL Cancel();

    private:
        CStdQueue<CJob>::Item* GetQueueItem() { return &m_QueueItem; }

        // ジョブキューに登録されるときに呼ばれる
        void Registered(CJobQueue* jobQueue);

        // 処理が終了してジョブキューから外れるときに呼ばれる
        void Detached();

    private:
        CStdQueue<CJob>::Item m_QueueItem;

        // 登録されているジョブキュー
        CJobQueue* m_JobQueue;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_H__)
