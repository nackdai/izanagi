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
		// 参照カウンタ増加
		inline void AddRef();

		// 解放
		inline void Release();

		// 参照カウンタ取得
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
		// 参照カウンタ
		UN_INT m_nRefCnt;

		CReleaseHandler* m_pReleaseHandler;
	};

	// コンストラクタ
	CObject::CObject()
	{
		m_nRefCnt = 0;
		m_pReleaseHandler = UN_NULL;
	}

	// 参照カウンタ増加
	void CObject::AddRef()
	{
		m_nRefCnt++;
	}

	// 解放
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

	// 参照カウンタ取得
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
