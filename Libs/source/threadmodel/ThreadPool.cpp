#include "threadmodel/ThreadPool.h"
#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
    IMemoryAllocator* CThreadPool::s_Allocator = IZ_NULL;
    
    IZ_UINT CThreadPool::s_WorkerNum = 0;
    CJobWorker** CThreadPool::s_Workers = IZ_NULL;
        
    IZ_BOOL CThreadPool::s_IsTerminated = IZ_FALSE;

    IZ_BOOL CThreadPool::Init(
        IMemoryAllocator* allocator,
        IZ_UINT threadNum)
    {
        VRETURN(!s_IsTerminated);

        if (s_Workers != IZ_NULL) {
            return IZ_TRUE;
        }

        void* buf = ALLOC(
            allocator, 
            sizeof(CJobWorker*) * threadNum + sizeof(CJobWorker) * threadNum);
        VRETURN(buf != IZ_NULL);

        IZ_UINT8* p = (IZ_UINT8*)buf;

        s_Workers = reinterpret_cast<CJobWorker**>(p);
        p += sizeof(CJobWorker*) * threadNum; 

        for (IZ_UINT i = 0; i < threadNum; i++) {
            s_Workers[i] = new(p) CJobWorker();
            p += sizeof(CJobWorker);

            s_Workers[i]->Start();
        }

        s_WorkerNum = threadNum;
        s_Allocator = allocator;

        return IZ_TRUE;
    }

    IZ_BOOL CThreadPool::Enqueue(CTask* task, IZ_BOOL deleteTaskWhenFinish/*= IZ_FALSE*/)
    {
        for (IZ_UINT i = 0; i < s_WorkerNum; i++) {
            if (s_Workers[i]->IsWaiting()) {
                // Taskを待機状態にセット
                task->SetState(CJob::State_Waiting);

                if (deleteTaskWhenFinish) {
                    task->SetEnableDeleteWhenFinish();
                }

                s_Workers[i]->Register(task);
                return IZ_TRUE;
            }
        }
        return IZ_FALSE;
    }

    void CThreadPool::Terminate()
    {
        if (s_IsTerminated) {
            return;
        }

        // ワーカーをJoinする
        for (IZ_UINT i = 0; i < s_WorkerNum; i++) {
            s_Workers[i]->Join();
            delete s_Workers[i];
        }

        FREE(s_Allocator, s_Workers);
        s_Workers = IZ_NULL;
        s_WorkerNum = 0;

        s_IsTerminated = IZ_TRUE;
    }
}   // namespace threadmodel
}   // namespace izanagi
