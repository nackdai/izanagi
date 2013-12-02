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
        /** ������.
         */
        static IZ_BOOL Init(
            IMemoryAllocator* allocator,
            IZ_UINT threadNum);

        /** �^�X�N���L���[�ɐς�.
         */
        static IZ_BOOL Enqueue(CTask* task, IZ_BOOL deleteTaskWhenFinish = IZ_FALSE);

        /**
         */
        static void WaitEmpty();

        /** ThreadPool�̏I��.
         */
        static void Terminate();

    private:
        // �󂢂Ă��郏�[�J�[���擾.
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
