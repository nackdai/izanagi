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

    public:
        CThreadPool();
		~CThreadPool();

		NO_COPIABLE(CThreadPool);

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
			CThread(CThreadPool* pool);
            virtual ~CThread();

            IZ_DECL_PLACEMENT_NEW();

        public:
            CStdList<CThread>::Item* GetListItem() { return &m_ListItem; }

            virtual void Run() final;

            void Terminate();

            IZ_BOOL IsWaiting();

        private:
			CThreadPool* m_Pool;

            std::mutex m_Mutex;
            
            State m_State;

            CStdList<CThread>::Item m_ListItem;
        };

    public:
        void Init(IMemoryAllocator* allocator);

        void Init(
            IMemoryAllocator* allocator,
            IZ_UINT threadNum);

        IZ_BOOL EneueueTask(CTask* task);

        void WaitEmpty();

        void Terminate();

        IZ_UINT GetMaxThreadNum();

    private:
        CThread* CreateThread();

        CTask* DequeueTask();

    private:
        IMemoryAllocator* m_Allocator;

        std::mutex m_TaskListLocker;
        sys::CEvent m_TaskWaiter;
        CStdList<CTask> m_TaskList;

        sys::CEvent m_TaskEmptyWaiter;

        IZ_UINT m_MaxThreadNum;
        CStdList<CThread> m_ThreadList;

        enum State {
			State_None = 0,
            State_Running,
            State_WillTerminate,
            State_Terminated,
        };

        State m_State;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_THREAD_POOL_H__)
