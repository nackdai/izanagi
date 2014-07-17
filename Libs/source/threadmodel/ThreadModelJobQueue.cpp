#include "threadmodel/ThreadModelJobQueue.h"
#include "threadmodel/ThreadModelJobWorker.h"
#include "threadmodel/ThreadModelJob.h"

namespace izanagi
{
namespace threadmodel
{
    sys::CMutex CJobQueue::s_QueueListGuarder;
    CStdList<CJobQueue> CJobQueue::s_JobQueueList;

    CJobQueue::CJobQueue()
    {
        m_Allocator = IZ_NULL;

        m_WorkerNum = 0;
        m_Workers = IZ_NULL;

        m_IsTerminated = IZ_FALSE;

        m_ListItem.Init(this);
    }

    CJobQueue::~CJobQueue()
    {
        // �O�̂���
        Terminate();
    }

    IZ_BOOL CJobQueue::Init(
        IMemoryAllocator* allocator,
        IZ_UINT threadNum)
    {
        VRETURN(!m_IsTerminated);

        if (m_Workers != IZ_NULL) {
            return IZ_TRUE;
        }

        void* buf = ALLOC(
            allocator, 
            sizeof(CJobWorker*) * threadNum + sizeof(CJobWorker) * threadNum);
        VRETURN(buf != IZ_NULL);

        IZ_UINT8* p = (IZ_UINT8*)buf;

        m_Workers = reinterpret_cast<CJobWorker**>(p);
        p += sizeof(CJobWorker*) * threadNum; 

        for (IZ_UINT i = 0; i < threadNum; i++) {
            m_Workers[i] = new(p) CJobWorker();
            p += sizeof(CJobWorker);
        }

        m_WorkerNum = threadNum;
        m_Allocator = allocator;

        m_JobListGuarder.Open();

        VRETURN(s_QueueListGuarder.Open());

        sys::CGuarder guard(s_QueueListGuarder);
        {
            s_JobQueueList.AddTail(&m_ListItem);
        }

        return IZ_TRUE;
    }

    IZ_BOOL CJobQueue::Start()
    {
        VRETURN(m_Workers != IZ_NULL);

        for (IZ_UINT i = 0; i < m_WorkerNum; i++) {
            m_Workers[i]->Start();
        }

        return IZ_TRUE;
    }

    IZ_BOOL CJobQueue::Enqueue(CJob* job, IZ_BOOL deleteJobWhenFinish/*= IZ_FALSE*/)
    {
        IZ_ASSERT(job != IZ_NULL);

        VRETURN(m_Workers != IZ_NULL);

        sys::CGuarder guard(m_JobListGuarder);
        {
            // Job��ҋ@��ԂɃZ�b�g
            job->SetState(CJob::State_Waiting);

            if (deleteJobWhenFinish) {
                job->SetEnableDeleteWhenFinish();
            }

            // ���X�g�ɓo�^
            m_JobList.AddTail(job->GetListItem());
        }

        return IZ_TRUE;
    }

    void CJobQueue::WaitEmpty()
    {
        while (IZ_TRUE) {
            IZ_UINT jobNum = 0;

            {
                sys::CGuarder guard(m_JobListGuarder);

                jobNum = m_JobList.GetItemNum();
            }

            if (jobNum == 0) {
                break;
            }

            sys::CThread::YieldThread();
        }
    }

    void CJobQueue::CancelAllJobs()
    {
        sys::CGuarder guard(m_JobListGuarder);
        {
            CStdList<CJob>::Item* item = m_JobList.GetTop();

            while (item != IZ_NULL) {
                CJob* job = item->GetData();
                job->Cancel();

                CStdList<CJob>::Item* next = item->GetNext();
                item->Leave();

                item = next;

                if (job->EnableDeleteWhenFinish()) {
                    CJob::DeleteJob(job);
                }
            }
        }
    }

