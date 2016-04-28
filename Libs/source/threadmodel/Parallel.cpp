#include "threadmodel/Parallel.h"
#include "threadmodel/ThreadPool.h"
#include "threadmodel/ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
    class CParallelTask : public CTask
    {
    public:
        CParallelTask() {}
        virtual ~CParallelTask() {}

    public:
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

    private:
        IZ_BOOL m_isWorking{ IZ_FALSE };
    };

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

    static const IZ_INT PARALLEL_CHUNK_SIZE = 4;

	// forループ処理用タスククラス.
    class CParallelFor : public CParallelTask
    {
    public:
        CParallelFor() {}
        virtual ~CParallelFor() {}

    public:
        void init(IZ_INT from, IZ_INT to, std::function<void(IZ_INT)> func)
        {
            m_From = from;
            m_To = to;

            m_function = func;
        }

        virtual void OnRun() override
        {
            for (IZ_INT i = m_From; i < m_To; i++) {
				if (m_function) {
					m_function(i);
				}
            }
        }

    protected:
        IZ_INT m_From;
        IZ_INT m_To;

		std::function<void(IZ_INT)> m_function;
    };

    static CParallelFor s_taskFor[10];

    void CParallel::waitFor()
    {
#ifdef WAIT_OUTSIDE
        // タスクが終了するのを待つ.
        wait(s_taskFor, COUNTOF(s_taskFor));
#endif
    }

	// 指定した範囲で処理を実行する.
    void CParallel::For(
		CThreadPool& threadPool,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        std::function<void(IZ_INT)> func)
    {
        waitFor();

		// from > to になるようにする
		if (fromInclusive > toExclusive) {
			IZ_INT tmp = toExclusive;
			toExclusive = fromInclusive;
			fromInclusive = tmp;
		}

		// 作成したタスク保持用.
        CParallelFor tasks[10];

#ifdef WAIT_OUTSIDE
        #define TASKS   s_taskFor
#else
        #define TASKS   tasks
#endif

		// 利用されるスレッド数.
		IZ_INT threadCount = sys::CEnvironment::GetProcessorNum();

		// スレッドプールに設定されている回数と調整する.
		threadCount = ((IZ_INT)threadPool.GetMaxThreadNum() > threadCount
			? threadCount
			: threadPool.GetMaxThreadNum());

        if (threadCount > COUNTOF(TASKS)) {
            threadCount = COUNTOF(TASKS);
		}

		// １スレッドあたりのループ回数.
		IZ_INT step = (toExclusive - fromInclusive) / threadCount;

		// TODO
		// ループ回数が一定以下の場合は調整する.
		// 回数は適当.
		if (step < 5) {
			step = 5;

			// 利用されるスレッド数を調整.
			threadCount = (toExclusive - fromInclusive) / step;
			if (threadCount < 1) {
				threadCount = 1;
			}
		}

		// toの回数を下回らないように調整.
		while (step * threadCount < toExclusive) {
			step++;
		}

		IZ_INT from = fromInclusive;

		for (IZ_INT i = 0; i < threadCount; i++) {
			IZ_INT to = from + step;
			if (to >= toExclusive) {
				to = toExclusive;
			}

            auto task = &TASKS[i];
            task->init(from, to, func);
            task->beginWorking();

			// スレッドプールに登録.
			threadPool.EneueueTask(task);

			from = to;
			if (from >= toExclusive) {
				break;
			}
		}

#ifndef WAIT_OUTSIDE
		// タスクが終了するのを待つ.
        wait(tasks, threadCount);
#endif

#undef TASKS
    }

    ////////////////////////////////////////////////////////

	// foreach処理用タスククラス
    class CParallelForEach : public CParallelTask
    {
    public:
        CParallelForEach() {}
        virtual ~CParallelForEach() {}

    public:
        void init(
            IZ_UINT8* p, size_t stride,
            IZ_UINT from, IZ_UINT to,
            std::function<void(void*)> func)
        {
            m_Ptr = p;
            m_Stride = stride;
            m_From = from;
            m_To = to;

            m_function = func;
        }

        virtual void OnRun() override
        {
            if (m_Ptr != IZ_NULL) {
                IZ_UINT8* ptr = m_Ptr;

                for (IZ_UINT i = m_From; i < m_To; i++) {
                    IZ_UINT8* p = ptr + i * m_Stride;

					if (m_function) {
						m_function(p);
					}
                }
            }
        }

    private:
        IZ_UINT8* m_Ptr;
        size_t m_Stride;
        IZ_UINT m_From;
        IZ_UINT m_To;

		std::function<void(void*)> m_function;
    };

    static CParallelForEach s_taskForEach[10];

    void CParallel::waitForEach()
    {
#ifdef WAIT_OUTSIDE
        // タスクが終了するのを待つ.
        wait(s_taskForEach, COUNTOF(s_taskForEach));
#endif
    }

	// 指定された回数だけ処理を実行する.
    void CParallel::ForEach(
		CThreadPool& threadPool,
        void* data, size_t stride,
        IZ_UINT count,
		std::function<void(void*)> func)
    {
        waitForEach();

		// 作成したタスク保持用.
        CParallelForEach tasks[10];

#ifdef WAIT_OUTSIDE
        #define TASKS   s_taskForEach
#else
        #define TASKS   tasks
#endif

		// 利用されるスレッド数.
        IZ_UINT threadCount = sys::CEnvironment::GetProcessorNum();

		// スレッドプールに設定されている回数と調整する.
		threadCount = ((IZ_INT)threadPool.GetMaxThreadNum() > threadCount
			? threadCount
			: threadPool.GetMaxThreadNum());

        if (threadCount > COUNTOF(TASKS)) {
            threadCount = COUNTOF(TASKS);
        }

		// １ループあたりの処理回数.
        IZ_UINT step = count / threadCount;
        
		// TODO
		// 処理回数が一定以下の場合は調整する.
		// 回数は適当.
        if (step < 5) {
            step = 5;

			// 指定された処理回数は超えないようにする.
            if (step > count) {
                step = count;
            }

			// 利用されるスレッド数を調整.
            threadCount = count / step;
            if (threadCount < 1) {
                threadCount = 1;
            }
        }

		// 指定された処理回数を下回らないようにする.
        while (step * threadCount < count) {
            step++;
        }

        IZ_UINT8* ptr = (IZ_UINT8*)data;
        IZ_UINT from = 0;

        for (IZ_UINT i = 0; i < threadCount; i++) {
            IZ_UINT to = from + step;
            if (to >= count) {
                to = count;
            }

            auto task = &TASKS[i];
            task->init(
                ptr, stride, from, to,
                func);
            task->beginWorking();

			// スレッドプールに登録.
			threadPool.EneueueTask(task);

            from = to;
            if (from >= count) {
                break;
            }
        }

#ifndef WAIT_OUTSIDE
		// タスクが終了するのを待つ.
        wait(tasks, threadCount);
#endif

#undef TASKS
    }
}   // namespace threadmodel
}   // namespace izanagi
