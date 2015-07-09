#include "threadmodel/Parallel.h"
#include "threadmodel/ThreadPool.h"
#include "threadmodel/ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
    void CParallel::SetAllocator(CTask* task, IMemoryAllocator* allocator)
    {
        task->SetAllocator(allocator);
    }

    static const IZ_INT PARALLEL_CHUNK_SIZE = 4;

	// forループ処理用タスククラス.
    class CParallelFor : public CTask
    {
    public:
		static CTask* Create(
            IMemoryAllocator* allocator,
            IZ_INT from, IZ_INT to,
			std::function<void(IZ_INT)> func)
        {
			void* buf = ALLOC(allocator, sizeof(CParallelFor));
            VRETURN_NULL(buf != IZ_NULL);

			CParallelFor* ret = new(buf)CParallelFor(from, to, func);
            return ret;
        }

    protected:
		CParallelFor(IZ_INT from, IZ_INT to, std::function<void(IZ_INT)> func)
        {
            m_From = from;
            m_To = to;

			m_function = func;
        }
        virtual ~CParallelFor() {}

    public:
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

	// 指定した範囲で処理を実行する.
    void CParallel::For(
		CThreadPool& threadPool,
        IMemoryAllocator* allocator,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        std::function<void(IZ_INT)> func)
    {
		// from > to になるようにする
		if (fromInclusive > toExclusive) {
			IZ_INT tmp = toExclusive;
			toExclusive = fromInclusive;
			fromInclusive = tmp;
		}

		// 作成したタスク保持用.
		CTask* tasks[10] = { IZ_NULL };

		// 利用されるスレッド数.
		IZ_INT threadCount = sys::CEnvironment::GetProcessorNum();

		// スレッドプールに設定されている回数と調整する.
		threadCount = ((IZ_INT)threadPool.GetMaxThreadNum() > threadCount
			? threadCount
			: threadPool.GetMaxThreadNum());

		if (threadCount > COUNTOF(tasks)) {
			threadCount = COUNTOF(tasks);
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

			// 実行インスタンスを作成
			CTask* task = CParallelFor::Create(
				allocator,
				from, to,
				func);

			IZ_ASSERT(task != IZ_NULL);

			SetAllocator(task, allocator);
			tasks[i] = task;

			// スレッドプールに登録.
			threadPool.EneueueTask(task);

			from = to;
			if (from >= toExclusive) {
				break;
			}
		}

		// タスクが終了するのを待つ.
		for (IZ_INT i = 0; i < threadCount; i++) {
			if (tasks[i] == IZ_NULL) {
				break;
			}
			tasks[i]->Wait();
		}
    }

    ////////////////////////////////////////////////////////

	// foreach処理用タスククラス
    class CParallelForEach : public CTask
    {
    public:
		static CTask* Create(
            IMemoryAllocator* allocator,
            IZ_UINT8* p, size_t stride, 
			IZ_UINT from, IZ_UINT to,
			std::function<void(void*)> func)
        {
			void* buf = ALLOC(allocator, sizeof(CParallelForEach));
            VRETURN_NULL(buf != IZ_NULL);

			CParallelForEach* ret = new(buf) CParallelForEach(p, stride, from, to, func);
            return ret;
        }

    protected:
        CParallelForEach(
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

        virtual ~CParallelForEach() {}

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

	// 指定された回数だけ処理を実行する.
    void CParallel::ForEach(
		CThreadPool& threadPool,
        IMemoryAllocator* allocator,
        void* data, size_t stride,
        IZ_UINT count,
		std::function<void(void*)> func)
    {
		// 作成したタスク保持用.
		CTask* tasks[10] = { IZ_NULL };

		// 利用されるスレッド数.
        IZ_UINT threadCount = sys::CEnvironment::GetProcessorNum();

		// スレッドプールに設定されている回数と調整する.
		threadCount = ((IZ_INT)threadPool.GetMaxThreadNum() > threadCount
			? threadCount
			: threadPool.GetMaxThreadNum());

        if (threadCount > COUNTOF(tasks)) {
            threadCount = COUNTOF(tasks);
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

            // 実行インスタンスを作成
			CTask* task = CParallelForEach::Create(
                allocator,
                ptr, stride, from, to,
                func);

            IZ_ASSERT(task != IZ_NULL);

            SetAllocator(task, allocator);
            tasks[i] = task;

			// スレッドプールに登録.
			threadPool.EneueueTask(task);

            from = to;
            if (from >= count) {
                break;
            }
        }

		// タスクが終了するのを待つ.
        for (IZ_UINT i = 0; i < threadCount; i++) {
            if (tasks[i] == IZ_NULL) {
                break;
            }

            tasks[i]->Wait();
        }
    }
}   // namespace threadmodel
}   // namespace izanagi
