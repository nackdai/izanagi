#include "threadmodel/Parallel.h"
#include "threadmodel/ThreadPool.h"

namespace izanagi
{
namespace threadmodel
{
    static const IZ_INT PARALLEL_CHUNK_SIZE = 4;

    class CIndexProxy
    {
    public:
        CIndexProxy(IZ_INT from, IZ_INT to)
        {
            m_Mutex.Open();
            m_From = from;
            m_To = to;

            m_Current = from;
        }

        ~CIndexProxy()
        {
            m_Mutex.Close();
        }

    public:
        IZ_BOOL GetIndex(IZ_INT& from, IZ_INT& to)
        {
            sys::CGuarder guarder(m_Mutex);

            if (m_Current >= m_To) {
                return IZ_FALSE;
            }

            from = m_Current;
            to = m_Current + PARALLEL_CHUNK_SIZE;
            to = (to >= m_To ? m_To : to);

            m_Current += PARALLEL_CHUNK_SIZE;

            return IZ_TRUE;
        }

    private:
        sys::CMutex m_Mutex;

        IZ_INT m_From;
        IZ_INT m_To;

        IZ_INT m_Current;
    };

    class CParallelFor : public sys::IRunnable, public CPlacementNew
    {
    public:
        template <typename _T, typename _CALLBACK>
        static _T* Create(
            IMemoryAllocator* allocator,
            CIndexProxy& proxy, 
            _CALLBACK action)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T(proxy, action);
            return ret;
        }

    protected:
        CParallelFor(CIndexProxy& proxy)
            : m_Proxy(proxy)
        {
        }
        virtual ~CParallelFor();

    public:
        virtual void Run(void* data)
        {
            IZ_INT from = 0;
            IZ_INT to = 0;

            if (m_Proxy.GetIndex(from, to)) {
                for (IZ_INT i = from; i < to; i++) {
                    RunInternal(i);
                }
            }
        }

        virtual void RunInternal(IZ_INT idx);

    protected:
        CIndexProxy& m_Proxy;
    };

    class CParallelForAction : public CParallelFor
    {
    public:
        static CParallelForAction* Create(
            IMemoryAllocator* allocator,
            CIndexProxy& proxy, 
            ActionDelegate<IZ_INT>& action)
        {
            CParallelForAction* ret = CParallelFor::Create<CParallelForAction, ActionDelegate<IZ_INT>&>(
                allocator,
                proxy,
                action);
            return ret;
        }

    public:
        CParallelForAction(
            CIndexProxy& proxy, 
            ActionDelegate<IZ_INT>& action)
            : CParallelFor(proxy), m_Action(action)
        {
        }
        virtual ~CParallelForAction();

        virtual void RunInternal(IZ_INT idx)
        {
            m_Action(idx);
        }

    private:
        ActionDelegate<IZ_INT>& m_Action;
    };

    class CParallelForFunc : public CParallelFor
    {
    public:
        static CParallelForFunc* Create(
            IMemoryAllocator* allocator,
            CIndexProxy& proxy, 
            void (*func)(IZ_INT))
        {
            CParallelForFunc* ret = CParallelFor::Create<CParallelForFunc, void(*)(IZ_INT)>(
                allocator,
                proxy,
                func);
            return ret;
        }

    public:
        CParallelForFunc(
            CIndexProxy& proxy, 
            void (*func)(IZ_INT))
            : CParallelFor(proxy), m_Func(func)
        {
        }
        virtual ~CParallelForFunc();

        virtual void RunInternal(IZ_INT idx)
        {
            (*m_Func)(idx);
        }

    private:
        void (*m_Func)(IZ_INT);
    };

