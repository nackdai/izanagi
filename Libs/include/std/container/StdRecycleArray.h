#if !defined(__URANUS_STD_STD_RECYCLE_ARRAY_H__)
#define __URANUS_STD_STD_RECYCLE_ARRAY_H__

#include "unDefs.h"
#include "std/allocator/MemoryAllocator.h"

namespace uranus {
	/**
	*/
	template <typename _T>
	class CRecycleArray {
		struct SElement {
			_T data;
			UN_BOOL isUsed;
		};

	public:
		CRecycleArray()
		{
			m_pAllocator = UN_NULL;
		
			m_Elements = UN_NULL;

			m_nLastNewPos = 0;

			m_nElementNum = 0:
			m_nUsedNum = 0;
			m_nFreedNum = 0;
		}

		~CRecycleArray() { Clear(); }

		NO_COPIABLE(CRecycleArray);

	public:
		UN_BOOL Init(
			IMemoryAllocator* pAllocator,
			UN_UINT nNum)
		{
			UN_ASSERT(pAllocator != UN_NULL);
			UN_ASSERT(nNum > 0);

			m_Elements = reinterpret_cast<SElement>(ALLOC_ZERO(pAllocator, sizeof(SElemet) * nNum));
			VRETURN(m_Elements != UN_NULL);

			m_pAllocator = pAllocator;
			m_nElementNum = nNum;

			return UN_TRUE;
		}

		UN_BOOL Init(
			void* pBuf,
			UN_UINT nNum)
		{
			UN_ASSERT(pBuf != UN_NULL);
			UN_ASSERT(nNum > 0);

			m_Elements = reinterpret_cast<SElement>(pBuf);
			m_nElementNum = nNum;
		}

		void Clear()
		{
			if (m_pAllocator != UN_NULL) {
				FREE(m_pAllocator, m_Elements);
			}
			m_Elements = UN_NULL;
		}

		UN_BOOL IsValid()
		{
			return (m_Elements != UN_NULL);
		}

		_T* New()
		{
			UN_ASSERT(IsValid());

			UN_UINT nFreedPos = m_nLastNewPos;

			for (UN_UINT i = 0; i < m_nElementNum; i++) {
				if (m_Elements[nFreedPos].isUsed) {
					_T* ret = &m_Elements[nFreedPos].data;

					// Initialize data.
					*ret = m_ClearDummyData;

					m_Elements[nFreedPos].isUsed = UN_TRUE;
					m_nLastNewPos = nFreedPos;
					IncrementUsedNum();

					return ret;
				}
				else {
					nFreedPos++;
					if (nFreedPos >= m_nElementNum) {
						nFreedPos = 0;
					}
				}
			}

			UN_ASSERT(UN_FALSE);
			return UN_NULL;
		}

		void DeleteAll()
		{
			UN_ASSERT(IsValid());

			for (UN_UINT i = 0; i < m_nElementNum; i++) {
				m_Elements[i].isUsed = UN_FALSE;
			}
			Reset();
		}

		void Delete(UN_UINT idx)
		{
			UN_ASSERT(IsValid());
			UN_ASSERT(idx < m_nElementNum);

			m_Elements[idx].isUsed = UN_FALSE;
			IncrementFreedNum();
			return UN_TRUE;
		}

		UN_BOOL Delete(_T* pElem)
		{
			UN_ASSERT(IsValid());
			UN_ASSERT(pElem != UN_NULL);

			UN_BOOL ret = UN_FALSE;

			for (UN_UINT i = 0; i < m_nElementNum; i++) {
				ret = (&m_Elements[idx].data == pElem);
				if (ret) {
					m_Elements[idx].isUsed = UN_FALSE;
					IncrementFreedNum();
				}
			}

			return ret;
		}

		_T* GetAt(UN_UINT idx)
		{
			UN_ASSERT(IsValid());
			UN_ASSERT(idx < m_nElementNum);

			return &m_Elements[i].data;
		}

		UN_BOOL IsUsed(UN_UINT idx)
		{
			UN_ASSERT(IsValid());
			UN_ASSERT(idx < m_nElementNum);

			return m_Elements[idx].isUsed;
		}

		UN_UINT GetUsedNum() const  { return m_nUsedNum; }
		UN_UINT GetFreedNum() const { return m_nFreedNum; }
		UN_UINT GetTotalNum() const { return m_nElementNum; }

		void* GetBufferPtr() { return m_Elements; }

	protected:
		void Reset()
		{
			m_nLastNewPos = 0;
			m_nUsedNum = 0;
			m_nFreedNum = m_nElementNum;
		}

		void IncrementUsedNum()
		{
			m_nUsedNum++;
			m_nFreedNum--;
			UN_ASSERT(m_nUsedNum + m_nFreedNum == m_nElementNum);
		}

		void IncrementFreedNum()
		{
			m_nUsedNum--;
			m_nFreedNum++;
			UN_ASSERT(m_nUsedNum + m_nFreedNum == m_nElementNum);
		}

	protected:
		IMemoryAllocator* m_pAllocator;
		
		SElement* m_Elements;

		UN_UINT m_nLastNewPos;

		UN_UINT m_nElementNum;
		UN_UINT m_nUsedNum;
		UN_UINT m_nFreedNum;

		_T m_ClearDummyData;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_STD_RECYCLE_ARRAY_H__)
