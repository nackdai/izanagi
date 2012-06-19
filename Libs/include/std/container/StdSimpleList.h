#if !defined(__IZANAGI_STD_SIMPLE_LIST_H__)
#define __IZANAGI_STD_SIMPLE_LIST_H__

#include "izDefs.h"
#include "std/StdPlacementNew.h"

namespace izanagi {
	/**
	 */
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

#endif	// #if !defined(__IZANAGI_STD_SIMPLE_LIST_H__)
