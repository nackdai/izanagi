#if !defined(__IZANAGI_THREAD_MODEL_TASK_SCHEDULER_H__)
#define __IZANAGI_THREAD_MODEL_TASK_SCHEDULER_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    class CTask;

    class CTaskScheduler
    {
        class CTaskExecuter : public sys::IRunnable
        {
        public:
            CTaskExecuter() {}
            virtual ~CTaskExecuter() {}

            virtual void Run(void* userData);

            void SetScheduler(CTaskScheduler* scheduler);

        private:
            CTaskScheduler* m_Scheduler;
        };

    public:
        CTaskScheduler();
        virtual ~CTaskScheduler();

        NO_COPIABLE(CTaskScheduler);

    public:
        IZ_BOOL Enqueue(CTask& task, IZ_BOOL isWaitThreadEmpty = IZ_TRUE);

    private:
        void ExecuteTask();

    private:
        sys::CMutex m_Mutex;
        CStdList<CTask> m_Tasks;

        sys::CSemaphore m_Sema;

        CTaskExecuter m_Executer;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TASK_SCHEDULER_H__)
