#if !defined(__IZANAGI_STD_SET_H__)
#define __IZANAGI_STD_SET_H__

#include "izDefs.h"

namespace izanagi {
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

			IZ_BOOL operator>(Item* pItem)
			{
				IZ_ASSERT(m_pData != IZ_NULL);
				IZ_ASSERT(Item != IZ_NULL);
				IZ_ASSERT(Item->m_pData != IZ_NULL);

				return *m_pData > *(pItem->m_pData);
			}
			IZ_BOOL operator==(Item* pItem)
			{
				IZ_ASSERT(m_pData != IZ_NULL);
				IZ_ASSERT(Item != IZ_NULL);
				IZ_ASSERT(Item->m_pData != IZ_NULL);

				return *m_pData == *(pItem->m_pData);
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

		IZ_BOOL Add(Item* pItem)
		{
			IZ_UINT nNum = GetItemNum();
			if (nNum == 0) {
				return pItem->JoinAfter(&m_Top);
			}
			else {
				IZ_UINT nTopIdx = 0;
				IZ_UINT nTailIdx = nNum - 1;

				Item* pTop = GetAt(nTopIdx);
				Item* pTail = GetAt(nTailIdx);

				if (*pTop > *pItem) {
					// Add to top.
					return pItem->JoinBefore(pTop);
				}
				else if (*pItem > *pTail) {
					// Add to tail.
					return pItem->JoinBefore(pTail);
				}

				Item* pMid = IZ_NULL;

				// Search insert position by binary search.
				while (nTopIdx <= nTailIdx) {
					IZ_UINT nMidxIdx = (nTopIdx + nTailIdx) >> 1;
					pMid = GetAt(nMidIdx);

					if (*pMid < *pItem) {
						nTopIdx = nMidxIdx + 1;
					}
					else if (*pItem > *pMid) {
						nTailIdx = nMidIdx - 1;
					}
					else {
						return IZ_FALSE;
					}
				}

				// Insert item.
				for (Item* p = pMid;; p != IZ_NULL; p = p->GetNext();) {
					if (*pItem > *p) {
						return pItem->JoinAfter(p);
					}
				}

				IZ_ASSERT(IZ_FALSE);
				return IZ_FALSE;
			}
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

		NO_COPIABLE(CStdSet);

	private:
		IZ_UINT32 m_nNum;

		Item m_Top;
		Item m_Tail;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_SET_H__)
