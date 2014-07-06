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
    class CJobWorker : public sys::CThread
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
        CJobWorker(const sys::ThreadName& name);
        virtual ~CJobWorker();

        NO_COPIABLE(CJobWorker);

        IZ_DECL_PLACEMENT_NEW();

    private:
        virtual void Run();

        void Register(CJob* job);

        void Join();

        // 状態を取得
        State GetState();

        IZ_BOOL IsWaiting();

        inline void SetState(State state);

    private:
        sys::CSemaphore m_Sema;
        sys::CMutex m_Mutex;

        CJob* m_Job;

        State m_State;
        sys::CMutex m_StateMutex;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_WORKER_H__)
