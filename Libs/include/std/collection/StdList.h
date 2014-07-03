#if !defined(__IZANAGI_STD_LIST_H__)
#define __IZANAGI_STD_LIST_H__

#include "izDefs.h"
#include "std/StdPlacementNew.h"
#include "std/allocator/MemoryAllocator.h"

namespace izanagi {
    /**
     */
    template <typename _T>
    class CStdList {
    public:
        IZ_DECL_PLACEMENT_NEW();

    public:
        class Item {
            friend class CStdList<_T>;

        public:
            Item()
            {
                m_pData = IZ_NULL;
                m_pList = IZ_NULL;
                m_pPrev = IZ_NULL;
                m_pNext = IZ_NULL;
            }

            ~Item()
            {
                Leave();
            }

            void Init(_T* pData)
            {
                IZ_ASSERT(pData != IZ_NULL);
                m_pData = pData;
            }

            _T* GetData()
            {
                //IZ_ASSERT(m_pData != IZ_NULL);
                return m_pData;
            }

            Item* GetPrev()
            {
                IZ_ASSERT(HasList());
                return ((m_pPrev != &m_pList->m_Top) ? m_pPrev : IZ_NULL);
            }

            Item* GetNext()
            {
                IZ_ASSERT(HasList());
                return (m_pNext != &m_pList->m_Tail) ? m_pNext : IZ_NULL;
            }

            IZ_BOOL HasList() const
            {
                return (m_pList != IZ_NULL);
            }

            CStdList<_T>* GetList()
            {
                return m_pList;
            }

            // 指定アイテムの前に入る
            IZ_BOOL JoinBefore(Item* pItem)
            {
                VRETURN(
                    (pItem != IZ_NULL)
                    && (pItem != this)
                    && (pItem->HasList()));

                // 今所属しているリストから外れる
                Leave();

                m_pList = pItem->m_pList;
                m_pPrev = pItem->m_pPrev;
                m_pNext = pItem;

                m_pPrev->m_pNext = this;
                m_pNext->m_pPrev = this;

                ++(m_pList->m_nNum);

                return IZ_TRUE;
            }

            // 指定アイテムの後に入る
            IZ_BOOL JoinAfter(Item* pItem)
            {
                VRETURN(
                    (pItem != IZ_NULL)
                    && (pItem != this)
                    && (pItem->HasList()));

                // 今所属しているリストから外れる
                Leave();

                m_pList = pItem->m_pList;
                m_pPrev = pItem;
                m_pNext = pItem->m_pNext;

                m_pPrev->m_pNext = this;
                m_pNext->m_pPrev = this;

                ++(m_pList->m_nNum);

                return IZ_TRUE;
            }

            void Leave()
            {
                if ((m_pPrev != IZ_NULL)
                    && (m_pNext != IZ_NULL))
                {
                    IZ_ASSERT(m_pList != IZ_NULL);
                    m_pList->m_nNum--;

                    m_pPrev->m_pNext = m_pNext;
                    m_pNext->m_pPrev = m_pPrev;

                    m_pList = IZ_NULL;
                    m_pPrev = IZ_NULL;
                    m_pNext = IZ_NULL;
                }
            }

        private:
            _T* m_pData;
            CStdList<_T>* m_pList;  // 所属先リスト
            Item* m_pPrev;
            Item* m_pNext;
        };

    public:
        CStdList()
        {
            m_nNum = 0;

            m_Top.m_pList = this;
            m_Tail.m_pList = this;

            m_Top.m_pNext = &m_Tail;
            m_Tail.m_pPrev = &m_Top;
        }

        virtual ~CStdList()
        {
            Clear();
        }

        NO_COPIABLE(CStdList);

    public:
        IZ_UINT GetItemNum() const
        {
            return m_nNum;
        }

        IZ_BOOL HasItem()
        {
            return (GetTop() != IZ_NULL);
        }

        Item* GetTop()
        {
            return (m_Top.m_pNext != &m_Tail) ? m_Top.m_pNext : IZ_NULL;
        }

        Item* GetTail()
        {
            return (m_Tail.m_pPrev != &m_Top) ? m_Tail.m_pPrev : IZ_NULL;
        }

