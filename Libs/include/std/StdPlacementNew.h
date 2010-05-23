#if !defined(__URANUS_STD_PLACEMENT_NEW_H__)
#define __URANUS_STD_PLACEMENT_NEW_H__

#include "unDefs.h"

namespace uranus {
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
		UN_ASSERT(UN_FALSE);
		return UN_NULL;
	}

	void* CPlacementNew::operator new(size_t size, void* buf)
	{
		return buf;
	}

	// delete
	void CPlacementNew::operator delete(void* data)
	{
		// ‰½‚à‚µ‚È‚¢
	}

	void CPlacementNew::operator delete(void* data, void* buf)
	{
		// ‰½‚à‚µ‚È‚¢
	}
}	// namespace uranus

#endif	// #if !defined(__URANUS_STD_PLACEMENT_NEW_H__)
