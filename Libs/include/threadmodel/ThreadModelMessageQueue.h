#if !defined(__IZANAGI_THREAD_MODEL_MESSAGE_QUEUE_H__)
#define __IZANAGI_THREAD_MODEL_MESSAGE_QUEUE_H__

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
        CMessage* Peek();
        CMessage* PeekWithWaitingIfNoMessage();

        CMessage* Get();
        CMessage* GetWithWaitingIfNoMessage();

        void Post(CMessage* msg);

        IZ_UINT GetPostedMessageNum();

        void WaitEmpty(IZ_BOOL deleteMsg);

    private:
        CStdList<CMessage> m_MessageList;

        sys::CEvent m_Event;
        sys::CMutex m_Mutex;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_QUEUE_H__)
