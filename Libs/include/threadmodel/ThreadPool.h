#if !defined(__IZANAGI_THREAD_MODEL_THREAD_POOL_H__)
#define __IZANAGI_THREAD_MODEL_THREAD_POOL_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    class CJobWorker;
    class CTask;

    class CThreadPool
    {
    private:
        CThreadPool() {}
        ~CThreadPool() {}

        NO_COPIABLE(CThreadPool);

    public:
        /** 初期化.
         */
        static IZ_BOOL Init(
            IMemoryAllocator* allocator,
            IZ_UINT threadNum);

        /** タスクをキューに積む.
         */
        static IZ_BOOL Enqueue(CTask* task, IZ_BOOL deleteTaskWhenFinish = IZ_FALSE);

        /**
         */
        static void WaitEmpty();

        /** ThreadPoolの終了.
         */
        static void Terminate();

    private:
        // 空いているワーカーを取得.
        CJobWorker* GetWaitingJobWorker();

    private:
        static IMemoryAllocator* s_Allocator;

        static IZ_UINT s_WorkerNum;
        static CJobWorker** s_Workers;

        static IZ_BOOL s_IsTerminated;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_THREAD_POOL_H__)
