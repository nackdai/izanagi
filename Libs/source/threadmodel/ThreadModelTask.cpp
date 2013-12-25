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

        m_IsDeleteSelf = IZ_FALSE;
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

    CStdList<CTask>::Item* CTask::GetListItem()
    {
        return &m_ListItem;
    }

    void CTask::SetAllocator(IMemoryAllocator* allocator)
    {
        m_Allocator = allocator;
    }

    void CTask::SetIsDeleteSelf(IZ_BOOL isDeleteSelf)
    {
        m_IsDeleteSelf = isDeleteSelf;
    }

    IZ_BOOL CTask::IsDeleteSelf()
    {
        return m_IsDeleteSelf;
    }
}   // namespace threadmodel
}   // namespace izanagi
