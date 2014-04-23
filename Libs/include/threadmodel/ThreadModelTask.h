#if !defined(__IZANAGI_THREAD_MODEL_TASK_H__)
#define __IZANAGI_THREAD_MODEL_TASK_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CTask : public CPlacementNew, public sys::IRunnable
    {
        friend class CThreadPool;
        friend class CParallel;
        friend class CTimerTaskExecuter;

    public:
        template <typename _T>
        static _T* CreateTask(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T();
            {
                ret->m_Allocator = allocator;
            }

            return ret;
        }

        static void DeleteTask(CTask* task);

    protected:
        CTask();
        virtual ~CTask();

    public:
        /**
         */
        void Wait();

        /**
         */
        IZ_BOOL Cancel();

        /**
         */
        IZ_BOOL WillCancel();

        /**
         */
        IZ_BOOL IsCanceled();

        /**
         */
        IZ_BOOL IsFinished();

    protected:
        /**
         */
        virtual void OnRun() = 0;

    private:
        virtual void Run(void* userData);

        void ResetState();

        CStdList<CTask>::Item* GetListItem();

        void SetAllocator(IMemoryAllocator* allocator);

        void SetIsDeleteSelf(IZ_BOOL isDeleteSelf);
        IZ_BOOL IsDeleteSelf();

    private:
        IMemoryAllocator* m_Allocator;

        CStdList<CTask>::Item m_ListItem;

        sys::CEvent m_Event;

        IZ_BOOL m_IsDeleteSelf;

        enum State {
            State_Init = 0,
            State_Registered,
            State_Running,
            State_Finished,
            State_WillCancel,
            State_Canceled,
        };

        sys::CMutex m_StateLocker;
        State m_State;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TASK_H__)
