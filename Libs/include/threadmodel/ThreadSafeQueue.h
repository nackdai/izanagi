#if !defined(__IZANAGI_THREAD_SAFE_QUEUE_H__)
#define __IZANAGI_THREAD_SAFE_QUEUE_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi
{
namespace threadmodel
{
    template <typename T>
    class CThreadSafeQueue
    {
    public:
        CThreadSafeQueue()
        {
            m_Mutex.Open();
        }
        ~CThreadSafeQueue()
        {
            IZ_ASSERT(m_Queue.GetItemNum() == 0);
            m_Mutex.Close();
        }

        NO_COPIABLE(CThreadSafeQueue);

    public:
        typedef void (*EnqueueFunc)(T* element, void* arg);

    public:
        // エンキュー
        IZ_BOOL Enqueue(
            typename CStdQueue<T>::Item* item,
            void* arg,
            EnqueueFunc func)
        {
            IZ_BOOL ret = IZ_FALSE;

            m_Mutex.Lock();
            {
                if (item->GetList() != (CStdList<T>*)m_Queue)
                {
                    ret = m_Queue.Enqueue(item);
                    if (ret && (func != IZ_NULL))
                    {
                        (*func)(item->GetData(), arg);
                    }
                }
            }
            m_Mutex.Unlock();

            return ret;
        }


        // デキュー
        T* Dequeue()
        {
            T* ret = IZ_NULL;

            m_Mutex.Lock();
            {
                CStdQueue<T>::Item* item = m_Queue.Dequeue();
                if (item != IZ_NULL)
                {
                    ret = item->GetData();
                }
            }
            m_Mutex.Unlock();

            return ret;
        }

        // 削除
        IZ_BOOL Remove(typename CStdQueue<T>::Item* item)
        {
            IZ_BOOL ret = IZ_FALSE;

            // まずは簡単に確認
            if (m_Queue.HasItem(item))
            {
                // キューに積まれているので外す
                m_Mutex.Lock();
                {
                    // もう一度確認しておく
                    if (m_Queue.HasItem(item))
                    {
                        item->Leave();
                        ret = IZ_TRUE;
                    }
                }
                m_Mutex.Unlock();
            }

            return ret;
        }

        // クリア
        void Clear()
        {
            m_Mutex.Lock();
            {
                CStdQueue<T>::Item* item = m_Queue.Dequeue();

                while (item != IZ_NULL)
                {
                    CStdQueue<T>::Item* next = item->GetNext();

                    item->Leave();
                    item->GetData()->Detach();

                    item = next;
                }
            }
            m_Mutex.Unlock();
        }

        // 要素が登録されているかどうか
        IZ_BOOL HasItem()
        {
            IZ_BOOL ret = IZ_FALSE;

            m_Mutex.Lock();
            {
                ret = m_Queue.HasItem();
            }
            m_Mutex.Unlock();

            return ret;
        }

        // 自己責任用

        void Lock()
        {
            m_Mutex.Lock();
        }
        
        void Unlock()
        {
            m_Mutex.Unlock();
        }

        typename CStdQueue<T>& GetQueue() { return m_Queue; }

    protected:
        typename CStdQueue<T> m_Queue;
        sys::CMutex m_Mutex;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREAD_SAFE_QUEUE_H__)
