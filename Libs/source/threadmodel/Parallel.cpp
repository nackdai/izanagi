#include "threadmodel/Parallel.h"
#include "threadmodel/ThreadPool.h"
#include "threadmodel/ThreadModelTask.h"
#include "threadmodel/ThreadModelTaskScheduler.h"

namespace izanagi
{
namespace threadmodel
{
    void CParallel::SetAllocator(CTask* task, IMemoryAllocator* allocator)
    {
        task->SetAllocator(allocator);
    }

    static const IZ_INT PARALLEL_CHUNK_SIZE = 4;

    class CParallelFor : public CTask
    {
    public:
        template <typename _T, typename _CALLBACK>
        static _T* Create(
            IMemoryAllocator* allocator,
            IZ_INT from, IZ_INT to,
            _CALLBACK action)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T(from, to, action);
            return ret;
        }

    protected:
        CParallelFor(IZ_INT from, IZ_INT to)
        {
            m_From = from;
            m_To = to;
        }
        virtual ~CParallelFor() {}

    public:
        virtual void OnRun()
        {
            for (IZ_INT i = m_From; i < m_To; i++) {
                RunInternal(i);
            }
        }

        virtual void RunInternal(IZ_INT idx) = 0;

    protected:
        IZ_INT m_From;
        IZ_INT m_To;
    };

    // Actionデリゲートで処理を実行する
    class CParallelForAction : public CParallelFor
    {
    public:
        static CParallelForAction* Create(
            IMemoryAllocator* allocator,
            IZ_INT from, IZ_INT to,
            ActionDelegate<IZ_INT>& action)
        {
            CParallelForAction* ret = CParallelFor::Create<CParallelForAction, ActionDelegate<IZ_INT>&>(
                allocator,
                from, to,
                action);
            return ret;
        }

    public:
        CParallelForAction(
            IZ_INT from, IZ_INT to,
            ActionDelegate<IZ_INT>& action)
            : CParallelFor(from, to), m_Action(action)
        {
        }
        virtual ~CParallelForAction() {}

        virtual void RunInternal(IZ_INT idx)
        {
            m_Action(idx);
        }

    private:
        ActionDelegate<IZ_INT>& m_Action;
    };

    // 関数ポインタで処理を実行する
    class CParallelForFunc : public CParallelFor
    {
    public:
        static CParallelForFunc* Create(
            IMemoryAllocator* allocator,
            IZ_INT from, IZ_INT to,
            void (*func)(IZ_INT))
        {
            CParallelForFunc* ret = CParallelFor::Create<CParallelForFunc, void(*)(IZ_INT)>(
                allocator,
                from, to,
                func);
            return ret;
        }

    public:
        CParallelForFunc(
            IZ_INT from, IZ_INT to,
            void (*func)(IZ_INT))
            : CParallelFor(from, to), m_Func(func)
        {
        }
        virtual ~CParallelForFunc() {}

        virtual void RunInternal(IZ_INT idx)
        {
            (*m_Func)(idx);
        }

    private:
        void (*m_Func)(IZ_INT);
    };

    // 関数オブジェクトで処理を実行する
    class CParallelForFunctor : public CParallelFor
    {
    public:
        static CParallelForFunctor* Create(
            IMemoryAllocator* allocator,
            IZ_INT from, IZ_INT to,
            CParallel::CFuncFor& func)
        {
            CParallelForFunctor* ret = CParallelFor::Create<CParallelForFunctor, CParallel::CFuncFor&>(
                allocator,
                from, to,
                func);
            return ret;
        }

    public:
        CParallelForFunctor(
            IZ_INT from, IZ_INT to,
            CParallel::CFuncFor& func)
            : CParallelFor(from, to), m_Func(func)
        {
        }
        virtual ~CParallelForFunctor() {}

        virtual void RunInternal(IZ_INT idx)
        {
            m_Func(idx);
        }

    private:
        CParallel::CFuncFor& m_Func;
    };

