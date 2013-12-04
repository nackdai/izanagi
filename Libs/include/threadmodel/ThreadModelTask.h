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
        friend class CTaskScheduler;
        friend class CParallel;

    public:
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

        static void DeleteTask(CTask* task);

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
        virtual void OnRun() = 0;

    private:
        virtual void Run(void* userData);

        CStdList<CTask>::Item* GetListItem() { return &m_ListItem; }
        void SetAllocator(IMemoryAllocator* allocator)
        {
            m_Allocator = allocator;
        }

    private:
        IMemoryAllocator* m_Allocator;

        CStdList<CTask>::Item m_ListItem;

        sys::CEvent m_Event;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_TASK_H__)
