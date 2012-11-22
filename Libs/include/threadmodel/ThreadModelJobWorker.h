#if !defined(__IZANAGI_THREAD_MODEL_JOB_WORKER_H__)
#define __IZANAGI_THREAD_MODEL_JOB_WORKER_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    class CJobQueue;

    /** ジョブを実行するワーカースレッド
     */
    class CJobWorker : public CThread
    {
        friend class CJobQueue;

    private:
        CJobWorker(CJobQueue* jobQueue);
        ~CJobWorker() {}

        NO_COPIABLE(CJobWorker);

        IZ_DECL_PLACEMENT_NEW();

    private:
        virtual void Run();

        // このスレッドの実行を開始.
        IZ_BOOL Start();

        void Resume();

        void Suspend();

        void WillJoin() { m_WillJoin = IZ_TRUE; }

    private:
        CJobQueue* m_JobQueue;
        
        CEvent m_Event;

        IZ_BOOL m_WillJoin;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_WORKER_H__)
