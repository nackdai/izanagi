#if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
#define __IZANAGI_THREAD_MODEL_PARALLEL_H__

#include "izStd.h"
#include "izSystem.h"

#include "threadmodel/ThreadPool.h"
#include "threadmodel/ThreadModelTask.h"

#define WAIT_OUTSIDE

namespace izanagi
{
namespace threadmodel
{
    class CParallelTask : public CTask
    {
        friend class CParallel;

    public:
        CParallelTask() {}
        virtual ~CParallelTask() {}

    private:
        void beginWorking()
        {
            m_isWorking = IZ_TRUE;
        }
        void endWorking()
        {
            m_isWorking = IZ_FALSE;
        }
        IZ_BOOL isWorking()
        {
            return m_isWorking;
        }

        template <typename _T>
        static void wait(_T* tasks, IZ_UINT num)
        {
            for (IZ_UINT i = 0; i < num; i++) {
                if (tasks[i].isWorking()) {
                    tasks[i].Wait();
                    tasks[i].endWorking();
                }
            }
        }

    private:
        IZ_BOOL m_isWorking{ IZ_FALSE };
    };

    // forループ処理用タスククラス.
    class CParallelFor : public CParallelTask
    {
        friend class CParallel;

    public:
        CParallelFor() {}
        virtual ~CParallelFor() {}

    private:
        void init(IZ_INT from, IZ_INT to, std::function<void(IZ_INT)> func);
        virtual void OnRun() override;

    protected:
        IZ_INT m_From;
        IZ_INT m_To;

        std::function<void(IZ_INT)> m_function;
    };


    // foreach処理用タスククラス
    class CParallelForEach : public CParallelTask
    {
        friend class CParallel;

    public:
        CParallelForEach() {}
        virtual ~CParallelForEach() {}

    private:
        void init(
            IZ_UINT8* p, size_t stride,
            IZ_UINT from, IZ_UINT to,
            std::function<void(void*)> func);

        virtual void OnRun() override;

    private:
        IZ_UINT8* m_Ptr;
        size_t m_Stride;
        IZ_UINT m_From;
        IZ_UINT m_To;

        std::function<void(void*)> m_function;
    };

    /** Parallel like C#.
     */
    class CParallel
    {
    private:
        CParallel();
        ~CParallel();

    public:
		/** 指定した範囲で処理を実行する.
		 */
		static void For(
			CThreadPool& threadPool,
            CParallelFor* tasks,
			IZ_INT fromInclusive, IZ_INT toExclusive,
			std::function<void(IZ_INT)> func);

		/** 指定された回数だけ処理を実行する.
		 */
		static void ForEach(
			CThreadPool& threadPool,
            CParallelForEach* tasks,
			void* data, size_t stride,
			IZ_UINT count,
			std::function<void(void*)> func);

#if 0
        static void waitFor();
        static void waitForEach();
#else
        static void waitFor(CParallelFor* tasks, IZ_UINT num);
        static void waitForEach(CParallelForEach* tasks, IZ_UINT num);
#endif
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
