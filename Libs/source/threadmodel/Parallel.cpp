#include "threadmodel/Parallel.h"

namespace izanagi
{
namespace threadmodel
{
    CParallel::CParallel()
    {
    }

    CParallel::~CParallel()
    {
    }

    void CParallel::Start()
    {
        m_IsRunning = IZ_TRUE;

        m_JobQueue.Start();
    }

    void CParallel::Join()
    {
        m_JobQueue.Join();

        m_IsRunning = IZ_FALSE;
    }
}   // namespace threadmodel
}   // namespace izanagi