        Item* GetAt(IZ_UINT idx)
        {
            IZ_UINT nNum = GetItemNum();
            VRETURN_VAL(idx < nNum, IZ_NULL);

            CStdList<_T>::Item* ret = GetTop();
            for (IZ_UINT i = 0; i < nNum; ++i) {
                if (i == idx) {
                    break;
                }
                ret = ret->GetNext();
            }

            return ret;
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
            while (HasItem()) {
                GetTop()->Leave();
            }
        }

        void Init(IZ_BOOL force = IZ_TRUE)
        {
            if (force
                || (!force || ((m_Top.m_pList == IZ_NULL) || (m_Tail.m_pList == IZ_NULL))))
            {
                Clear();

                m_nNum = 0;

                m_Top.m_pList = this;
                m_Tail.m_pList = this;

                m_Top.m_pNext = &m_Tail;
                m_Tail.m_pPrev = &m_Top;
            }
        }

        typedef IZ_BOOL(*FuncMatch)(_T* data);

        _T* Find(FuncMatch* funcMatch)
        {
            IZ_ASSERT(funcMatch != IZ_NULL);

            CStdList<_T>::Item* item = GetTop();
            while (item != IZ_NULL)
            {
                if ((*funcMatch)(item->GetData()))
                {
                    return item->GetData();
                }
                item = item->GetNext();
            }
            return IZ_NULL;
        }
        
        IZ_INT Find(_T* data)
        {
            IZ_UINT idx = 0;

            CStdList::Item* item = GetTop();
            while (item != IZ_NULL)
            {
                _T* d = item->GetData();
                if (d == data)
                {
                    return idx;
                }

                item = item->GetNext();
                idx++;
            }

            return -1;
        }

    private:
        IZ_UINT32 m_nNum;

        Item m_Top;
        Item m_Tail;
    };

    /**
     */
    template <typename T>
    class CStdListEx : public CStdList<T>
    {
    public:
        class Item : CStdList<T>::Item
        {
            friend class CStdListEx<T>;

            IZ_DECL_PLACEMENT_NEW();

        protected:
            void Leave()
            {
                CStdList<T>::Item::Leave();
            }
        };

    public:
        CStdListEx() : CStdList<T>()
        {
            m_Allocator = IZ_NULL;
        }

        NO_COPIABLE(CStdListEx);

    public:
        void Init(IMemoryAllocator* allocator)
        {
            CStdList<T>::Init();
            m_Allocator = allocator;
        }

        IZ_BOOL AddTop(T* data)
        {
            IZ_ASSERT(m_Allocator != IZ_NULL);
            
            void* buf = ALLOC_ZERO(m_Allocator, sizeof(Item));
            VRETURN(buf != IZ_NULL);

            Item* item = new(buf) Item;
            item->Init(data);

            return AddTop(item);
        }

        IZ_BOOL AddTail(T* data)
        {
            IZ_ASSERT(m_Allocator != IZ_NULL);
            
            void* buf = ALLOC_ZERO(m_Allocator, sizeof(Item));
            VRETURN(buf != IZ_NULL);

            Item* item = new(buf) Item;
            item->Init(data);

            return AddTail(item);
        }

        IZ_INT Find(T* data)
        {
            IZ_UINT idx = 0;

            Item* item = this->GetTop();
            while (item != IZ_NULL)
            {
                T* d = item->GetData();
                if (d == data)
                {
                    return idx;
                }

                item = item->GetNext();
                idx++;
            }

            return -1;
        }

        IZ_BOOL Remove(Item* item)
        {
            if (this == item->GetList())
            {
                item->Leave();
                delete item;
                FREE(m_Allocator, item);
                return IZ_TRUE;
            }
            return IZ_FALSE;
        }

        IZ_BOOL Remove(T* data)
        {
            IZ_ASSERT(data != IZ_NULL);

            Item* item = this->GetTop();
            while (item != IZ_NULL)
            {
                T* d = item->GetData();
                if (d == data)
                {
                    item->Leave();
                    delete item;
                    return IZ_TRUE;
                }

                item = item->GetNext();
            }

            IZ_ASSERT(IZ_FALSE);
            return IZ_FALSE;
        }

    private:
        IZ_BOOL AddTop(Item* pItem)
        {
            return CStdList<T>::AddTop(pItem);
        }

        IZ_BOOL AddTail(Item* pItem)
        {
            return CStdList<T>::AddTail(pItem);
        }

    private:
        IMemoryAllocator* m_Allocator;
    };
}   // namespace izanagi

#endif  // #if !defined(__IZANAGI_STD_LIST_H__)
