#if !defined(__IZANAGI_THREAD_MODEL_THREAD_POOL_H__)
#define __IZANAGI_THREAD_MODEL_THREAD_POOL_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    class CTask;

    class CThreadPool
    {
        friend class CJobWorker;
        friend class CParallel;

    private:
        CThreadPool();
        ~CThreadPool();

    private:
        class CThread : public sys::CThread
        {
            enum State
            {
                State_Waiting = 0,
                State_Running,
                State_WillFinish,
                State_Finished,
            };

        public:
            CThread();
            virtual ~CThread();

            IZ_DECL_PLACEMENT_NEW();

        public:
            CStdList<CThread>::Item* GetListItem() { return &m_ListItem; }

            virtual void Run();

            void Terminate();

            IZ_BOOL IsWaiting();

        private:
            sys::CMutex m_Mutex;
            
            State m_State;

            CStdList<CThread>::Item m_ListItem;
        };

    public:
        static void Init(IMemoryAllocator* allocator);

        static void Init(
            IMemoryAllocator* allocator,
            IZ_UINT threadNum);

        static void EneueueTask(CTask* task);

        static void WaitEmpty();

        static void Terminate();

        static IZ_UINT GetMaxThreadNum();

    private:
        static CThread* CreateThread();

        static CTask* DequeueTask();

    private:
        static IMemoryAllocator* s_Allocator;

        static sys::CMutex s_TaskListLocker;
        static sys::CEvent s_TaskWaiter;
        static CStdList<CTask> s_TaskList;

        static sys::CEvent s_TaskEmptyWaiter;

        static IZ_UINT s_MaxThreadNum;
        static CStdList<CThread> s_ThreadList;

        enum State {
            State_Running = 0,
            State_WillTerminate,
            State_Terminated,
        };

        static State s_State;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_THREAD_POOL_H__)
