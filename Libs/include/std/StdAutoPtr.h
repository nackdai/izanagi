#if !defined(__URANUS_STD_AUTO_PTR_H__)
#define __URANUS_STD_AUTO_PTR_H__

#include "unDefs.h"

namespace uranus {
	/**
	*/
	template <class _T>
	class CAutoPtr {
	public:
		CAutoPtr() { m_pBody = UN_NULL; }
		~CAutoPtr() { SAFE_RELEASE(m_pBody); }

		CAutoPtr(_T* p) { m_pBody = p; }

		CAutoPtr(const CAutoPtr& rhs) { m_pBody = rhs.m_pBody; }

		const CAutoPtr& operator=(const CAutoPtr& rhs) { m_pBody = rhs.m_pBody; }
		const CAutoPtr& operator=(_T* p) { m_pBody = p; }

	public:
		/**
		* Returns whether this is valid.
		*/
		UN_BOOL IsValid() const { return (m_pBody != UN_NULL); }

		_T* operator->()
		{
			UN_ASSERT(IsValid());
			return m_pBody;
		}

	private:
		_T* m_pBody;
	};
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_AUTO_PTR_H__)
