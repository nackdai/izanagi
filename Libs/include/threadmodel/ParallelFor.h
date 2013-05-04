#if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_FOR_H__)
#define __IZANAGI_THREAD_MODEL_PARALLEL_FOR_H__

#include "threadmodel/Parallel.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CParallelFor : public CParallel
    {
    public:
        CParallelFor();
        virtual ~CParallelFor();

        NO_COPIABLE(CParallelFor);

    public:
        typedef ActionDelegate<IZ_UINT> Action;
        typedef IZ_UINT (*Func)(IZ_UINT idx);

    public:
        IZ_BOOL Init(
            IMemoryAllocator* allocator, 
            IZ_UINT num,
            Func func);

        IZ_BOOL Init(
            IMemoryAllocator* allocator,
            IZ_UINT num,
            Action* action);

    private:
        IZ_BOOL AllocJobs(
            IMemoryAllocator* allocator,
            IZ_UINT num);

    private:
        class CParallelForJob : public CParallelJob
        {
        public:
            CParallelForJob() { m_Func = IZ_NULL; }

            virtual ~CParallelForJob() {}

            IZ_DECL_PLACEMENT_NEW();

        public:
            virtual void OnExecute()
            {
                if (m_Func != IZ_NULL) {
                    (*m_Func)(m_Idx);
                }
                else {
                    (*((CParallelFor::Action*)m_Delegate))(m_Idx);
                }
            }

            void SetFunc(Func func)
            {
                m_Func = func;
            }

            void SetIdx(IZ_UINT idx)
            {
                m_Idx = idx;
            }

        private:
            IZ_UINT m_Idx;
            Func m_Func;
        };

    private:
        IZ_UINT m_Num;
        IZ_UINT m_RegisteredNum;
        CParallelForJob** m_Jobs;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_FOR_H__)
