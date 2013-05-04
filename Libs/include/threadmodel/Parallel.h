#if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
#define __IZANAGI_THREAD_MODEL_PARALLEL_H__

#include "izStd.h"
#include "izSystem.h"

#include "threadmodel/ThreadModelJob.h"
#include "threadmodel/ThreadModelJobQueue.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CParallel
    {
    protected:
        CParallel();
        virtual ~CParallel();

        NO_COPIABLE(CParallel);

    public:
        void Start();

        void Join();

    protected:
        IZ_BOOL IsRunning() const;

    protected:
        IMemoryAllocator* m_Allocator;

        CJobQueue m_JobQueue;

        IZ_BOOL m_IsRunning;

        class CParallelJob : public CJob
        {
        public:
            CParallelJob()
            {
                m_Delegate = IZ_NULL;
            }

            virtual ~CParallelJob()
            {
                if (m_Delegate != IZ_NULL)
                {
                    Delegate::Delete(m_Delegate);
                }
            }

            void SetDelegate(Delegate* delegate)
            {
                m_Delegate = delegate;
            }

        protected:
            Delegate* m_Delegate;
        };
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
