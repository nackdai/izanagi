#if !defined(__IZANAGI_STD_PLACEMENT_NEW_H__)
#define __IZANAGI_STD_PLACEMENT_NEW_H__

#include "izDefs.h"

namespace izanagi {
	class CPlacementNew {
	protected:
		CPlacementNew() {}
		~CPlacementNew() {}

		NO_COPIABLE(CPlacementNew);

	public:
		inline void* operator new(size_t size);
		inline void* operator new(size_t size, void* buf);

		inline void operator delete(void* data);
		inline void operator delete(void* data, void* buf);
	};

	// new
	void* CPlacementNew::operator new(size_t size)
	{
		IZ_ASSERT(IZ_FALSE);
		return IZ_NULL;
	}

	void* CPlacementNew::operator new(size_t size, void* buf)
	{
		return buf;
	}

	// delete
	void CPlacementNew::operator delete(void* data)
	{
		// 何もしない
	}

	void CPlacementNew::operator delete(void* data, void* buf)
	{
		// 何もしない
	}
}	// namespace izanagi

#endif	// #if !defined(__IZANAGI_STD_PLACEMENT_NEW_H__)
