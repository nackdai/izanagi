#if !defined(__URANUS_STD_LIST_H__)
#define __URANUS_STD_LIST_H__

#include "unDefs.h"
#include "std/StdPlacementNew.h"

namespace uranus {
	/**
	*/
	template <typename _T>
	class CStdList : public CPlacementNew {
	public:
		class Item {
			friend class CStdList<_T>;

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

			// 指定アイテムの前に入る
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

			// 指定アイテムの後に入る
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

		private:
			_T* m_pData;
			CStdList<_T>* m_pList;	// 所属先リスト
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

		~CStdList()
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

		UN_BOOL AddTop(Item* pItem)
		{
			UN_ASSERT(pItem != UN_NULL);
			return pItem->JoinAfter(&m_Top);
		}

		UN_BOOL AddTail(Item* pItem)
		{
			UN_ASSERT(pItem != UN_NULL);
			return pItem->JoinBefore(&m_Tail);
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

		NO_COPIABLE(CStdList);

	private:
		UN_UINT32 m_nNum;

		Item m_Top;
		Item m_Tail;
	};

	/////////////////////////////////////////////////////

	template <typename _T>
	class CStdSimpleList {
	public:

		CStdSimpleList()
		{
			m_Top = m_Tail = UN_NULL;
		}

		UN_BOOL HasItem() { return (m_Top != UN_NULL); }

		_T* GetTop() { return m_Top; }
		_T* GetTail() { return m_Tail; }

		void AddTop(_T* p)
		{
			if (m_Top != UN_NULL) {
				p->next = m_Top;
				p->prev = UN_NULL;

				m_Top->prev = p;

				m_Top = p;
			}
			else {
				m_Top = p;
				m_Tail = p;
			}
		}

		void AddTail(_T* p)
		{
			if (m_Tail != UN_NULL) {
				p->next = UN_NULL;
				p->prev = m_Tail;

				m_Tail->next = p;

				m_Tail = p;
			}
			else {
				m_Top = p;
				m_Tail = p;
			}
		}

		void Clear()
		{
			_T* p = GetTop();
			_T* next = UN_NULL;

			while (p != UN_NULL) {
				next = p->next;

				p->prev = UN_NULL;
				p->next = UN_NULL;

				p = next;
			}

			m_Top = UN_NULL;
			m_Tail = UN_NULL;
		}

		void Remove(_T* p)
		{
			UN_BOOL bIsRemoved = UN_FALSE;

			if (m_Top == p) {
				if (p->next != UN_NULL) {
					p->next->prev = UN_NULL;
				}
				m_Top = p->next;
				bIsRemoved = UN_TRUE;
			}
			
			if (m_Tail == p) {
				if (p->prev != UN_NULL) {
					p->prev->next = UN_NULL;
				}
				m_Tail = p->prev;
				bIsRemoved = UN_TRUE;
			}

			if (!bIsRemoved) {
				if (p->prev != UN_NULL) {
					p->prev->next = p->next;
				}
				if (p->next != UN_NULL) {
					p->next->prev = p->prev;
				}

				bIsRemoved = UN_TRUE;
			}

			if (bIsRemoved) {
				p->next = UN_NULL;
				p->prev = UN_NULL;
			}
		}

		// p0 の前に p1 を挿入
		void JoinBefore(_T* p0, _T* p1)
		{
			_T* prev = p0->prev;

			if (p0->prev != UN_NULL) {
				p0->prev->next = p1;
			}
			p0->prev = p1;

			p1->prev = prev;
			p1->next = p0;

			if (p0 == m_Top) {
				m_Top = p1;
			}
		}

		// p0 の後に p1 を挿入
		void JoinAfter(_T* p0, _T* p1)
		{
			_T* next = p0->next;

			if (p0->next != UN_NULL) {
				p0->next->prev = p1;
			}
			p0->next = p1;

			p1->prev = p0;
			p1->next = next;

			if (p0 == m_Tail) {
				m_Tail = p1;
			}
		}

		NO_COPIABLE(CStdSimpleList);

	private:
		_T* m_Top;
		_T* m_Tail;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_LIST_H__)
