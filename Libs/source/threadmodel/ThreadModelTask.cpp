#include "threadmodel/ThreadModelTask.h"

namespace izanagi
{
namespace threadmodel
{
    CTask::CTask()
    {
        m_Event.Open();
    }

    CTask::~CTask()
    {
        m_Event.Close();
    }

    void CTask::Wait()
    {
        m_Event.Wait();
    }

    IZ_BOOL CTask::OnRun()
    {
        IZ_BOOL ret = RunTask();
        m_Event.Set();
        return ret;
    }
}   // namespace threadmodel
}   // namespace izanagi
