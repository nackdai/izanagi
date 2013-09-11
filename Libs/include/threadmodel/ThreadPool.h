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

            IZ_BOOL IsWaiting()
            {
                return m_IsWaiting;
            }

        private:
            sys::CSemaphore m_Sema;
            IZ_BOOL m_IsTerminated;
            IZ_BOOL m_IsWaiting;

            sys::CMutex m_RunnableGuarder;

            CStdList<CThread>::Item m_ListItem;
        };

    public:
        static void Init(IMemoryAllocator* allocator);
        static void Terminate();

    private:
        static void Init(
            IMemoryAllocator* allocator,
            IZ_UINT threadNum);

        static CThread* CreateThread();

        static CThread* GetThread(sys::IRunnable* runnable);

        static CThread* GetThreadForcibliyIfNoWaitedThread(sys::IRunnable* runnable);

    private:
        static IMemoryAllocator* s_Allocator;
        static sys::CMutex s_Mutex;
        static CStdList<CThread> s_ThreadList;

        typedef CStdList<CThread>::Item ListItem;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_THREAD_POOL_H__)
