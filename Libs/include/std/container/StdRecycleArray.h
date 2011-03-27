#if !defined(__IZANAGI_STD_STD_RECYCLE_ARRAY_H__)
#define __IZANAGI_STD_STD_RECYCLE_ARRAY_H__

#include "izDefs.h"
#include "std/allocator/MemoryAllocator.h"

namespace izanagi {
	/**
	*/
	template <typename _T>
	class CRecycleArray {
		struct SElement {
			_T data;
			IZ_BOOL isUsed;
		};

	public:
		CRecycleArray()
		{
			m_pAllocator = IZ_NULL;
		
			m_Elements = IZ_NULL;

			m_nLastNewPos = 0;

			m_nElementNum = 0:
			m_nUsedNum = 0;
			m_nFreedNum = 0;
		}

		~CRecycleArray() { Clear(); }

		NO_COPIABLE(CRecycleArray);

	public:
		IZ_BOOL Init(
			IMemoryAllocator* pAllocator,
			IZ_UINT nNum)
		{
			IZ_ASSERT(pAllocator != IZ_NULL);
			IZ_ASSERT(nNum > 0);

			m_Elements = reinterpret_cast<SElement>(ALLOC_ZERO(pAllocator, sizeof(SElemet) * nNum));
			VRETURN(m_Elements != IZ_NULL);

			m_pAllocator = pAllocator;
			m_nElementNum = nNum;

			return IZ_TRUE;
		}

		IZ_BOOL Init(
			void* pBuf,
			IZ_UINT nNum)
		{
			IZ_ASSERT(pBuf != IZ_NULL);
			IZ_ASSERT(nNum > 0);

			m_Elements = reinterpret_cast<SElement>(pBuf);
			m_nElementNum = nNum;
		}

		void Clear()
		{
			if (m_pAllocator != IZ_NULL) {
				FREE(m_pAllocator, m_Elements);
			}
			m_Elements = IZ_NULL;
		}

		IZ_BOOL IsValid()
		{
			return (m_Elements != IZ_NULL);
		}

		_T* New()
		{
			IZ_ASSERT(IsValid());

			IZ_UINT nFreedPos = m_nLastNewPos;

			for (IZ_UINT i = 0; i < m_nElementNum; ++i) {
				if (m_Elements[nFreedPos].isUsed) {
					_T* ret = &m_Elements[nFreedPos].data;

					// Initialize data.
					*ret = m_ClearDummyData;

					m_Elements[nFreedPos].isUsed = IZ_TRUE;
					m_nLastNewPos = nFreedPos;
					IncrementUsedNum();

					return ret;
				}
				else {
					++nFreedPos;
					if (nFreedPos >= m_nElementNum) {
						nFreedPos = 0;
					}
				}
			}

			IZ_ASSERT(IZ_FALSE);
			return IZ_NULL;
		}

		void DeleteAll()
		{
			IZ_ASSERT(IsValid());

			for (IZ_UINT i = 0; i < m_nElementNum; ++i) {
				m_Elements[i].isUsed = IZ_FALSE;
			}
			Reset();
		}

		void Delete(IZ_UINT idx)
		{
			IZ_ASSERT(IsValid());
			IZ_ASSERT(idx < m_nElementNum);

			m_Elements[idx].isUsed = IZ_FALSE;
			IncrementFreedNum();
			return IZ_TRUE;
		}

		IZ_BOOL Delete(_T* pElem)
		{
			IZ_ASSERT(IsValid());
			IZ_ASSERT(pElem != IZ_NULL);

			IZ_BOOL ret = IZ_FALSE;

			for (IZ_UINT i = 0; i < m_nElementNum; ++i) {
				ret = (&m_Elements[idx].data == pElem);
				if (ret) {
					m_Elements[idx].isUsed = IZ_FALSE;
					IncrementFreedNum();
				}
			}

			return ret;
		}

		_T* GetAt(IZ_UINT idx)
		{
			IZ_ASSERT(IsValid());
			IZ_ASSERT(idx < m_nElementNum);

			return &m_Elements[i].data;
		}

		IZ_BOOL IsUsed(IZ_UINT idx)
		{
			IZ_ASSERT(IsValid());
			IZ_ASSERT(idx < m_nElementNum);

			return m_Elements[idx].isUsed;
		}

		IZ_UINT GetUsedNum() const  { return m_nUsedNum; }
		IZ_UINT GetFreedNum() const { return m_nFreedNum; }
		IZ_UINT GetTotalNum() const { return m_nElementNum; }

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
			++m_nUsedNum;
			--m_nFreedNum;
			IZ_ASSERT(m_nUsedNum + m_nFreedNum == m_nElementNum);
		}

		void IncrementFreedNum()
		{
			--m_nUsedNum;
			++m_nFreedNum;
			IZ_ASSERT(m_nUsedNum + m_nFreedNum == m_nElementNum);
		}

	protected:
		IMemoryAllocator* m_pAllocator;
		
		SElement* m_Elements;

		IZ_UINT m_nLastNewPos;

		IZ_UINT m_nElementNum;
		IZ_UINT m_nUsedNum;
		IZ_UINT m_nFreedNum;

		_T m_ClearDummyData;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_STD_RECYCLE_ARRAY_H__)
