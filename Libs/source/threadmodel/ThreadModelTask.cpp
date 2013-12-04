#include "threadmodel/ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
    void CTask::DeleteTask(CTask* task)
    {
        delete task;
        FREE(task->m_Allocator, task);
    }

    CTask::CTask()
    {
        m_Allocator = IZ_NULL;
        m_Event.Open();

        m_ListItem.Init(this);
    }

    CTask::~CTask()
    {
        m_Event.Close();
    }

    void CTask::Wait()
    {
        m_Event.Wait();
    }

    void CTask::Run(void* userData)
    {
        OnRun();
        m_Event.Set();
    }
}   // namespace threadmodel
}   // namespace izanagi
