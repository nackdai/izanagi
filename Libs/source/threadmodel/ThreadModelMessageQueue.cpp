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

    CMessage* CMessageQueue::PeekWithWaitingIfNoMessage()
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

    CMessage* CMessageQueue::GetWithWaitingIfNoMessage()
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

    IZ_UINT CMessageQueue::GetPostedMessageNum()
    {
        sys::CGuarder guard(m_Mutex);
        {
            IZ_UINT ret = m_MessageList.GetItemNum();
            return ret;
        }
    }

    void CMessageQueue::WaitEmpty(IZ_BOOL deleteMsg)
    {
        for (;;) {
            CMessage* msg = Peek();

            if (msg == IZ_NULL) {
                break;
            }
            else if (deleteMsg) {
                CMessage::DeleteMessage(msg);
            }

            sys::CThread::YieldThread();
        }
    }
}   // namespace threadmodel
}   // namespace izanagi
