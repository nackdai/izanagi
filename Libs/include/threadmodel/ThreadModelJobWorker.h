#if !defined(__IZANAGI_THREAD_MODEL_JOB_WORKER_H__)
#define __IZANAGI_THREAD_MODEL_JOB_WORKER_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    class CJob;

    /** ジョブを実行するワーカースレッド
     */
    class CJobWorker
    {
        friend class CJobQueue;

    private:
        enum State
        {
            State_Waiting = 0,
            State_Registered,
            State_Running,
            State_WillJoin,
            State_Joined,
        };

    private:
        CJobWorker();
        virtual ~CJobWorker();

        NO_COPIABLE(CJobWorker);

        IZ_DECL_PLACEMENT_NEW();

    private:
        void Register(CJob* job);

        void Start();

        void Join();

        // 状態を取得
        State GetState();

        IZ_BOOL IsWaiting();

    private:
        sys::CSemaphore m_Sema;
        sys::CMutex m_Mutex;

        CJob* m_Job;

        State m_State;

        sys::CThread* m_Thread;

        class CRunnable : public sys::IRunnable
        {
        public:
            CRunnable() {}
            ~CRunnable() {}

            virtual void Run(void* data);

            CJobWorker* worker;
        };

        CRunnable m_Runner;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_WORKER_H__)
