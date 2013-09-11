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
        void For(
            IMemoryAllocator* allocator,
            IZ_INT fromInclusive, IZ_INT toExclusive, 
            ActionDelegate<IZ_INT>& action);

        void For(
            IMemoryAllocator* allocator,
            IZ_INT fromInclusive, IZ_INT toExclusive, 
            void (*func)(IZ_INT));

        void ForEach(
            IMemoryAllocator* allocator,
            void* data, size_t stride,
            IZ_UINT count,
            ActionDelegate<void*>& action);

        void ForEach(
            IMemoryAllocator* allocator,
            void* data, size_t stride,
            IZ_UINT count,
            void (*func)(void*));

    private:
        template <typename _T, typename _CALLBACK>
        void For(
            IMemoryAllocator* allocator,
            IZ_INT fromInclusive, IZ_INT toExclusive, 
            _CALLBACK callback);

        template <typename _T, typename _CALLBACK>
        void ForEach(
            IMemoryAllocator* allocator,
            void* data, size_t stride,
            IZ_UINT count,
            _CALLBACK callback);
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_PARALLEL_H__)
