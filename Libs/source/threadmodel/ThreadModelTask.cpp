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

        m_StateLocker.Open();
        m_State = State_Init;
    }

    CTask::~CTask()
    {
        m_Event.Close();
        m_StateLocker.Close();
    }

    void CTask::Wait()
    {
        m_Event.Wait();
    }

    IZ_BOOL CTask::Cancel()
    {
        sys::CGuarder guard(m_StateLocker);
        
        if (m_State != State_Canceled
            && m_State != State_Running
            && m_State != State_Finished)
        {
            m_State = State_WillCancel;
            return IZ_TRUE;
        }

        return IZ_FALSE;
    }

    IZ_BOOL CTask::WillCancel()
    {
        sys::CGuarder guard(m_StateLocker);
        return (m_State == State_WillCancel);
    }

    IZ_BOOL CTask::IsCanceled()
    {
        sys::CGuarder guard(m_StateLocker);
        return (m_State == State_Canceled);
    }

    IZ_BOOL CTask::IsFinished()
    {
        sys::CGuarder guard(m_StateLocker);
        return (m_State == State_Finished);
    }

    void CTask::Run(void* userData)
    {
        sys::CGuarder guard(m_StateLocker);

        if (m_State == State_WillCancel) {
            m_State = State_Canceled;
        }
        else {
            m_State = State_Running;
            OnRun();
            m_State = State_Finished;
        }

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
