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

        m_ListItem.Init(this);

        m_IsDeleteSelf = IZ_FALSE;

        m_State = State_Init;
    }

    CTask::~CTask()
    {
    }

    void CTask::Wait()
    {
		std::unique_lock<std::mutex> lock(m_mutex);
		m_condVar.wait(
			lock,
			[this]{
			return (m_State = State_Canceled) || (m_State == State_Finished);
		});
    }

    IZ_BOOL CTask::Cancel()
    {
		std::unique_lock<std::mutex> lock(m_mutex);
        
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
		std::unique_lock<std::mutex> lock(m_mutex);
        return (m_State == State_WillCancel);
    }

    IZ_BOOL CTask::IsCanceled()
    {
		std::unique_lock<std::mutex> lock(m_mutex);
        return (m_State == State_Canceled);
    }

    IZ_BOOL CTask::IsFinished()
    {
		std::unique_lock<std::mutex> lock(m_mutex);
        return (m_State == State_Finished);
    }

	// ƒ^ƒXƒN‚ª“o˜^‰Â”\‚©‚Ç‚¤‚©‚ð•Ô‚·.
	IZ_BOOL CTask::CanRegister()
	{
		std::unique_lock<std::mutex> lock(m_mutex);
		return (m_State == State_Finished) || (m_State == State_Init);
	}

    void CTask::Run(void* userData)
    {
		std::unique_lock<std::mutex> lock(m_mutex);

        if (m_State == State_WillCancel) {
            m_State = State_Canceled;
        }
        else {
            m_State = State_Running;
            OnRun();
            m_State = State_Finished;
        }

		m_condVar.notify_one();
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
