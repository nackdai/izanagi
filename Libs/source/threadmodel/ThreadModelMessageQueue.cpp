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

	// 登録メッセージを取得かつキューから削除する.
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

	// 登録メッセージを取得かつキューから削除する.ただし、メッセージが無い場合は待つ.
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

	// 登録メッセージを取得するが、キューからは削除しない.
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

	// 登録メッセージを取得するが、キューからは削除しない.ただし、メッセージが無い場合は待つ
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

	// メッセージを登録する.
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

	// 登録されているメッセージ数を取得.
    IZ_UINT CMessageQueue::GetPostedMessageNum()
    {
		std::unique_lock<std::mutex> lock(m_Mutex);
        IZ_UINT ret = m_MessageList.GetItemNum();
        return ret;
    }

	// 実行中かどうか.
	IZ_BOOL CMessageQueue::IsRunning()
	{
		return m_isRunning.load();
	}

	// 空になるまで待って終了.
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
