#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"
#include "threadmodel/ThreadModelJobQueue.h"

namespace izanagi
{
namespace threadmodel
{

CJob::CJob()
{
    m_QueueItem.Init(this);
    m_JobQueue = IZ_NULL;
}

// �L�����Z��.
IZ_BOOL CJob::Cancel()
{
    IZ_ASSERT(m_JobQueue != IZ_NULL);

    return m_JobQueue->Cancel(this);
}

// �W���u�L���[�ɓo�^�����Ƃ��ɌĂ΂��
void CJob::Registered(CJobQueue* jobQueue)
{
    // NOTE
    // ���b�N����Ă����ԂŌĂяo����邱�Ƃ��ۏ؂����

    if (m_JobQueue != jobQueue)
    {
        if (m_JobQueue != IZ_NULL)
        {
            m_JobQueue->Cancel(this);
        }

        m_JobQueue = jobQueue;
    }
}

// �������I�����ăW���u�L���[����O���Ƃ��ɌĂ΂��
void CJob::Detached()
{
    // NOTE
    // ���b�N����Ă����ԂŌĂяo����邱�Ƃ��ۏ؂����

    m_JobQueue = IZ_NULL;
}

}   // namespace threadmodel
}   // namespace izanagi
