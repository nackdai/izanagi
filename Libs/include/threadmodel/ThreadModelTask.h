#if !defined(__IZANAGI_THREAD_MODEL_TASK_H__)
#define __IZANAGI_THREAD_MODEL_TASK_H__

#include "izStd.h"
#include "threadmodel/ThreadModelJob.h"

namespace izanagi
{
namespace threadmodel
{
    class CJobQueue;

    /** ジョブのベース
     */
    class CTask : public CJob
    {
        template <typename _T>
        static CTask* CreateTask(IMemoryAllocator* allocator)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T();
            {
                ret->m_Allocator = allocator;
            }

            return ret;
        }

        static void DeleteTask(CTask*& task)
        {
            CJob::DeleteJob((CJob*&)task);
        }

    protected:
        CTask();
        virtual ~CTask();

    public:
        /**
         */
        void Wait();

    protected:
        /**
         */
        virtual IZ_BOOL RunTask() = 0;

    private:
        virtual IZ_BOOL OnRun();

    private:
        void Cancel() {}
        IZ_BOOL IsCanceled() { return IZ_FALSE; }

        virtual void OnFinish(IZ_BOOL runResult) {}
        virtual void OnCancel() {}

    private:
        sys::CEvent m_Event;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TASK_H__)