    template <typename _T, typename _CALLBACK>
    void CParallel::For(
        IMemoryAllocator* allocator,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        _CALLBACK callback)
    {
        if (fromInclusive > toExclusive) {
            IZ_INT tmp = toExclusive;
            toExclusive = fromInclusive;
            fromInclusive = tmp;
        }

        _T* tasks[10] = { IZ_NULL };

        IZ_INT threadCount = sys::CEnvironment::GetProcessorNum();
        threadCount = ((IZ_INT)CThreadPool::GetMaxThreadNum() > threadCount ? threadCount : CThreadPool::GetMaxThreadNum());

        if (threadCount > COUNTOF(tasks)) {
            threadCount = COUNTOF(tasks);
        }

        IZ_INT step = (toExclusive - fromInclusive) / threadCount;

        if (step < 5) {
            step = 5;
            threadCount = (toExclusive - fromInclusive) / step;
            if (threadCount < 1) {
                threadCount = 1;
            }
        }

        while (step * threadCount < toExclusive) {
            step++;
        }
        
        IZ_INT from = fromInclusive;

        CTaskScheduler scheduler;

        for (IZ_INT i = 0; i < threadCount; i++) {
            IZ_INT to = from + step;
            if (to >= toExclusive) {
                to = toExclusive;
            }

            // 実行インスタンスを作成
            _T* task = _T::Create(
                allocator,
                from, to,
                callback);

            IZ_ASSERT(task != IZ_NULL);

            SetAllocator(task, allocator);
            tasks[i] = task;

            scheduler.Enqueue(*task);

            from = to;
            if (from >= toExclusive) {
                break;
            }
        }

        for (IZ_INT i = 0; i < threadCount; i++) {
            if (tasks[i] == IZ_NULL) {
                break;
            }
            tasks[i]->Wait();
            CTask::DeleteTask(tasks[i]);
        }
    }

    void CParallel::For(
        IMemoryAllocator* allocator,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        ActionDelegate<IZ_INT>& action)
    {
        For<CParallelForAction, ActionDelegate<IZ_INT>&>(
            allocator,
            fromInclusive, toExclusive,
            action);
    }

    void CParallel::For(
        IMemoryAllocator* allocator,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        void (*func)(IZ_INT))
    {
        For<CParallelForFunc, void(*)(IZ_INT)>(
            allocator,
            fromInclusive, toExclusive,
            func);
    }

    void CParallel::For(
        IMemoryAllocator* allocator,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        CFuncFor& func)
    {
        For<CParallelForFunctor, CFuncFor&>(
            allocator,
            fromInclusive, toExclusive,
            func);
    }

    ////////////////////////////////////////////////////////

    class CParallelForEach : public CTask
    {
    public:
        template <typename _T, typename _CALLBACK>
        static _T* Create(
            IMemoryAllocator* allocator,
            IZ_UINT8* p, size_t stride, IZ_UINT from, IZ_UINT to,
            _CALLBACK action)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T(p, stride, from, to, action);
            return ret;
        }

    protected:
        CParallelForEach(IZ_UINT8* p, size_t stride, IZ_UINT from, IZ_UINT to)
        {
            m_Ptr = p;
            m_Stride = stride;
            m_From = from;
            m_To = to;
        }

        virtual ~CParallelForEach() {}

        virtual void OnRun()
        {
            if (m_Ptr != IZ_NULL) {
                IZ_UINT8* ptr = m_Ptr;

                for (IZ_UINT i = m_From; i < m_To; i++) {
                    IZ_UINT8* p = ptr + i * m_Stride;
                    RunInternal(p);
                }
            }
        }

        virtual void RunInternal(void* data) = 0;

