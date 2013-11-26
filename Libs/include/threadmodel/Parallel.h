#if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
#define __IZANAGI_THREAD_MODEL_PARALLEL_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CParallel
    {
    private:
        CParallel();
        ~CParallel();

    public:
        class CFuncFor {
        public:
            CFuncFor() {}
            virtual ~CFuncFor() {}

            virtual void operator()(IZ_INT) = 0;
        };

        class CFuncForEach {
        public:
            CFuncForEach() {}
            virtual ~CFuncForEach() {}

            virtual void operator()(void*) = 0;
        };

    public:
        static void For(
            IMemoryAllocator* allocator,
            IZ_INT fromInclusive, IZ_INT toExclusive, 
            ActionDelegate<IZ_INT>& action);

        static void For(
            IMemoryAllocator* allocator,
            IZ_INT fromInclusive, IZ_INT toExclusive, 
            void (*func)(IZ_INT));

        static void For(
            IMemoryAllocator* allocator,
            IZ_INT fromInclusive, IZ_INT toExclusive, 
            CFuncFor& func);

        static void ForEach(
            IMemoryAllocator* allocator,
            void* data, size_t stride,
            IZ_UINT count,
            ActionDelegate<void*>& action);

        static void ForEach(
            IMemoryAllocator* allocator,
            void* data, size_t stride,
            IZ_UINT count,
            void (*func)(void*));

        static void ForEach(
            IMemoryAllocator* allocator,
            void* data, size_t stride,
            IZ_UINT count,
            CFuncForEach& func);

    private:
        template <typename _T, typename _CALLBACK>
        static void For(
            IMemoryAllocator* allocator,
            IZ_INT fromInclusive, IZ_INT toExclusive, 
            _CALLBACK callback);

        template <typename _T, typename _CALLBACK>
        static void ForEach(
            IMemoryAllocator* allocator,
            void* data, size_t stride,
            IZ_UINT count,
            _CALLBACK callback);
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
