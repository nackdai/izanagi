#if !defined(__IZANAGI_THREADMODEL_THREADSAFE_LIST_H__)
#define __IZANAGI_THREADMODEL_THREADSAFE_LIST_H__

#include "izStd.h"
#include "izSystem.h"

namespace izanagi {
namespace threadmodel {
    /**
     */
    template <typename _T>
    class ThreadSafeList : public CStdList < _T > {
    public:
        class Item : public CStdList<_T>::Item {
        public:
            Item() : CStdList<_T>::Item()
            {
            }

            ~Item()
            {
                Leave();
            }

            Item* GetPrev()
            {
                sys::Lock locker(AsThreadSafeList()->m_locker);
                return CStdList<_T>::Item::GetPrev();
            }

            Item* GetNext()
            {
                sys::Lock locker(AsThreadSafeList()->m_locker);
                return CStdList<_T>::Item::GetNext();
            }

            // 指定アイテムの前に入る
            IZ_BOOL JoinBefore(Item* pItem)
            {
                sys::Lock locker(AsThreadSafeList()->m_locker);
                return CStdList<_T>::Item::JoinBefore(pItem);
            }

            // 指定アイテムの後に入る
            IZ_BOOL JoinAfter(Item* pItem)
            {
                sys::Lock locker(AsThreadSafeList()->m_locker);
                return CStdList<_T>::Item::JoinAfter(pItem);
            }

            void Leave()
            {
                sys::Lock locker(AsThreadSafeList()->m_locker);
                CStdList<_T>::Item::Leave();
            }

        private:
            ThreadSafeList<_T>* AsThreadSafeList()
            {
                return (ThreadSafeList<_T>*)m_pList;
            }
        };

    public:
        ThreadSafeList() : CStdList<_T>() {}

        virtual ~ThreadSafeList()
        {
            Clear();
        }

        NO_COPIABLE(ThreadSafeList);

    public:
        IZ_UINT GetItemNum() const
        {
            sys::Lock locker(m_locker);
            return CStdList<_T>::GetItemNum();
        }

        IZ_BOOL HasItem()
        {
            sys::Lock locker(m_locker);
            return CStdList<_T>::HasItem();
        }

        Item* GetTop()
        {
            sys::Lock locker(m_locker);
            return (Item*)CStdList<_T>::GetTop();
        }

        Item* GetTail()
        {
            sys::Lock locker(m_locker);
            return (Item*)CStdList<_T>::GetTail();
        }

        Item* GetAt(IZ_UINT idx)
        {
            sys::Lock locker(m_locker);
            return (Item*)CStdList<_T>::GetAt(idx);
        }

        IZ_BOOL AddTop(Item* pItem)
        {
            IZ_ASSERT(pItem != IZ_NULL);
            return pItem->JoinAfter(&m_Top);
        }

        IZ_BOOL AddTail(Item* pItem)
        {
            IZ_ASSERT(pItem != IZ_NULL);
            return pItem->JoinBefore(&m_Tail);
        }

        void Clear()
        {
            sys::Lock locker(m_locker);
            CStdList<_T>::Clear();
        }

        void Init(IZ_BOOL force = IZ_TRUE)
        {
            CStdList<_T>::Init(force);
        }

        _T* Find(std::function<IZ_BOOL(_T*)> funcMatch)
        {
            sys::Lock locker(m_locker);
            return CStdList<_T>::Find(funcMatch);
        }

        IZ_INT Find(_T* data)
        {
            sys::Lock locker(m_locker);
            return CStdList<_T>::Find(data);
        }

        sys::CSpinLock& GetLocker()
        {
            return m_locker;
        }

    private:
        sys::CSpinLock m_locker;
    };
}   // namespace threadmodel
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_THREADMODEL_THREADSAFE_LIST_H__)