    private:
        IZ_UINT8* m_Ptr;
        size_t m_Stride;
        IZ_UINT m_From;
        IZ_UINT m_To;
    };

    // Actionデリゲートで処理を実行
    class CParallelForEachAction : public CParallelForEach
    {
    public:
        static CParallelForEachAction* Create(
            IMemoryAllocator* allocator,
            IZ_UINT8* p, size_t stride, IZ_UINT from, IZ_UINT to,
            ActionDelegate<void*>& action)
        {
            CParallelForEachAction* ret = CParallelForEach::Create<CParallelForEachAction, ActionDelegate<void*>&>(
                allocator,
                p, stride, from, to,
                action);
            return ret;
        }

    public:
        CParallelForEachAction(
            IZ_UINT8* p, size_t stride, IZ_UINT from, IZ_UINT to,
            ActionDelegate<void*>& action)
            : CParallelForEach(p, stride, from, to), m_Action(action)
        {
        }

        virtual ~CParallelForEachAction() {}

    public:
        virtual void RunInternal(void* data)
        {
            m_Action(data);
        }

    private:
        ActionDelegate<void*>& m_Action;
    };

    // 関数ポインタで処理を実行
    class CParallelForEachFunc : public CParallelForEach
    {
    public:
        static CParallelForEachFunc* Create(
            IMemoryAllocator* allocator,
            IZ_UINT8* p, size_t stride, IZ_UINT from, IZ_UINT to,
            void (*func)(void*))
        {
            CParallelForEachFunc* ret = CParallelForEach::Create<CParallelForEachFunc, void (*)(void*)>(
                allocator,
                p, stride, from, to,
                func);
            return ret;
        }

    public:
        CParallelForEachFunc(
            IZ_UINT8* p, size_t stride, IZ_UINT from, IZ_UINT to,
            void (*func)(void*))
            : CParallelForEach(p, stride, from, to), m_Func(func)
        {
        }

        virtual ~CParallelForEachFunc() {}

    public:
        virtual void RunInternal(void* data)
        {
            (*m_Func)(data);
        }

    private:
        void (*m_Func)(void*);
    };

    // 関数オブジェクトで処理を実行
    class CParallelForEachFunctor : public CParallelForEach
    {
    public:
        static CParallelForEachFunctor* Create(
            IMemoryAllocator* allocator,
            IZ_UINT8* p, size_t stride, IZ_UINT from, IZ_UINT to,
            CParallel::CFuncForEach& func)
        {
            CParallelForEachFunctor* ret = CParallelForEach::Create<CParallelForEachFunctor, CParallel::CFuncForEach&>(
                allocator,
                p, stride, from, to,
                func);
            return ret;
        }

    public:
        CParallelForEachFunctor(
            IZ_UINT8* p, size_t stride, IZ_UINT from, IZ_UINT to,
            CParallel::CFuncForEach& func)
            : CParallelForEach(p, stride, from, to), m_Func(func)
        {
        }

        virtual ~CParallelForEachFunctor() {}

    public:
        virtual void RunInternal(void* data)
        {
            m_Func(data);
        }

    private:
        CParallel::CFuncForEach& m_Func;
    };

    template <typename _T, typename _CALLBACK>
    void CParallel::ForEach(
        IMemoryAllocator* allocator,
        void* data, size_t stride,
        IZ_UINT count,
        _CALLBACK callback)
    {
        IZ_UINT threadCount = sys::CEnvironment::GetProcessorNum();
        threadCount = ((IZ_INT)CThreadPool::GetMaxThreadNum() > threadCount ? threadCount : CThreadPool::GetMaxThreadNum());

        _T* tasks[10] = { IZ_NULL };

        if (threadCount > COUNTOF(tasks)) {
            threadCount = COUNTOF(tasks);
        }

        IZ_UINT step = count / threadCount;
        
        if (step < 5) {
            step = 5;
            if (step > count) {
                step = count;
            }

            threadCount = count / step;
            if (threadCount < 1) {
                threadCount = 1;
            }
        }

        while (step * threadCount < count) {
            step++;
        }

        IZ_UINT8* ptr = (IZ_UINT8*)data;
        IZ_UINT from = 0;

        CTaskScheduler scheduler;

        for (IZ_UINT i = 0; i < threadCount; i++) {
            IZ_UINT to = from + step;
            if (to >= count) {
                to = count;
            }

            // 実行インスタンスを作成
            _T* task = _T::Create(
                allocator,
                ptr, stride, from, to,
                callback);

            IZ_ASSERT(task != IZ_NULL);

            SetAllocator(task, allocator);
            tasks[i] = task;

            scheduler.Enqueue(*task);

            from = to;
            if (from >= count) {
                break;
            }
        }

        for (IZ_UINT i = 0; i < threadCount; i++) {
            if (tasks[i] == IZ_NULL) {
                break;
            }

            tasks[i]->Wait();
            CTask::DeleteTask(tasks[i]);
        }
    }

    void CParallel::ForEach(
        IMemoryAllocator* allocator,
        void* data, size_t stride,
        IZ_UINT count,
        ActionDelegate<void*>& action)
    {
        ForEach<CParallelForEachAction, ActionDelegate<void*>&>(
            allocator,
            data, stride,
            count,
            action);
    }

    void CParallel::ForEach(
        IMemoryAllocator* allocator,
        void* data, size_t stride,
        IZ_UINT count,
        void (*func)(void*))
    {
        ForEach<CParallelForEachFunc, void(*)(void*)>(
            allocator,
            data, stride,
            count,
            func);
    }

    void CParallel::ForEach(
        IMemoryAllocator* allocator,
        void* data, size_t stride,
        IZ_UINT count,
        CParallel::CFuncForEach& func)
    {
        ForEach<CParallelForEachFunctor, CParallel::CFuncForEach&>(
            allocator,
            data, stride,
            count,
            func);
    }
}   // namespace threadmodel
}   // namespace izanagi
