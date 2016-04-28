#if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
#define __IZANAGI_THREAD_MODEL_PARALLEL_H__

#include "izStd.h"
#include "izSystem.h"

#include "threadmodel/ThreadPool.h"

#define WAIT_OUTSIDE

namespace izanagi
{
namespace threadmodel
{
    class CTask;

    /** Parallel like C#.
     */
    class CParallel
    {
    private:
        CParallel();
        ~CParallel();

#ifdef WAIT_OUTSIDE
        static CTask* s_tasks[10];
#endif

    public:
		/** �w�肵���͈͂ŏ��������s����.
		 */
		static void For(
			CThreadPool& threadPool,
			IMemoryAllocator* allocator,
			IZ_INT fromInclusive, IZ_INT toExclusive,
			std::function<void(IZ_INT)> func);

		/** �w�肳�ꂽ�񐔂������������s����.
		 */
		static void ForEach(
			CThreadPool& threadPool,
			IMemoryAllocator* allocator,
			void* data, size_t stride,
			IZ_UINT count,
			std::function<void(void*)> func);

        static void wait();

    private:
        static void SetAllocator(CTask* task, IMemoryAllocator* allocator);
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
