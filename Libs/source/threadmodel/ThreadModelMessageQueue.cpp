#include <thread>

#include "threadmodel/ThreadModelMessageQueue.h"
#include "threadmodel/ThreadModelMessage.h"

namespace izanagi
{
namespace threadmodel
{
    CMessageQueue::CMessageQueue()
    {
		m_isRunning.store(IZ_TRUE);
    }

    CMessageQueue::~CMessageQueue()
    {
    }

	// �o�^���b�Z�[�W���擾���L���[����폜����.
    CMessage* CMessageQueue::Peek()
    {
		CStdList<CMessage>::Item* item = IZ_NULL;

		std::unique_lock<std::mutex> lock(m_Mutex);

		item = m_MessageList.GetTop();

        CMessage* ret = IZ_NULL;

        if (item != IZ_NULL) {
            ret = item->GetData();
			ret->reset();
            item->Leave();
        }
            
        return ret;
    }

	// �o�^���b�Z�[�W���擾���L���[����폜����.�������A���b�Z�[�W�������ꍇ�͑҂�.
    CMessage* CMessageQueue::PeekWithWaitingIfNoMessage()
    {
		std::unique_lock<std::mutex> lock(m_Mutex);

		if (IsRunning()) {
			m_condVar.wait(
				lock,
				[this] {
				return m_MessageList.GetItemNum() > 0;
			});
		}

        CStdList<CMessage>::Item* item = m_MessageList.GetTop();

        CMessage* ret = IZ_NULL;

        if (item != IZ_NULL) {
            ret = item->GetData();
			ret->reset();
            item->Leave();
        }
            
        return ret;
    }

	// �o�^���b�Z�[�W���擾���邪�A�L���[����͍폜���Ȃ�.
    CMessage* CMessageQueue::Get()
    {
		CStdList<CMessage>::Item* item = IZ_NULL;

		{
			std::unique_lock<std::mutex> lock(m_Mutex);
			item = m_MessageList.GetTop();
		}

        CMessage* ret = IZ_NULL;

        if (item != IZ_NULL) {
            ret = item->GetData();
        }
            
        return ret;
    }

	// �o�^���b�Z�[�W���擾���邪�A�L���[����͍폜���Ȃ�.�������A���b�Z�[�W�������ꍇ�͑҂�
    CMessage* CMessageQueue::GetWithWaitingIfNoMessage()
    {
		std::unique_lock<std::mutex> lock(m_Mutex);

		if (IsRunning()) {
			m_condVar.wait(
				lock,
				[this] {
				return m_MessageList.GetItemNum() > 0;
			});
		}

        CStdList<CMessage>::Item* item = m_MessageList.GetTop();

        CMessage* ret = IZ_NULL;

        if (item != IZ_NULL) {
            ret = item->GetData();
        }
            
        return ret;
    }

	// ���b�Z�[�W��o�^����.
    IZ_BOOL CMessageQueue::Post(CMessage* msg)
    {
		if (!IsRunning()) {
			return IZ_FALSE;
		}

        IZ_ASSERT(msg != IZ_NULL);

		std::unique_lock<std::mutex> lock(m_Mutex);

		if (msg->isRegistered()) {
			return IZ_FALSE;
		}

        m_MessageList.AddTail(msg->GetItem());

		m_condVar.notify_one();

		return IZ_TRUE;
    }

	// �o�^����Ă��郁�b�Z�[�W�����擾.
    IZ_UINT CMessageQueue::GetPostedMessageNum()
    {
		std::unique_lock<std::mutex> lock(m_Mutex);
        IZ_UINT ret = m_MessageList.GetItemNum();
        return ret;
    }

	// ���s�����ǂ���.
	IZ_BOOL CMessageQueue::IsRunning()
	{
		return m_isRunning.load();
	}

	// ��ɂȂ�܂ő҂��ďI��.
	void CMessageQueue::Terminate(IZ_BOOL deleteMsg)
	{
		m_isRunning.store(IZ_FALSE);

		// TODO

		while (IZ_TRUE) {
			CMessage* msg = Peek();

			if (msg == IZ_NULL) {
				break;
			}

			if (deleteMsg) {
				CMessage::DeleteMessage(msg);
			}

			std::this_thread::yield();
		}
	}
}   // namespace threadmodel
}   // namespace izanagi
