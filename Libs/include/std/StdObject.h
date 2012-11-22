#if !defined(__IZANAGI_STD_OBEJCT_H__)
#define __IZANAGI_STD_OBEJCT_H__

#include "izDefs.h"

namespace izanagi {
	class CObject {
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
		inline IZ_INT GetRefCnt() const;

	private:
		void* operator new(size_t size)
		{
			// Don't uset..
		}

	protected:
		void* operator new(size_t size, void* buf)
		{
			return buf;
		}

		void operator delete(void* data)
		{
			// Nothing is done.
		}

		void operator delete(void* data, void* buf)
		{
			// Nothing is done.
		}

	private:
		virtual void InternalRelease() = 0;

	private:
		// 参照カウンタ
		IZ_INT m_nRefCnt;
	};

	// コンストラクタ
	CObject::CObject()
	{
		m_nRefCnt = 0;
	}

	// 参照カウンタ増加
	void CObject::AddRef()
	{
		++m_nRefCnt;
	}

	// 解放
	void CObject::Release()
	{
		if (m_nRefCnt > 0) {
			m_nRefCnt--;

			if (m_nRefCnt == 0) {
				InternalRelease();
			}
		}
	}

	// 参照カウンタ取得
	IZ_INT CObject::GetRefCnt() const
	{
		return m_nRefCnt;
	}
}	// namespace izanagi

/**
* Utility for defining CObject::InternalRelease.
*/
#define IZ_DEFINE_INTERNAL_RELEASE()\
	void InternalRelease() {\
		delete this;\
		FREE(m_Allocator, this);\
	}

#endif	// #if !defined(__IZANAGI_STD_OBEJCT_H__)