    void CJobQueue::Terminate()
    {
        if (m_IsTerminated) {
            return;
        }

        //CancelAllJobs();
        {
            sys::CGuarder guard(m_JobListGuarder);

            CStdList<CJob>::Item* item = m_JobList.GetTop();

            while (item != IZ_NULL) {
                CJob* job = item->GetData();
                job->Cancel();
                item = item->GetNext();
            }
        }

        // ���X�g����ɂȂ�܂ŏ�������
        while (IZ_TRUE) {
            IZ_UINT jobNum = 0;

            {
                sys::CGuarder guard(m_JobListGuarder);

                jobNum = m_JobList.GetItemNum();
            }

            if (jobNum == 0) {
                break;
            }

            Update();
        }

        // ���[�J�[��Join����
        for (IZ_UINT i = 0; i < m_WorkerNum; i++) {
            m_Workers[i]->Join();
            delete m_Workers[i];
        }

        FREE(m_Allocator, m_Workers);
        m_WorkerNum = 0;

        {
            sys::CGuarder guard(m_JobListGuarder);
            CStdList<CJob>::Item* item = m_JobList.GetTop();

            while (item != IZ_NULL) {
                CJob* job = item->GetData();

                CStdList<CJob>::Item* next = item->GetNext();
                item->Leave();

                item = next;

                if (job->EnableDeleteWhenFinish()) {
                    CJob::DeleteJob(job);
                }
            }
        }

        m_JobListGuarder.Close();

        // JobQueue���X�g���甲��
        {
            sys::CGuarder guard(s_QueueListGuarder);
            m_ListItem.Leave();
        }

        m_IsTerminated = IZ_TRUE;
    }

    void CJobQueue::Update()
    {
        IZ_ASSERT(m_Workers != IZ_NULL);

        sys::CGuarder guard(m_JobListGuarder);
        {
            CStdList<CJob>::Item* item = m_JobList.GetTop();

            while (item != IZ_NULL) {
                // �ҋ@��Ԃ̃��[�J�[���擾
                CJobWorker* worker = GetWaitingJobWorker();

                if (worker == IZ_NULL) {
                    // �󂢂Ă��郏�[�J�[���Ȃ��̂ŏI��
                    return;
                }

                CJob* job = item->GetData();

                IZ_BOOL isFinish = IZ_FALSE;

                if (job->IsCanceled()) {
                    job->Finish();
                    isFinish = IZ_TRUE;
                }
                else {
                    if (job->GetState() == CJob::State_Waiting) {
                        worker->Register(job);
                    }
                    else if (job->GetState() == CJob::State_WillFinish) {
                        job->Finish();
                        isFinish = IZ_TRUE;
                    }
                }

                CStdList<CJob>::Item* next = item->GetNext();

                if (isFinish) {
                    item->Leave();

                    if (job->EnableDeleteWhenFinish()) {
                        CJob::DeleteJob(job);
                    }
                }

                item = next;
            }
        }
    }

    // �󂢂Ă��郏�[�J�[���擾.
    CJobWorker* CJobQueue::GetWaitingJobWorker()
    {
        for (IZ_UINT i = 0; i < m_WorkerNum; i++) {
            if (m_Workers[i]->IsWaiting()) {
                return m_Workers[i];
            }
        }

        return IZ_NULL;
    }

    void CJobQueue::UpdateQueues()
    {
        sys::CGuarder guard(s_QueueListGuarder);
        {
            CStdList<CJobQueue>::Item* item = s_JobQueueList.GetTop();

            while (item != IZ_NULL) {
                CJobQueue* jobQueue = item->GetData();
                jobQueue->Update();

                item = item->GetNext();
            }
        }
    }

    void CJobQueue::TerminateJobQueue()
    {
        {
            sys::CGuarder guard(s_QueueListGuarder);

            CStdList<CJobQueue>::Item* item = s_JobQueueList.GetTop();

            while (item != IZ_NULL) {
                CJobQueue* jobQueue = item->GetData();
                item = item->GetNext();

                jobQueue->Terminate();
            }

            s_JobQueueList.Clear();
        }

        s_QueueListGuarder.Close();
    }
}   // namespace threadmodel
}   // namespace izanagi
