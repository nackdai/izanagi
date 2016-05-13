#include "threadmodel/Parallel.h"

namespace izanagi
{
namespace threadmodel
{
    static const IZ_INT PARALLEL_CHUNK_SIZE = 4;

    void CParallelFor::init(IZ_INT from, IZ_INT to, std::function<void(IZ_INT)> func)
    {
        m_From = from;
        m_To = to;

        m_function = func;
    }

    void CParallelFor::OnRun()
    {
        for (IZ_INT i = m_From; i < m_To; i++) {
            if (m_function) {
                m_function(i);
            }
        }
    }

    static CParallelFor s_taskFor[10];

#if 0
    void CParallel::waitFor()
    {
#ifdef WAIT_OUTSIDE
        // タスクが終了するのを待つ.
        waitFor(s_taskFor, COUNTOF(s_taskFor));
#endif
    }
#else
    void CParallel::waitFor(CParallelFor* tasks, IZ_UINT num)
    {
        // タスクが終了するのを待つ.
        CParallelTask::wait(tasks, num);
    }
#endif

	// 指定した範囲で処理を実行する.
    void CParallel::For(
		CThreadPool& threadPool,
        CParallelFor* tasks,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        std::function<void(IZ_INT)> func)
    {
        //waitFor();

		// from > to になるようにする
		if (fromInclusive > toExclusive) {
			IZ_INT tmp = toExclusive;
			toExclusive = fromInclusive;
			fromInclusive = tmp;
		}

#ifdef WAIT_OUTSIDE
        //#define TASKS   s_taskFor
        #define TASKS   tasks
#else
        // 作成したタスク保持用.
        CParallelFor tasks[10];

        #define TASKS   tasks
#endif

		// 利用されるスレッド数.
		IZ_INT threadCount = sys::CEnvironment::GetProcessorNum();

		// スレッドプールに設定されている回数と調整する.
		threadCount = ((IZ_INT)threadPool.GetMaxThreadNum() > threadCount
			? threadCount
			: threadPool.GetMaxThreadNum());

#if 0
        if (threadCount > COUNTOF(TASKS)) {
            threadCount = COUNTOF(TASKS);
		}
#endif

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

        threadPool.beginEnqueueWithoutWait();

		for (IZ_INT i = 0; i < threadCount; i++) {
			IZ_INT to = from + step;
			if (to >= toExclusive) {
				to = toExclusive;
			}

            auto task = &TASKS[i];
            task->init(from, to, func);
            task->beginWorking();

			// スレッドプールに登録.
			//threadPool.EneueueTask(task);
            threadPool.enqueueWithoutWait(task);

			from = to;
			if (from >= toExclusive) {
				break;
			}
		}

        threadPool.endEnqueueWithoutWait();

#ifndef WAIT_OUTSIDE
		// タスクが終了するのを待つ.
        wait(tasks, threadCount);
#endif

#undef TASKS
    }

    ////////////////////////////////////////////////////////

    void CParallelForEach::init(
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

    void CParallelForEach::OnRun()
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

    static CParallelForEach s_taskForEach[10];

#if 0
    void CParallel::waitForEach()
    {
#ifdef WAIT_OUTSIDE
        // タスクが終了するのを待つ.
        waitForEach(s_taskForEach, COUNTOF(s_taskForEach));
#endif
    }
#else
    void CParallel::waitForEach(CParallelForEach* tasks, IZ_UINT num)
    {
        CParallelTask::wait(tasks, num);
    }
#endif

	// 指定された回数だけ処理を実行する.
    void CParallel::ForEach(
		CThreadPool& threadPool,
        CParallelForEach* tasks,
        void* data, size_t stride,
        IZ_UINT count,
		std::function<void(void*)> func)
    {
        //waitForEach();

#ifdef WAIT_OUTSIDE
        //#define TASKS   s_taskForEach
        #define TASKS   tasks
#else
        // 作成したタスク保持用.
        CParallelForEach tasks[10];
        #define TASKS   tasks
#endif

		// 利用されるスレッド数.
        IZ_UINT threadCount = sys::CEnvironment::GetProcessorNum();

		// スレッドプールに設定されている回数と調整する.
		threadCount = ((IZ_INT)threadPool.GetMaxThreadNum() > threadCount
			? threadCount
			: threadPool.GetMaxThreadNum());

#if 0
        if (threadCount > COUNTOF(TASKS)) {
            threadCount = COUNTOF(TASKS);
        }
#endif

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

        threadPool.beginEnqueueWithoutWait();

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
			//threadPool.EneueueTask(task);
            threadPool.enqueueWithoutWait(task);

            from = to;
            if (from >= count) {
                break;
            }
        }

        threadPool.endEnqueueWithoutWait();

#ifndef WAIT_OUTSIDE
		// タスクが終了するのを待つ.
        wait(tasks, threadCount);
#endif

#undef TASKS
    }
}   // namespace threadmodel
}   // namespace izanagi
