#if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_INVOKE_H__)
#define __IZANAGI_THREAD_MODEL_PARALLEL_INVOKE_H__

#include "threadmodel/Parallel.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CParallelInvoke : public CParallel
    {
    public:
        CParallelInvoke();
        virtual ~CParallelInvoke();

        NO_COPIABLE(CParallelInvoke);

    public:
        typedef ActionDelegate<IZ_UINT> Action;
        typedef void (*Func)(IZ_UINT);

    public:
        IZ_BOOL Init(IMemoryAllocator* allocator, IZ_UINT num);

        IZ_BOOL Register(Action* action);
        IZ_BOOL Register(Func func);

    private:
        class CParallelInvokeJob : public CParallelJob
        {
        public:
            CParallelInvokeJob() { m_Func = IZ_NULL; }

            virtual ~CParallelInvokeJob() {}

            IZ_DECL_PLACEMENT_NEW();

        public:
            virtual void OnExecute()
            {
                if (m_Func != IZ_NULL) {
                    (*m_Func)(m_Idx);
                }
                else {
                    (*((CParallelInvoke::Action*)m_Delegate))(m_Idx);
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
        CParallelInvokeJob** m_Jobs;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_INVOKE_H__)
