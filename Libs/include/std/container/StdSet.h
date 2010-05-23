#if !defined(__URANUS_STD_SET_H__)
#define __URANUS_STD_SET_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	template <typename _T>
	class CStdSet {
	public:
		class Item {
			friend class CStdSet<_T>;

		public:
			Item()
			{
				m_pData = UN_NULL;
				m_pList = UN_NULL;
				m_pPrev = UN_NULL;
				m_pNext = UN_NULL;
			}

			~Item()
			{
				Leave();
			}

			void Init(_T* pData)
			{
				UN_ASSERT(pData != UN_NULL);
				m_pData = pData;
			}

			_T* GetData()
			{
				//UN_ASSERT(m_pData != UN_NULL);
				return m_pData;
			}

			Item* GetPrev()
			{
				UN_ASSERT(HasList());
				return ((m_pPrev != &m_pList->m_Top) ? m_pPrev : UN_NULL);
			}

			Item* GetNext()
			{
				UN_ASSERT(HasList());
				return (m_pNext != &m_pList->m_Tail) ? m_pNext : UN_NULL;
			}

			UN_BOOL HasList() const
			{
				return (m_pList != UN_NULL);
			}

			CStdList<_T>* GetList()
			{
				return m_pList;
			}

			UN_BOOL JoinBefore(Item* pItem)
			{
				VRETURN(
					(pItem != UN_NULL)
					&& (pItem != this)
					&& (pItem->HasList()));

				// 今所属しているリストから外れる
				Leave();

				m_pList = pItem->m_pList;
				m_pPrev = pItem->m_pPrev;
				m_pNext = pItem;

				m_pPrev->m_pNext = this;
				m_pNext->m_pPrev = this;

				m_pList->m_nNum++;

				return UN_TRUE;
			}

			UN_BOOL JoinAfter(Item* pItem)
			{
				VRETURN(
					(pItem != UN_NULL)
					&& (pItem != this)
					&& (pItem->HasList()));

				// 今所属しているリストから外れる
				Leave();

				m_pList = pItem->m_pList;
				m_pPrev = pItem;
				m_pNext = pItem->m_pNext;

				m_pPrev->m_pNext = this;
				m_pNext->m_pPrev = this;

				m_pList->m_nNum++;

				return UN_TRUE;
			}

			void Leave()
			{
				if ((m_pPrev != UN_NULL)
					&& (m_pNext != UN_NULL))
				{
					UN_ASSERT(m_pList != UN_NULL);
					m_pList->m_nNum--;

					m_pPrev->m_pNext = m_pNext;
					m_pNext->m_pPrev = m_pPrev;

					m_pList = UN_NULL;
					m_pPrev = UN_NULL;
					m_pNext = UN_NULL;
				}
			}

			UN_BOOL operator>(Item* pItem)
			{
				UN_ASSERT(m_pData != UN_NULL);
				UN_ASSERT(Item != UN_NULL);
				UN_ASSERT(Item->m_pData != UN_NULL);

				return m_pData > pItem->m_pData;
			}
			UN_BOOL operator==(Item* pItem)
			{
				UN_ASSERT(m_pData != UN_NULL);
				UN_ASSERT(Item != UN_NULL);
				UN_ASSERT(Item->m_pData != UN_NULL);

				return m_pData == pItem->m_pData;
			}

		private:
			_T* m_pData;
			CStdList<_T>* m_pList;	// 所属先リスト
			Item* m_pPrev;
			Item* m_pNext;
		};

	public:
		CStdSet()
		{
			m_nNum = 0;

			m_Top.m_pList = this;
			m_Tail.m_pList = this;

			m_Top.m_pNext = &m_Tail;
			m_Tail.m_pPrev = &m_Top;
		}

		~CStdSet()
		{
			Clear();
		}

		UN_UINT GetItemNum() const
		{
			return m_nNum;
		}

		UN_BOOL HasItem()
		{
			return (GetTop() != UN_NULL);
		}

		Item* GetTop()
		{
			return (m_Top.m_pNext != &m_Tail) ? m_Top.m_pNext : UN_NULL;
		}

		Item* GetTail()
		{
			return (m_Tail.m_pPrev != &m_Top) ? m_Tail.m_pPrev : UN_NULL;
		}

		Item* GetAt(UN_UINT idx)
		{
			UN_UINT nNum = GetItemNum();
			VRETURN_VAL(idx < nNum, UN_NULL);

			CStdList<_T>::Item* ret = GetTop();
			for (UN_UINT i = 0; i < nNum; i++) {
				if (i == idx) {
					break;
				}
				ret = ret->GetNext();
			}

			return ret;
		}

		UN_BOOL Add(Item* pItem)
		{
			UN_UINT nNum = GetItemNum();
			if (nNum == 0) {
				return pItem->JoinAfter(&m_Top);
			}
			else {
				UN_UINT nTopIdx = 0;
				UN_UINT nTailIdx = nNum - 1;

				Item* pTop = GetAt(nTopIdx);
				Item* pTail = GetAt(nTailIdx);

				if (pTop > pItem) {
					// Add to top.
					return pItem->JoinBefore(pTop);
				}
				else if (pTail < pItem) {
					// Add to tail.
					return pItem->JoinBefore(pTail);
				}

				Item* pMid = UN_NULL;

				// Search insert position by binary search.
				while (nTopIdx <= nTailIdx) {
					UN_UINT nMidxIdx = (nTopIdx + nTailIdx) >> 1;
					pMid = GetAt(nMidIdx);

					if (pMid < pItem) {
						nTopIdx = nMidxIdx + 1;
					}
					else if (pMid > pItem) {
						nTailIdx = nMidIdx - 1;
					}
					else {
						return UN_FALSE;
					}
				}

				// Insert item.
				for (Item* p = pMid;; p != UN_NULL; p = p->GetNext();) {
					if (p < pItem) {
						return pItem->JoinAfter(p);
					}
				}

				UN_ASSERT(UN_FALSE);
				return UN_FALSE;
			}
		}

		void Clear()
		{
			while (HasItem()) {
				GetTop()->Leave();
			}
		}

		void Init(UN_BOOL force = UN_TRUE)
		{
			if (force
				|| (!force || ((m_Top.m_pList == UN_NULL) || (m_Tail.m_pList == UN_NULL))))
			{
				Clear();

				m_nNum = 0;

				m_Top.m_pList = this;
				m_Tail.m_pList = this;

				m_Top.m_pNext = &m_Tail;
				m_Tail.m_pPrev = &m_Top;
			}
		}

		NO_COPIABLE(CStdSet);

	private:
		UN_UINT32 m_nNum;

		Item m_Top;
		Item m_Tail;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_SET_H__)
