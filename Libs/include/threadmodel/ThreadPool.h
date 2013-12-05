#if !defined(__IZANAGI_THREAD_MODEL_THREAD_POOL_H__)
#define __IZANAGI_THREAD_MODEL_THREAD_POOL_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
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
                State_None = 0,
                State_Waiting,
                State_Registered,
                State_Running,
                State_WillFinish,
                State_Finished,
            };

        public:
            CThread();
            virtual ~CThread() {}

            IZ_DECL_PLACEMENT_NEW();

        public:
            void SetRunnable(sys::IRunnable* runnable);
            sys::IRunnable* GetRunnable();

            CStdList<CThread>::Item* GetListItem() { return &m_ListItem; }

            virtual void Run();
            virtual void Join();

            void Terminate();

            IZ_BOOL IsWaiting();

        private:
            sys::CSemaphore m_Sema;
            sys::CEvent m_Event;
            sys::CMutex m_Mutex;
            
            State m_State;

            CStdList<CThread>::Item m_ListItem;
        };

    public:
        static void Init(IMemoryAllocator* allocator);

        static void Init(
            IMemoryAllocator* allocator,
            IZ_UINT threadNum);

        static CThread* GetThread(sys::IRunnable* runnable);

        static CThread* GetThreadUntilThreadIsEmpty(sys::IRunnable* runnable);

        static void Terminate();

        static IZ_UINT GetMaxThreadNum();

    private:
        static CThread* CreateThread();
        static CThread* GetThreadCore(sys::IRunnable* runnable);

    private:
        static IMemoryAllocator* s_Allocator;
        static sys::CMutex s_Mutex;

        static IZ_UINT s_MaxThreadNum;
        static CStdList<CThread> s_ThreadList;

        static sys::CMutex s_CurrentThreadNumLocker;
        static sys::CEvent s_ThreadEmptyWaiter;
        static IZ_UINT s_CurrentThreadNum;

        typedef CStdList<CThread>::Item ListItem;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_THREAD_POOL_H__)
