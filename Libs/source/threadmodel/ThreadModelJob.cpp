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

        m_IsFinished = IZ_FALSE;
        m_WillCancel = IZ_FALSE;
        m_IsCanceled = IZ_FALSE;
    }

    // �L�����Z��.
    void CJob::Cancel()
    {
        if (m_JobQueue->Cancel(this))
        {
            NotifyCancel();
            Detach();
        }
        else
        {
            m_WillCancel = IZ_TRUE;
        }
    }

    // �w�肳�ꂽ�W���u�L���[�ɓo�^�ς݂��ǂ���
    IZ_BOOL CJob::IsRegistered(CJobQueue* jobQueue)
    {
        return (m_JobQueue == jobQueue);
    }

    // �G���L���[�̂��߂̏���
    void CJob::Prepare(CJobQueue* jobQueue)
    {
        // NOTE
        // ���b�N����Ă����ԂŌĂяo����邱�Ƃ��ۏ؂����

        // �t���O�����Z�b�g
        m_IsFinished = IZ_FALSE;
        m_WillCancel = IZ_FALSE;
        m_IsCanceled = IZ_FALSE;

        m_JobQueue = jobQueue;
    }

    // �������I�����ăW���u�L���[����O���Ƃ��ɌĂ΂��
    void CJob::Detach()
    {
        // NOTE
        // ���b�N����Ă����ԂŌĂяo����邱�Ƃ��ۏ؂����

        if (m_QueueItem.HasList())
        {
            m_QueueItem.Leave();
        }

        m_JobQueue = IZ_NULL;
        m_IsFinished = IZ_TRUE;
    }

    void CJob::NotifyCancel()
    {
        m_WillCancel = IZ_FALSE;
        m_IsCanceled = IZ_TRUE;
    }

}   // namespace threadmodel
}   // namespace izanagi
