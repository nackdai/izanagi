#if !defined(__URANUS_STD_OBEJCT_H__)
#define __URANUS_STD_OBEJCT_H__

#include "unDefs.h"
#include "StdPlacementNew.h"

namespace uranus {
	class CObject : public CPlacementNew {
		friend class CResource;

	protected:
		inline CObject();
		virtual ~CObject() {}

		NO_COPIABLE(CObject);

	public:
		// �Q�ƃJ�E���^����
		inline void AddRef();

		// ���
		inline void Release();

		// �Q�ƃJ�E���^�擾
		inline UN_INT GetRefCnt() const;

#if 0
	protected:
		void* operator new(size_t size) { return CPlacementNew::operator new(size); }
		void* operator new(size_t size, void* buf) { return CPlacementNew::operator new(size, buf); }

		void operator delete(void* data) { CPlacementNew::operator delete(data); }
		void operator delete(void* data, void* buf) { CPlacementNew::operator delete(data, buf); }
#endif

	private:
		virtual void InternalRelease() = 0;

		class CReleaseHandler {
		public:
			CReleaseHandler() {}
			virtual ~CReleaseHandler() {}
			PURE_VIRTUAL(void HandleRelease());
		};

		void HandleRelease()
		{
			if (m_pReleaseHandler != UN_NULL) {
				m_pReleaseHandler->HandleRelease();
			}
		}
		void SetReleaseHandler(CReleaseHandler* pHandler) { m_pReleaseHandler = pHandler; }

	private:
		// �Q�ƃJ�E���^
		UN_INT m_nRefCnt;

		CReleaseHandler* m_pReleaseHandler;
	};

	// �R���X�g���N�^
	CObject::CObject()
	{
		m_nRefCnt = 0;
		m_pReleaseHandler = UN_NULL;
	}

	// �Q�ƃJ�E���^����
	void CObject::AddRef()
	{
		m_nRefCnt++;
	}

	// ���
	void CObject::Release()
	{
		if (m_nRefCnt > 0) {
			m_nRefCnt--;

			if (m_nRefCnt == 0) {
				InternalRelease();
			}
			else {
				HandleRelease();
			}
		}
	}

	// �Q�ƃJ�E���^�擾
	UN_INT CObject::GetRefCnt() const
	{
		return m_nRefCnt;
	}
}	// namespace uranus

/**
* Utility for defining CObject::InternalRelease.
*/
#define UN_DEFINE_INTERNAL_RELEASE()\
	void InternalRelease() {\
		delete this;\
		FREE(m_pAllocator, this);\
	}

#endif	// #if !defined(__URANUS_STD_OBEJCT_H__)
