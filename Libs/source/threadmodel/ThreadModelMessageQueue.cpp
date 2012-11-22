#include "threadmodel/ThreadModelMessageQueue.h"

namespace izanagi
{
namespace threadmodel
{
    CMessage* CMessageQueue::PeekWithMatchFull(IZ_UINT key)
    {
        CMessage* ret = IZ_NULL;

        IZ_UINT pos = key % HASH_NUM;

        m_Mutex.Lock();
        {
            LIST& list = (LIST&)m_Queue[pos];

            LIST::Item* item = list.GetTop();
            while (item != IZ_NULL)
            {
                CMessage* data = item->GetData();
                if (data->GetKey() == key)
                {
                    ret = data;
                    break;
                }
                item = item->GetNext();
            }
        }
        m_Mutex.Unlock();

        return ret;
    }

    CMessage* CMessageQueue::Peek(IZ_UINT key)
    {
        CMessage* ret = IZ_NULL;

        IZ_UINT pos = key % HASH_NUM;

        m_Mutex.Lock();
        {
            QUEUE::Item* item = m_Queue[pos].Dequeue();
            if (item != IZ_NULL)
            {
                ret = item->GetData();
            }
        }
        m_Mutex.Unlock();

        return ret;
    }

    CMessage* CMessageQueue::GetWithMatchFull(IZ_UINT key)
    {
        CMessage* ret = IZ_NULL;

        IZ_UINT pos = key % HASH_NUM;

        while (m_Event.Wait())
        {
            m_Mutex.Lock();
            {
                LIST& list = (LIST&)m_Queue[pos];

                if (list.GetItemNum() == 0)
                {
                    m_Event.Reset();
                }
                else
                {
                    LIST::Item* item = list.GetTop();
                    while (item != IZ_NULL)
                    {
                        CMessage* data = item->GetData();
                        if (data->GetKey() == key)
                        {
                            ret = data;
                            goto __EXIT__;
                        }
                        item = item->GetNext();
                    }

                    goto __EXIT__;
                }
            }
            m_Mutex.Unlock();
        }

        __EXIT__:
        return ret;
    }

    CMessage* CMessageQueue::Get(IZ_UINT key)
    {
        CMessage* ret = IZ_NULL;

        IZ_UINT pos = key % HASH_NUM;

        while (m_Event.Wait())
        {
            m_Mutex.Lock();
            {
                QUEUE& queue = m_Queue[pos];

                if (queue.GetItemNum() == 0)
                {
                    m_Event.Reset();
                }
                else
                {
                    QUEUE::Item* item = queue.Dequeue();
                    if (item != IZ_NULL)
                    {
                        ret = item->GetData();
                        break;
                    }
                }
            }
            m_Mutex.Unlock();
        }

        return ret;
    }

    void CMessageQueue::Post(IZ_UINT key, CMessage* msg)
    {
        IZ_UINT pos = key % HASH_NUM;

        msg->SetKey(key);

        m_Mutex.Lock();
        {
            m_Queue[pos].Enqueue(msg->GetItem());
        }
        m_Mutex.Unlock();

        m_Event.Set();
    }
}   // namespace threadmodel
}   // namespace izanagi
