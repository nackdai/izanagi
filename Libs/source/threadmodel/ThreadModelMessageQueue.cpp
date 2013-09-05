#include "threadmodel/ThreadModelMessageQueue.h"
#include "threadmodel/ThreadModelMessage.h"

namespace izanagi
{
namespace threadmodel
{
    CMessageQueue::CMessageQueue()
    {
        m_Event.Open();
        m_Mutex.Open();
    }

    CMessageQueue::~CMessageQueue()
    {
        m_Mutex.Lock();
        {
            IZ_ASSERT(m_MessageList.GetItemNum() == 0);
        }
        m_Mutex.Unlock();

        m_Event.Close();
        m_Mutex.Close();
    }

    CMessage* CMessageQueue::Peek()
    {
        sys::CGuarder guard(m_Mutex);

        m_Event.Wait();

        CStdList<CMessage>::Item* item = m_MessageList.GetTop();

        CMessage* ret = IZ_NULL;

        if (item != IZ_NULL) {
            ret = item->GetData();
            item->Leave();
        }
        else {
            m_Event.Reset();
        }
            
        return ret;
    }

    CMessage* CMessageQueue::Get()
    {
        sys::CGuarder guard(m_Mutex);

        m_Event.Wait();

        CStdList<CMessage>::Item* item = m_MessageList.GetTop();

        CMessage* ret = IZ_NULL;

        if (item != IZ_NULL) {
            ret = item->GetData();
        }
        else {
            m_Event.Reset();
        }
            
        return ret;
    }

    void CMessageQueue::Post(CMessage* msg)
    {
        IZ_ASSERT(msg != IZ_NULL);

        sys::CGuarder guard(m_Mutex);

        m_MessageList.AddTail(msg->GetItem());

        m_Event.Set();
    }
}   // namespace threadmodel
}   // namespace izanagi
