#if !defined(__IZANAGI_THREAD_MODEL_MESSAGE_QUEUE_H__)
#define __IZANAGI_THREAD_MODEL_MESSAGE_QUEUE_H__

#include "izStd.h"
#include "izSystem.h"

#include "threadmodel/ThreadModelMessage.h"

namespace izanagi
{
namespace threadmodel
{
    /**
     */
    class CMessageQueue
    {
    public:
        CMessageQueue()
        {
            m_Event.Open();
            m_Event.Set();
            m_Mutex.Open();
        }
        ~CMessageQueue() {}

        NO_COPIABLE(CMessageQueue);

    public:
        CMessage* PeekWithMatchFull(IZ_UINT key);
        CMessage* Peek(IZ_UINT key);

        CMessage* GetWithMatchFull(IZ_UINT key);
        CMessage* Get(IZ_UINT key);

        void Post(IZ_UINT key, CMessage* msg);

    private:
        static const IZ_UINT HASH_NUM = 5;  // TODO
        typedef CStdQueue<CMessage> QUEUE;
        typedef CStdList<CMessage> LIST;

        QUEUE m_Queue[HASH_NUM];
        sys::CEvent m_Event;
        sys::CMutex m_Mutex;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_MODEL_JOB_QUEUE_H__)
