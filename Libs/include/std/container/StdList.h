#if !defined(__IZANAGI_STD_LIST_H__)
#define __IZANAGI_STD_LIST_H__

#include "izDefs.h"
#include "std/StdPlacementNew.h"

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

		NO_COPIABLE(CStdList);

	private:
		IZ_UINT32 m_nNum;

		Item m_Top;
		Item m_Tail;
	};

	/////////////////////////////////////////////////////

	template <typename _T>
	class CStdSimpleList {
	public:

		CStdSimpleList()
		{
			m_Top = m_Tail = IZ_NULL;
		}

		IZ_BOOL HasItem() { return (m_Top != IZ_NULL); }

		_T* GetTop() { return m_Top; }
		_T* GetTail() { return m_Tail; }

		void AddTop(_T* p)
		{
			if (m_Top != IZ_NULL) {
				p->next = m_Top;
				p->prev = IZ_NULL;

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
			if (m_Tail != IZ_NULL) {
				p->next = IZ_NULL;
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
			_T* next = IZ_NULL;

			while (p != IZ_NULL) {
				next = p->next;

				p->prev = IZ_NULL;
				p->next = IZ_NULL;

				p = next;
			}

			m_Top = IZ_NULL;
			m_Tail = IZ_NULL;
		}

		void Remove(_T* p)
		{
			IZ_BOOL bIsRemoved = IZ_FALSE;

			if (m_Top == p) {
				if (p->next != IZ_NULL) {
					p->next->prev = IZ_NULL;
				}
				m_Top = p->next;
				bIsRemoved = IZ_TRUE;
			}
			
			if (m_Tail == p) {
				if (p->prev != IZ_NULL) {
					p->prev->next = IZ_NULL;
				}
				m_Tail = p->prev;
				bIsRemoved = IZ_TRUE;
			}

			if (!bIsRemoved) {
				if (p->prev != IZ_NULL) {
					p->prev->next = p->next;
				}
				if (p->next != IZ_NULL) {
					p->next->prev = p->prev;
				}

				bIsRemoved = IZ_TRUE;
			}

			if (bIsRemoved) {
				p->next = IZ_NULL;
				p->prev = IZ_NULL;
			}
		}

		// p0 の前に p1 を挿入
		void JoinBefore(_T* p0, _T* p1)
		{
			_T* prev = p0->prev;

			if (p0->prev != IZ_NULL) {
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

			if (p0->next != IZ_NULL) {
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
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_LIST_H__)
