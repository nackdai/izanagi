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
		/** �o�^���b�Z�[�W���擾���L���[����폜����.
		 */
        CMessage* Peek();

		/** �o�^���b�Z�[�W���擾���L���[����폜����.�������A���b�Z�[�W�������ꍇ�͑҂�.
		*/
        CMessage* PeekWithWaitingIfNoMessage();

		/** �o�^���b�Z�[�W���擾���邪�A�L���[����͍폜���Ȃ�.
		 */
        CMessage* Get();

		/** �o�^���b�Z�[�W���擾���邪�A�L���[����͍폜���Ȃ�.�������A���b�Z�[�W�������ꍇ�͑҂�.
		*/
        CMessage* GetWithWaitingIfNoMessage();

		/** ���b�Z�[�W��o�^����.
		 */
        IZ_BOOL Post(CMessage* msg);

		/** �o�^����Ă��郁�b�Z�[�W�����擾.
		 */
        IZ_UINT GetPostedMessageNum();

		/** ���s�����ǂ���.
		 */
		IZ_BOOL IsRunning();

		/** ��ɂȂ�܂ő҂��ďI��.
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
