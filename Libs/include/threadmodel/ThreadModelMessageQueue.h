#if !defined(__IZANAGI_THREAD_MODEL_MESSAGE_QUEUE_H__)
#define __IZANAGI_THREAD_MODEL_MESSAGE_QUEUE_H__

#include <atomic>

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    class CMessage;

    /**
     */
    class CMessageQueue
    {
    public:
        CMessageQueue();
        ~CMessageQueue();

        NO_COPIABLE(CMessageQueue);

    public:
		/** 登録メッセージを取得かつキューから削除する.
		 */
        CMessage* Peek();

		/** 登録メッセージを取得かつキューから削除する.ただし、メッセージが無い場合は待つ.
		*/
        CMessage* PeekWithWaitingIfNoMessage();

		/** 登録メッセージを取得するが、キューからは削除しない.
		 */
        CMessage* Get();

		/** 登録メッセージを取得するが、キューからは削除しない.ただし、メッセージが無い場合は待つ.
		*/
        CMessage* GetWithWaitingIfNoMessage();

		/** メッセージを登録する.
		 */
        IZ_BOOL Post(CMessage* msg);

		/** 登録されているメッセージ数を取得.
		 */
        IZ_UINT GetPostedMessageNum();

		/** 実行中かどうか.
		 */
		IZ_BOOL IsRunning();

		/** 空になるまで待って終了.
		 */
		void Terminate(IZ_BOOL deleteMsg);

    private:
        CStdList<CMessage> m_MessageList;

        std::mutex m_Mutex;
		std::condition_variable m_condVar;

		sys::CEvent m_emptyWaiter;

		std::atomic<IZ_BOOL> m_isRunning;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_QUEUE_H__)
