#if !defined(__IZANAGI_STD_AUTO_PTR_H__)
#define __IZANAGI_STD_AUTO_PTR_H__

#include "izDefs.h"

namespace izanagi {
	/**
	*/
	template <class _T>
	class CAutoPtr {
	public:
		CAutoPtr() { m_pBody = IZ_NULL; }
		~CAutoPtr() { SAFE_RELEASE(m_pBody); }

		CAutoPtr(_T* p) { m_pBody = p; }

		CAutoPtr(const CAutoPtr& rhs) { m_pBody = rhs.m_pBody; }

		const CAutoPtr& operator=(const CAutoPtr& rhs) { m_pBody = rhs.m_pBody; }
		const CAutoPtr& operator=(_T* p) { m_pBody = p; }

	public:
		/**
		* Returns whether this is valid.
		*/
		IZ_BOOL IsValid() const { return (m_pBody != IZ_NULL); }

		_T* operator->()
		{
			IZ_ASSERT(IsValid());
			return m_pBody;
		}

	private:
		_T* m_pBody;
	};
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_AUTO_PTR_H__)