    template <typename _T, typename _CALLBACK>
    void CParallel::For(
        IMemoryAllocator* allocator,
        IZ_INT fromInclusive, IZ_INT toExclusive, 
        _CALLBACK callback)
    {
        IZ_UINT threadCount = sys::CEnvironment::GetProcessorNum();

        // TODO
        CThreadPool::CThread* threadArray[10] = { IZ_NULL };

        CIndexProxy proxy(fromInclusive, toExclusive);

        for (IZ_UINT i = 0; i < threadCount; i++) { 
            _T* runnable = _T::Create(
                allocator,
                proxy,
                callback);

            IZ_ASSERT(runnable != IZ_NULL);

            // TODO
            CThreadPool::CThread* thread = CThreadPool::GetThread(runnable);
            IZ_ASSERT(thread != IZ_NULL);

            thread->Start();
            threadArray[i] = thread;
        }

        for (IZ_UINT i = 0; i < threadCount; i++) {
            if (threadArray[i] == IZ_NULL) {
                break;
            }

            threadArray[i]->Join();
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

    class CDataProxy
    {
    public:
        CDataProxy(void* data, size_t stride, IZ_UINT count)
        {
            m_Mutex.Open();
            m_Data = (IZ_UINT8*)data;
            m_Stride = stride;
            m_Count = count;
            m_Current = 0;
        }
        ~CDataProxy()
        {
            m_Mutex.Close();
        }

    public:
        IZ_UINT8* GetData(IZ_UINT count)
        {
            sys::CGuarder guarder(m_Mutex);

            if (m_Current >= m_Count) {
                return IZ_NULL;
            }

            IZ_UINT8* ret = m_Data + m_Stride * m_Current;
            count = PARALLEL_CHUNK_SIZE;

            if (m_Current + PARALLEL_CHUNK_SIZE > m_Count) {
                count = m_Count - m_Current;
                m_Current = m_Count;
            }
            else {
                m_Current += PARALLEL_CHUNK_SIZE;
            }

            return ret;
        }

        size_t GetStride() { return m_Stride; }

    private:
        sys::CMutex m_Mutex;

        IZ_UINT8* m_Data;
        size_t m_Stride;

        IZ_UINT m_Count;
        IZ_UINT m_Current; 
    };

    class CParallelForEach : public sys::IRunnable, public CPlacementNew
    {
    public:
        template <typename _T, typename _CALLBACK>
        static _T* Create(
            IMemoryAllocator* allocator,
            CDataProxy& proxy, 
            _CALLBACK action)
        {
            void* buf = ALLOC(allocator, sizeof(_T));
            VRETURN_NULL(buf != IZ_NULL);

            _T* ret = new(buf) _T(proxy, action);
            return ret;
        }

    protected:
        CParallelForEach(CDataProxy& proxy)
            : m_Proxy(proxy)
        {
        }

        virtual ~CParallelForEach() {}

        virtual void Run(void* data)
        {
            IZ_UINT count = 0;
            IZ_UINT8* p = m_Proxy.GetData(count);

            if (p != IZ_NULL) {
                size_t stride = m_Proxy.GetStride();

                for (IZ_UINT i = 0; i < count; i++) {
                    RunInternal(p);
                    p += stride;
                }
            }
        }

        virtual void RunInternal(void* data) = 0;

    private:
        CDataProxy& m_Proxy;
    };

    class CParallelForEachAction : public CParallelForEach
    {
    public:
        static CParallelForEachAction* Create(
            IMemoryAllocator* allocator,
            CDataProxy& proxy, 
            ActionDelegate<void*>& action)
        {
            CParallelForEachAction* ret = CParallelForEach::Create<CParallelForEachAction, ActionDelegate<void*>&>(
                allocator,
                proxy,
                action);
            return ret;
        }

    public:
        CParallelForEachAction(
            CDataProxy& proxy, 
            ActionDelegate<void*>& action)
            : CParallelForEach(proxy), m_Action(action)
        {
        }

        virtual ~CParallelForEachAction();

    public:
        virtual void RunInternal(void* data)
        {
            m_Action(data);
        }

    private:
        ActionDelegate<void*>& m_Action;
    };

    class CParallelForEachFunc : public CParallelForEach
    {
    public:
        static CParallelForEachFunc* Create(
            IMemoryAllocator* allocator,
            CDataProxy& proxy, 
            void (*func)(void*))
        {
            CParallelForEachFunc* ret = CParallelForEach::Create<CParallelForEachFunc, void (*)(void*)>(
                allocator,
                proxy,
                func);
            return ret;
        }

    public:
        CParallelForEachFunc(
            CDataProxy& proxy, 
            void (*func)(void*))
            : CParallelForEach(proxy), m_Func(func)
        {
        }

        virtual ~CParallelForEachFunc();

    public:
        virtual void RunInternal(void* data)
        {
            (*m_Func)(data);
        }

    private:
        void (*m_Func)(void*);
    };

    template <typename _T, typename _CALLBACK>
    void CParallel::ForEach(
        IMemoryAllocator* allocator,
        void* data, size_t stride,
        IZ_UINT count,
        _CALLBACK callback)
    {
        IZ_UINT threadCount = sys::CEnvironment::GetProcessorNum();

        // TODO
        CThreadPool::CThread* threadArray[10] = { IZ_NULL };

        CDataProxy proxy(data, stride, count);

        for (IZ_UINT i = 0; i < threadCount; i++) {
            _T* runnable = _T::Create(
                allocator,
                proxy,
                callback);

            IZ_ASSERT(runnable != IZ_NULL);

            // TODO
            CThreadPool::CThread* thread = CThreadPool::GetThread(runnable);
            IZ_ASSERT(thread != IZ_NULL);

            thread->Start();
            threadArray[i] = thread;
        }

        for (IZ_UINT i = 0; i < threadCount; i++) {
            if (threadArray[i] == IZ_NULL) {
                break;
            }

            threadArray[i]->Join();
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
}   // namespace threadmodel
}   // namespace izanagi
