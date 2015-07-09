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

	// for���[�v�����p�^�X�N�N���X.
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

	// �w�肵���͈͂ŏ��������s����.
    void CParallel::For(
		CThreadPool& threadPool,
        IMemoryAllocator* allocator,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        std::function<void(IZ_INT)> func)
    {
		// from > to �ɂȂ�悤�ɂ���
		if (fromInclusive > toExclusive) {
			IZ_INT tmp = toExclusive;
			toExclusive = fromInclusive;
			fromInclusive = tmp;
		}

		// �쐬�����^�X�N�ێ��p.
		CTask* tasks[10] = { IZ_NULL };

		// ���p�����X���b�h��.
		IZ_INT threadCount = sys::CEnvironment::GetProcessorNum();

		// �X���b�h�v�[���ɐݒ肳��Ă���񐔂ƒ�������.
		threadCount = ((IZ_INT)threadPool.GetMaxThreadNum() > threadCount
			? threadCount
			: threadPool.GetMaxThreadNum());

		if (threadCount > COUNTOF(tasks)) {
			threadCount = COUNTOF(tasks);
		}

		// �P�X���b�h������̃��[�v��.
		IZ_INT step = (toExclusive - fromInclusive) / threadCount;

		// TODO
		// ���[�v�񐔂����ȉ��̏ꍇ�͒�������.
		// �񐔂͓K��.
		if (step < 5) {
			step = 5;

			// ���p�����X���b�h���𒲐�.
			threadCount = (toExclusive - fromInclusive) / step;
			if (threadCount < 1) {
				threadCount = 1;
			}
		}

		// to�̉񐔂������Ȃ��悤�ɒ���.
		while (step * threadCount < toExclusive) {
			step++;
		}

		IZ_INT from = fromInclusive;

		for (IZ_INT i = 0; i < threadCount; i++) {
			IZ_INT to = from + step;
			if (to >= toExclusive) {
				to = toExclusive;
			}

			// ���s�C���X�^���X���쐬
			CTask* task = CParallelFor::Create(
				allocator,
				from, to,
				func);

			IZ_ASSERT(task != IZ_NULL);

			SetAllocator(task, allocator);
			tasks[i] = task;

			// �X���b�h�v�[���ɓo�^.
			threadPool.EneueueTask(task);

			from = to;
			if (from >= toExclusive) {
				break;
			}
		}

		// �^�X�N���I������̂�҂�.
		for (IZ_INT i = 0; i < threadCount; i++) {
			if (tasks[i] == IZ_NULL) {
				break;
			}
			tasks[i]->Wait();
		}
    }

    ////////////////////////////////////////////////////////

	// foreach�����p�^�X�N�N���X
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

	// �w�肳�ꂽ�񐔂������������s����.
    void CParallel::ForEach(
		CThreadPool& threadPool,
        IMemoryAllocator* allocator,
        void* data, size_t stride,
        IZ_UINT count,
		std::function<void(void*)> func)
    {
		// �쐬�����^�X�N�ێ��p.
		CTask* tasks[10] = { IZ_NULL };

		// ���p�����X���b�h��.
        IZ_UINT threadCount = sys::CEnvironment::GetProcessorNum();

		// �X���b�h�v�[���ɐݒ肳��Ă���񐔂ƒ�������.
		threadCount = ((IZ_INT)threadPool.GetMaxThreadNum() > threadCount
			? threadCount
			: threadPool.GetMaxThreadNum());

        if (threadCount > COUNTOF(tasks)) {
            threadCount = COUNTOF(tasks);
        }

		// �P���[�v������̏�����.
        IZ_UINT step = count / threadCount;
        
		// TODO
		// �����񐔂����ȉ��̏ꍇ�͒�������.
		// �񐔂͓K��.
        if (step < 5) {
            step = 5;

			// �w�肳�ꂽ�����񐔂͒����Ȃ��悤�ɂ���.
            if (step > count) {
                step = count;
            }

			// ���p�����X���b�h���𒲐�.
            threadCount = count / step;
            if (threadCount < 1) {
                threadCount = 1;
            }
        }

		// �w�肳�ꂽ�����񐔂������Ȃ��悤�ɂ���.
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

            // ���s�C���X�^���X���쐬
			CTask* task = CParallelForEach::Create(
                allocator,
                ptr, stride, from, to,
                func);

            IZ_ASSERT(task != IZ_NULL);

            SetAllocator(task, allocator);
            tasks[i] = task;

			// �X���b�h�v�[���ɓo�^.
			threadPool.EneueueTask(task);

            from = to;
            if (from >= count) {
                break;
            }
        }

		// �^�X�N���I������̂�҂�.
        for (IZ_UINT i = 0; i < threadCount; i++) {
            if (tasks[i] == IZ_NULL) {
                break;
            }

            tasks[i]->Wait();
        }
    }
}   // namespace threadmodel
}   // namespace izanagi
